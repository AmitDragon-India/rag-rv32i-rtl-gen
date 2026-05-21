"""HuggingFace Spaces entry point for RAG-RV32I RTL Generation demo."""

import os
import sys
import logging
from pathlib import Path

# Setup logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(levelname)-8s | %(name)s | %(message)s",
)

logger = logging.getLogger(__name__)

# ─── Load UI and launch ───────────────────────────────────────────
from app.ui import build_ui, hybrid_retriever

# Load hybrid retriever on startup
logger.info("Loading hybrid retriever for HuggingFace Spaces...")
try:
    hybrid_retriever.load(mode="hybrid", enable_reranker=True)
    logger.info("Hybrid retriever loaded successfully")
except Exception as e:
    logger.warning(f"Hybrid retriever failed, trying dense-only: {e}")
    try:
        hybrid_retriever.load(mode="dense_only", enable_reranker=False)
        logger.info("Dense-only retriever loaded (fallback)")
    except Exception as e2:
        logger.error(f"Retriever failed to load: {e2}")

# Build and launch Gradio UI
demo = build_ui()
demo.launch()
