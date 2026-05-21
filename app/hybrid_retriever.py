"""Production hybrid retriever: FAISS (dense) + BM25 (sparse) + Reranker.

Retrieval pipeline:
    Query
      ├─→ FAISS dense search (semantic similarity)    → top 20 candidates
      ├─→ BM25 sparse search (exact keyword matching) → top 20 candidates
      └─→ Reciprocal Rank Fusion (combine both)       → top K fused
          └─→ Cross-encoder reranker (joint scoring)   → final ranked docs
"""

import time
import logging
from pathlib import Path
from typing import List, Dict, Any, Optional
from collections import defaultdict

from langchain_community.vectorstores import FAISS
from langchain_community.embeddings import HuggingFaceEmbeddings
from langchain_core.documents import Document

from app.config import settings

logger = logging.getLogger(__name__)


class HybridRetriever:
    """
    Hybrid retriever combining dense (FAISS) and sparse (BM25) search
    with Reciprocal Rank Fusion and optional cross-encoder reranking.
    """

    def __init__(self):
        self._faiss_db: Optional[FAISS] = None
        self._embeddings: Optional[HuggingFaceEmbeddings] = None
        self._bm25 = None
        self._reranker = None
        self._mode: str = "hybrid"  # "dense_only", "sparse_only", "hybrid"

    @property
    def is_loaded(self) -> bool:
        if self._mode == "dense_only":
            return self._faiss_db is not None
        elif self._mode == "sparse_only":
            return self._bm25 is not None and self._bm25.is_loaded
        else:  # hybrid
            return self._faiss_db is not None and self._bm25 is not None and self._bm25.is_loaded

    def load(self, mode: str = "hybrid", enable_reranker: bool = True) -> None:
        """
        Load retrieval components.
        
        Args:
            mode: "dense_only" (FAISS), "sparse_only" (BM25), or "hybrid" (both)
            enable_reranker: Whether to load cross-encoder reranker
        """
        self._mode = mode
        start = time.time()

        # ── Load FAISS (dense) ───────────────────────
        if mode in ("dense_only", "hybrid"):
            logger.info(f"Loading embedding model: {settings.embedding.model_name}")
            self._embeddings = HuggingFaceEmbeddings(
                model_name=settings.embedding.model_name
            )

            vs_path = settings.vectorstore.path
            logger.info(f"Loading FAISS index from: {vs_path}")
            self._faiss_db = FAISS.load_local(
                str(vs_path),
                self._embeddings,
                allow_dangerous_deserialization=True,
            )
            logger.info("FAISS index loaded")

        # ── Load BM25 (sparse) ──────────────────────
        if mode in ("sparse_only", "hybrid"):
            from app.bm25_index import bm25_index
            self._bm25 = bm25_index
            self._bm25.build()
            logger.info("BM25 index loaded")

        # ── Load Reranker ───────────────────────────
        if enable_reranker:
            from app.reranker import reranker
            self._reranker = reranker
            self._reranker.load()
            logger.info("Reranker loaded")

        elapsed = time.time() - start
        logger.info(f"HybridRetriever loaded in {elapsed:.2f}s (mode={mode}, reranker={enable_reranker})")

    def _deduplicate(self, docs: List[Document]) -> List[Document]:
        """Remove duplicate documents by source + chunk_id."""
        seen = set()
        unique = []
        for d in docs:
            key = (d.metadata.get("source", ""), d.metadata.get("chunk_id", ""))
            if key not in seen:
                seen.add(key)
                unique.append(d)
        return unique

    def _doc_key(self, doc: Document) -> str:
        """Unique key for a document."""
        return f"{doc.metadata.get('source', '')}::{doc.metadata.get('chunk_id', '')}"

    def _reciprocal_rank_fusion(
        self,
        ranked_lists: List[List[Document]],
        k: int = 60,
    ) -> List[Document]:
        """
        Reciprocal Rank Fusion (RRF) to combine multiple ranked lists.
        
        RRF score = Σ 1 / (k + rank_i) for each list where the doc appears.
        k=60 is the standard constant from the original paper (Cormack et al., 2009).
        
        Why RRF over simple score averaging?
        - Scores from FAISS (L2 distances) and BM25 (TF-IDF) are on different scales
        - RRF only uses RANK positions, making it scale-invariant
        - Simple, no tuning needed, proven effective
        """
        rrf_scores: Dict[str, float] = defaultdict(float)
        doc_map: Dict[str, Document] = {}

        for ranked_list in ranked_lists:
            for rank, doc in enumerate(ranked_list, start=1):
                key = self._doc_key(doc)
                rrf_scores[key] += 1.0 / (k + rank)
                doc_map[key] = doc  # Keep the latest version

        # Sort by RRF score descending
        sorted_keys = sorted(rrf_scores.keys(), key=lambda x: rrf_scores[x], reverse=True)

        return [doc_map[key] for key in sorted_keys]

    def _faiss_search(self, query: str, k: int = 20) -> List[Document]:
        """Dense semantic search via FAISS."""
        if self._faiss_db is None:
            return []
        return self._faiss_db.similarity_search(query, k=k)

    def _bm25_search(self, query: str, k: int = 20) -> List[Document]:
        """Sparse keyword search via BM25."""
        if self._bm25 is None or not self._bm25.is_loaded:
            return []
        results = self._bm25.search(query, k=k)
        return [r["document"] for r in results]

    def _sort_by_priority(self, docs: List[Document]) -> List[Document]:
        """Sort context by source priority for optimal prompt assembly."""
        priority_map = {
            "04_architecture.md": 0,
            "02_Instruction.md": 1,
            "01_formats.md": 2,
            "08_alu_spec.md": 3,
            "07_regfile_spec.md": 3,
            "03_pipeline_stages.md": 3,
            "09_hazard_control.md": 3,
            "10_top_integration.md": 3,
            "05_verilog_design_patterns.md": 4,
            "06_common_rtl_bugs_and_fixes.md": 4,
        }

        def get_priority(d):
            src = d.metadata.get("source", "")
            for name, p in priority_map.items():
                if name in src:
                    return p
            return 5

        return sorted(docs, key=get_priority)

    def retrieve(
        self,
        query: str,
        k_rtl: int = None,
        top_k: int = 15,
        rerank: bool = True,
    ) -> Dict[str, Any]:
        """
        Hybrid retrieval pipeline.

        1. Run FAISS (dense) and BM25 (sparse) searches in parallel
        2. Combine with Reciprocal Rank Fusion
        3. Apply structured filtering (spec → pattern → RTL)
        4. Optionally rerank with cross-encoder

        Returns dict with 'documents', 'metadata'.
        """
        if not self.is_loaded:
            raise RuntimeError("Retriever not loaded. Call load() first.")

        if k_rtl is None:
            k_rtl = settings.retrieval.default_k_rtl

        start = time.time()
        cfg = settings.retrieval

        # ═══════════════════════════════════════════════
        # STAGE 1: Dual search (dense + sparse)
        # ═══════════════════════════════════════════════
        faiss_t = time.time()
        faiss_docs = self._faiss_search(query, k=cfg.spec_candidates_k)
        faiss_elapsed = time.time() - faiss_t

        bm25_t = time.time()
        bm25_docs = self._bm25_search(query, k=cfg.spec_candidates_k)
        bm25_elapsed = time.time() - bm25_t

        logger.debug(
            f"Search: FAISS={len(faiss_docs)} in {faiss_elapsed*1000:.1f}ms, "
            f"BM25={len(bm25_docs)} in {bm25_elapsed*1000:.1f}ms"
        )

        # ═══════════════════════════════════════════════
        # STAGE 2: Reciprocal Rank Fusion
        # ═══════════════════════════════════════════════
        if self._mode == "dense_only":
            fused_docs = faiss_docs
        elif self._mode == "sparse_only":
            fused_docs = bm25_docs
        else:
            fused_docs = self._reciprocal_rank_fusion(
                [faiss_docs, bm25_docs], k=60
            )

        logger.debug(f"RRF fused: {len(fused_docs)} unique documents")

        # ═══════════════════════════════════════════════
        # STAGE 3: Structured filtering (same as before)
        # ═══════════════════════════════════════════════

        # Spec documents
        spec_docs = [
            d for d in fused_docs
            if any(src in d.metadata.get("source", "") for src in cfg.spec_sources)
            and len(d.page_content.strip()) > cfg.min_chunk_length
        ][:cfg.max_spec_docs]

        # Pattern documents (with query-adaptive pattern selection)
        if any(x in query.lower() for x in ["decode", "alu", "control", "immediate"]):
            pattern_query = "always_comb unique case default combinational latch prevention"
        else:
            pattern_query = "always_ff pipeline register stall flush reset non-blocking"

        # Search patterns separately (they need different query)
        pattern_faiss = self._faiss_search(pattern_query, k=cfg.pattern_candidates_k)
        pattern_bm25 = self._bm25_search(pattern_query, k=cfg.pattern_candidates_k)
        pattern_fused = self._reciprocal_rank_fusion([pattern_faiss, pattern_bm25], k=60)

        pattern_docs = [
            d for d in pattern_fused
            if any(src in d.metadata.get("source", "") for src in cfg.pattern_sources)
            and len(d.page_content.strip()) > cfg.min_chunk_length
        ][:cfg.max_pattern_docs]

        # RTL reference documents
        if any(x in query.lower() for x in ["decode", "instruction decode", "alu control"]):
            k_rtl = min(k_rtl, 2)

        rtl_docs = []
        if k_rtl > 0:
            rtl_docs = [
                d for d in fused_docs
                if d.metadata.get("type") == "rtl"
            ][:k_rtl]

        # Combine all
        all_docs = self._deduplicate(spec_docs + pattern_docs + rtl_docs)

        # ═══════════════════════════════════════════════
        # STAGE 4: Cross-encoder reranking (optional)
        # ═══════════════════════════════════════════════
        reranked = False
        rerank_time_ms = 0

        if rerank and self._reranker is not None and self._reranker.is_loaded and len(all_docs) > 1:
            rerank_t = time.time()
            reranked_results = self._reranker.rerank(
                query=query,
                documents=all_docs,
                top_k=min(top_k, len(all_docs)),
            )
            all_docs = [r["document"] for r in reranked_results]
            reranked = True
            rerank_time_ms = round((time.time() - rerank_t) * 1000, 2)
        else:
            # Sort by priority if not reranking
            all_docs = self._sort_by_priority(all_docs)

        elapsed = time.time() - start

        result = {
            "documents": all_docs,
            "metadata": {
                "query": query,
                "mode": self._mode,
                "k_rtl": k_rtl,
                "total_docs": len(all_docs),
                "spec_count": len(spec_docs),
                "pattern_count": len(pattern_docs),
                "rtl_count": len(rtl_docs),
                "faiss_results": len(faiss_docs),
                "bm25_results": len(bm25_docs),
                "fused_candidates": len(fused_docs),
                "reranked": reranked,
                "faiss_time_ms": round(faiss_elapsed * 1000, 2),
                "bm25_time_ms": round(bm25_elapsed * 1000, 2),
                "rerank_time_ms": rerank_time_ms,
                "total_retrieval_time_ms": round(elapsed * 1000, 2),
                "sources": [
                    {
                        "source": Path(d.metadata["source"]).name,
                        "chunk_id": d.metadata["chunk_id"],
                        "type": d.metadata.get("type", "unknown"),
                        "length": len(d.page_content),
                    }
                    for d in all_docs
                ],
            },
        }

        logger.info(
            f"Hybrid retrieval: {len(all_docs)} docs in {elapsed*1000:.1f}ms "
            f"(FAISS={len(faiss_docs)}, BM25={len(bm25_docs)}, "
            f"reranked={reranked})"
        )

        return result


# Singleton instance
hybrid_retriever = HybridRetriever()
