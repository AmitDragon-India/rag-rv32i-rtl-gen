"""Cross-encoder reranker for final-stage document ranking.

Why reranking?
- FAISS and BM25 score documents INDEPENDENTLY against the query
- A cross-encoder scores (query, document) PAIRS jointly — much more accurate
- But cross-encoders are slow (no pre-computation), so we only rerank top candidates
- Pipeline: FAISS + BM25 (fast, broad recall) → Reranker (slow, precise ranking)
"""

import time
import logging
from typing import List, Dict, Any, Optional

from langchain_core.documents import Document

from app.config import settings

logger = logging.getLogger(__name__)


class Reranker:
    """Cross-encoder reranker using sentence-transformers."""

    def __init__(self):
        self._model = None
        self._model_name: str = "cross-encoder/ms-marco-MiniLM-L-6-v2"

    @property
    def is_loaded(self) -> bool:
        return self._model is not None

    def load(self, model_name: str = None) -> None:
        """Load cross-encoder model."""
        from sentence_transformers import CrossEncoder

        if model_name:
            self._model_name = model_name

        start = time.time()
        logger.info(f"Loading reranker model: {self._model_name}")

        self._model = CrossEncoder(self._model_name)

        elapsed = time.time() - start
        logger.info(f"Reranker loaded in {elapsed:.2f}s")

    def rerank(
        self,
        query: str,
        documents: List[Document],
        top_k: int = 10,
    ) -> List[Dict[str, Any]]:
        """
        Rerank documents using cross-encoder scores.

        Args:
            query: The search query
            documents: List of candidate documents to rerank
            top_k: Number of top documents to return

        Returns:
            List of dicts with 'document', 'score', 'original_rank', 'reranked_rank'
        """
        if not self.is_loaded:
            raise RuntimeError("Reranker not loaded. Call load() first.")

        if not documents:
            return []

        start = time.time()

        # Create (query, document) pairs for cross-encoder
        pairs = [(query, doc.page_content) for doc in documents]

        # Score all pairs
        scores = self._model.predict(pairs)

        # Create ranked results
        scored_docs = []
        for i, (doc, score) in enumerate(zip(documents, scores)):
            scored_docs.append({
                "document": doc,
                "score": float(score),
                "original_rank": i + 1,
            })

        # Sort by cross-encoder score (descending)
        scored_docs.sort(key=lambda x: x["score"], reverse=True)

        # Add reranked position
        for i, item in enumerate(scored_docs):
            item["reranked_rank"] = i + 1

        # Trim to top_k
        results = scored_docs[:top_k]

        elapsed = time.time() - start
        logger.info(
            f"Reranked {len(documents)} docs → top {len(results)} in {elapsed*1000:.1f}ms"
        )

        # Log rank changes for debugging
        for r in results[:5]:
            rank_change = r["original_rank"] - r["reranked_rank"]
            direction = "↑" if rank_change > 0 else "↓" if rank_change < 0 else "="
            source = r["document"].metadata.get("chunk_id", "unknown")
            logger.debug(
                f"  {source}: {r['original_rank']} → {r['reranked_rank']} "
                f"({direction}{abs(rank_change)}) score={r['score']:.4f}"
            )

        return results


# Singleton instance
reranker = Reranker()
