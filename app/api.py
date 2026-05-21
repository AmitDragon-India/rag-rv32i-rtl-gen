"""FastAPI backend for RAG-RV32I RTL Generation pipeline."""

import time
import logging
from typing import Optional
from contextlib import asynccontextmanager

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Field

from app import __version__
from app.config import settings
from app.hybrid_retriever import hybrid_retriever
from app.generator import generator

logger = logging.getLogger(__name__)


# ─────────────────────────────────────────────
# Request / Response schemas
# ─────────────────────────────────────────────

class RetrieveRequest(BaseModel):
    query: str = Field(..., description="Retrieval query for the vector store")
    k_rtl: Optional[int] = Field(None, description="Number of RTL reference chunks to retrieve")
    rerank: bool = Field(default=True, description="Whether to apply cross-encoder reranking")


class RetrieveResponse(BaseModel):
    documents: list
    metadata: dict


class GenerateRequest(BaseModel):
    task: str = Field(..., description="Module generation task specification")
    retrieval_query: str = Field(..., description="Query to retrieve relevant context")
    task_label: str = Field(default="module", description="Label for the generated module")
    k_rtl: Optional[int] = Field(None, description="Number of RTL reference chunks")


class GenerateResponse(BaseModel):
    verilog: str
    validation: dict
    retrieval_metadata: dict
    generation_metadata: dict


class HealthResponse(BaseModel):
    status: str
    version: str
    retriever_loaded: bool
    retrieval_mode: str
    model: str
    uptime_seconds: float


# ─────────────────────────────────────────────
# App lifecycle
# ─────────────────────────────────────────────

_start_time = time.time()


@asynccontextmanager
async def lifespan(app: FastAPI):
    """Load hybrid retriever on startup, cleanup on shutdown."""
    logger.info("Starting RAG-RV32I API server...")
    try:
        hybrid_retriever.load(
            mode="hybrid",
            enable_reranker=True,
        )
        logger.info("Hybrid retriever loaded successfully")
    except Exception as e:
        logger.error(f"Failed to load hybrid retriever: {e}")
        # Fallback: try dense-only without reranker
        try:
            logger.info("Falling back to dense-only mode...")
            hybrid_retriever.load(mode="dense_only", enable_reranker=False)
            logger.info("Dense-only retriever loaded (BM25/reranker unavailable)")
        except Exception as e2:
            logger.error(f"Fallback also failed: {e2}")
            logger.warning("Server starting without retriever — endpoints will fail")
    yield
    logger.info("Shutting down RAG-RV32I API server")


# ─────────────────────────────────────────────
# FastAPI app
# ─────────────────────────────────────────────

