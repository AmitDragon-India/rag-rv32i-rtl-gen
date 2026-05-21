"""Production retriever with structured retrieval, logging, and metrics."""

import time
import logging
from pathlib import Path
from typing import List, Dict, Any, Optional

from langchain_community.vectorstores import FAISS
from langchain_community.embeddings import HuggingFaceEmbeddings
from langchain_core.documents import Document

from app.config import settings

logger = logging.getLogger(__name__)


class Retriever:
    """Production-grade retriever with FAISS vector search."""

    def __init__(self):
        self._db: Optional[FAISS] = None
        self._embeddings: Optional[HuggingFaceEmbeddings] = None
        self._load_time: Optional[float] = None

    @property
    def is_loaded(self) -> bool:
        return self._db is not None

    def load(self) -> None:
        """Load FAISS index and embedding model."""
        if self._db is not None:
            logger.info("Retriever already loaded, skipping")
            return

        start = time.time()
        logger.info(f"Loading embedding model: {settings.embedding.model_name}")

        self._embeddings = HuggingFaceEmbeddings(
            model_name=settings.embedding.model_name
        )

        vs_path = settings.vectorstore.path
        logger.info(f"Loading FAISS index from: {vs_path}")

        self._db = FAISS.load_local(
            str(vs_path),
            self._embeddings,
            allow_dangerous_deserialization=True,
        )

        self._load_time = time.time() - start
        logger.info(f"Retriever loaded in {self._load_time:.2f}s")

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

    def retrieve(self, query: str, k_rtl: int = None) -> Dict[str, Any]:
        """
        Structured retrieval: spec docs → pattern docs → RTL reference docs.

        Returns dict with 'documents', 'metadata' (timing, counts, sources).
        """
        if not self.is_loaded:
            raise RuntimeError("Retriever not loaded. Call load() first.")

        if k_rtl is None:
            k_rtl = settings.retrieval.default_k_rtl

        start = time.time()
        cfg = settings.retrieval

        # ── 1. Spec documents ────────────────────────────────
        spec_candidates = self._db.similarity_search(query, k=cfg.spec_candidates_k)
        spec_docs = [
            d for d in spec_candidates
            if any(src in d.metadata.get("source", "") for src in cfg.spec_sources)
            and len(d.page_content.strip()) > cfg.min_chunk_length
        ][:cfg.max_spec_docs]

        logger.debug(f"Spec docs: {len(spec_docs)} (from {len(spec_candidates)} candidates)")

        # ── 2. Hardware pattern documents ────────────────────
        if any(x in query.lower() for x in ["decode", "alu", "control", "immediate"]):
            pattern_query = "always_comb unique case default combinational latch prevention"
        else:
            pattern_query = "always_ff pipeline register stall flush reset non-blocking"

        pattern_candidates = self._db.similarity_search(pattern_query, k=cfg.pattern_candidates_k)
        pattern_docs = [
            d for d in pattern_candidates
            if any(src in d.metadata.get("source", "") for src in cfg.pattern_sources)
            and len(d.page_content.strip()) > cfg.min_chunk_length
        ][:cfg.max_pattern_docs]

        logger.debug(f"Pattern docs: {len(pattern_docs)}")

        # ── 3. RTL reference documents ───────────────────────
        if any(x in query.lower() for x in ["decode", "instruction decode", "alu control"]):
            k_rtl = min(k_rtl, 2)

        rtl_docs = []
        if k_rtl > 0:
            rtl_candidates = self._db.similarity_search(query, k=k_rtl * 3)
            rtl_docs = [
                d for d in rtl_candidates
                if d.metadata.get("type") == "rtl"
            ][:k_rtl]

        logger.debug(f"RTL docs: {len(rtl_docs)}")

        # ── Combine and deduplicate ──────────────────────────
        all_docs = self._deduplicate(spec_docs + pattern_docs + rtl_docs)

        # Sort by priority
        all_docs = self._sort_by_priority(all_docs)

        elapsed = time.time() - start

        result = {
            "documents": all_docs,
            "metadata": {
                "query": query,
                "k_rtl": k_rtl,
                "total_docs": len(all_docs),
                "spec_count": len(spec_docs),
                "pattern_count": len(pattern_docs),
                "rtl_count": len(rtl_docs),
                "retrieval_time_ms": round(elapsed * 1000, 2),
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
            f"Retrieved {len(all_docs)} docs in {elapsed*1000:.1f}ms "
            f"(spec={len(spec_docs)}, pattern={len(pattern_docs)}, rtl={len(rtl_docs)})"
        )

        return result

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


# Singleton instance
retriever = Retriever()
