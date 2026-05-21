"""BM25 sparse keyword index for hybrid retrieval."""

import json
import re
import logging
import time
from pathlib import Path
from typing import List, Dict, Any, Optional

from langchain_core.documents import Document

from app.config import settings

logger = logging.getLogger(__name__)


class BM25Index:
    """
    BM25 sparse keyword index that complements FAISS dense retrieval.
    
    Why BM25 alongside FAISS?
    - FAISS (dense): good at semantic similarity ("register storage" ≈ "regfile")
    - BM25 (sparse): good at exact keyword matching ("always_comb" finds "always_comb")
    - Hardware queries often need EXACT signal/module names that dense embeddings miss
    """

    def __init__(self):
        self._index = None
        self._documents: List[Document] = []
        self._tokenized_corpus: List[List[str]] = []

    @property
    def is_loaded(self) -> bool:
        return self._index is not None

    def _tokenize(self, text: str) -> List[str]:
        """
        Hardware-aware tokenizer.
        
        Standard BM25 tokenizers split on whitespace/punctuation, which destroys
        signal names like 'always_comb', 'alu_srcA', 'ex_mem_rd'. We preserve
        underscored identifiers as single tokens.
        """
        # Lowercase
        text = text.lower()

        # Keep underscored identifiers intact (alu_op, always_comb, etc.)
        # Split on whitespace and common delimiters, but NOT underscores
        tokens = re.findall(r'[a-z][a-z0-9_]*[a-z0-9]|[a-z0-9]+', text)

        # Remove very short tokens (noise)
        tokens = [t for t in tokens if len(t) > 1]

        return tokens

    def build(self, chunks_file: str = None) -> None:
        """Build BM25 index from chunked corpus."""
        from rank_bm25 import BM25Okapi

        if chunks_file is None:
            chunks_file = settings.corpus.chunks_file

        chunks_path = Path(chunks_file)
        if not chunks_path.exists():
            raise FileNotFoundError(f"Chunks file not found: {chunks_path}")

        start = time.time()
        logger.info(f"Building BM25 index from {chunks_path}")

        with open(chunks_path, "r", encoding="utf-8") as f:
            raw_chunks = json.load(f)

        # Build document list and tokenized corpus
        self._documents = []
        self._tokenized_corpus = []

        for item in raw_chunks:
            doc = Document(
                page_content=item["text"],
                metadata={
                    "source": item["source"],
                    "chunk_id": item["chunk_id"],
                    "type": item["type"],
                },
            )
            self._documents.append(doc)
            self._tokenized_corpus.append(self._tokenize(item["text"]))

        # Build BM25 index
        self._index = BM25Okapi(self._tokenized_corpus)

        elapsed = time.time() - start
        logger.info(
            f"BM25 index built in {elapsed:.2f}s "
            f"({len(self._documents)} documents, "
            f"avg {sum(len(t) for t in self._tokenized_corpus) / len(self._tokenized_corpus):.0f} tokens/doc)"
        )

    def search(self, query: str, k: int = 20) -> List[Dict[str, Any]]:
        """
        Search BM25 index and return ranked results with scores.
        
        Returns list of dicts: {'document': Document, 'score': float, 'rank': int}
        """
        if not self.is_loaded:
            raise RuntimeError("BM25 index not built. Call build() first.")

        start = time.time()
        query_tokens = self._tokenize(query)

        # Get BM25 scores for all documents
        scores = self._index.get_scores(query_tokens)

        # Get top-k indices sorted by score (descending)
        top_indices = sorted(range(len(scores)), key=lambda i: scores[i], reverse=True)[:k]

        results = []
        for rank, idx in enumerate(top_indices):
            if scores[idx] > 0:  # Only include documents with non-zero scores
                results.append({
                    "document": self._documents[idx],
                    "score": float(scores[idx]),
                    "rank": rank + 1,
                })

        elapsed = time.time() - start
        logger.debug(
            f"BM25 search completed in {elapsed*1000:.1f}ms "
            f"(query_tokens={len(query_tokens)}, results={len(results)})"
        )

        return results


# Singleton instance
bm25_index = BM25Index()