app = FastAPI(
    title="RAG-RV32I RTL Generation API",
    description=(
        "Production API for generating synthesizable SystemVerilog RTL "
        "for a 5-stage in-order RV32I processor using hybrid RAG "
        "(FAISS dense + BM25 sparse + cross-encoder reranker) + LLM."
    ),
    version=__version__,
    lifespan=lifespan,
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


# ─────────────────────────────────────────────
# Endpoints
# ─────────────────────────────────────────────

@app.get("/health", response_model=HealthResponse)
async def health():
    """Health check endpoint."""
    return HealthResponse(
        status="healthy" if hybrid_retriever.is_loaded else "degraded",
        version=__version__,
        retriever_loaded=hybrid_retriever.is_loaded,
        retrieval_mode=hybrid_retriever._mode,
        model=settings.generation.model,
        uptime_seconds=round(time.time() - _start_time, 2),
    )


@app.post("/retrieve", response_model=RetrieveResponse)
async def retrieve_context(request: RetrieveRequest):
    """
    Retrieve relevant context documents using hybrid search.

    Pipeline: FAISS (dense) + BM25 (sparse) → RRF fusion → Cross-encoder reranking
    """
    if not hybrid_retriever.is_loaded:
        raise HTTPException(status_code=503, detail="Retriever not loaded")

    try:
        result = hybrid_retriever.retrieve(
            query=request.query,
            k_rtl=request.k_rtl,
            rerank=request.rerank,
        )

        docs_serialized = [
            {
                "content": doc.page_content[:500] + "..."
                if len(doc.page_content) > 500
                else doc.page_content,
                "source": doc.metadata.get("source", ""),
                "chunk_id": doc.metadata.get("chunk_id", ""),
                "type": doc.metadata.get("type", ""),
            }
            for doc in result["documents"]
        ]

        return RetrieveResponse(
            documents=docs_serialized,
            metadata=result["metadata"],
        )

    except Exception as e:
        logger.error(f"Retrieval failed: {e}")
        raise HTTPException(status_code=500, detail=str(e))


@app.post("/generate", response_model=GenerateResponse)
async def generate_rtl(request: GenerateRequest):
    """Generate SystemVerilog RTL for a given module specification."""
    if not hybrid_retriever.is_loaded:
        raise HTTPException(status_code=503, detail="Retriever not loaded")

    try:
        result = generator.generate(
            task=request.task,
            retrieval_query=request.retrieval_query,
            task_label=request.task_label,
            k_rtl=request.k_rtl,
        )

        return GenerateResponse(
            verilog=result["verilog"],
            validation=result["validation"],
            retrieval_metadata=result["retrieval_metadata"],
            generation_metadata=result["generation_metadata"],
        )

    except Exception as e:
        logger.error(f"Generation failed: {e}")
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/config")
async def get_config():
    """Return current pipeline configuration (non-sensitive)."""
    return {
        "embedding_model": settings.embedding.model_name,
        "vectorstore_type": settings.vectorstore.type,
        "generation_model": settings.generation.model,
        "retrieval_mode": hybrid_retriever._mode,
        "retrieval": {
            "default_k_rtl": settings.retrieval.default_k_rtl,
            "spec_sources": settings.retrieval.spec_sources,
            "pattern_sources": settings.retrieval.pattern_sources,
        },
    }


@app.post("/evaluate")
async def evaluate_retrieval():
    """
    Run retrieval evaluation on predefined test cases.

    Returns per-module metrics (hit rate, MRR, precision, source coverage)
    and aggregate scores.
    """
    if not hybrid_retriever.is_loaded:
        raise HTTPException(status_code=503, detail="Retriever not loaded")

    try:
        from app.evaluate import RetrievalEvaluator, EVAL_TEST_CASES

        evaluator = RetrievalEvaluator()
        evaluator._retriever = hybrid_retriever  # Reuse loaded retriever
        report = evaluator.evaluate_all(EVAL_TEST_CASES)

        return {
            "mode": report.mode,
            "total_tests": report.total_tests,
            "avg_hit_rate": report.avg_hit_rate,
            "avg_mrr": report.avg_mrr,
            "avg_precision": report.avg_precision,
            "avg_source_coverage": report.avg_source_coverage,
            "avg_retrieval_time_ms": report.avg_retrieval_time_ms,
            "test_results": report.test_results,
        }

    except Exception as e:
        logger.error(f"Evaluation failed: {e}")
        raise HTTPException(status_code=500, detail=str(e))


@app.post("/evaluate")
async def evaluate_retrieval(
    modules: Optional[list] = None,
    rerank: bool = True,
):
    """
    Evaluate retrieval quality against ground truth.

    Returns precision, recall, MRR, and hit rate per module + aggregated.
    """
    if not hybrid_retriever.is_loaded:
        raise HTTPException(status_code=503, detail="Retriever not loaded")

    try:
        from app.evaluator import PipelineEvaluator
        from dataclasses import asdict

        evaluator = PipelineEvaluator(hybrid_retriever)
        report = evaluator.evaluate_retrieval(modules=modules, rerank=rerank)

        return asdict(report)

    except Exception as e:
        logger.error(f"Evaluation failed: {e}")
        raise HTTPException(status_code=500, detail=str(e))
