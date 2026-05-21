"""Configuration loader for the RAG-RV32I pipeline."""

import yaml
import logging
from pathlib import Path
from dataclasses import dataclass, field
from typing import List

logger = logging.getLogger(__name__)

CONFIG_PATH = Path(__file__).parent.parent / "configs" / "config.yaml"


@dataclass
class EmbeddingConfig:
    model_name: str = "BAAI/bge-base-en-v1.5"


@dataclass
class VectorStoreConfig:
    type: str = "faiss"
    path: str = "vectorstore/faiss_index"


@dataclass
class CorpusConfig:
    dir: str = "corpus"
    chunks_file: str = "outputs/chunks.json"
    supported_extensions: List[str] = field(default_factory=lambda: [".md", ".v", ".sv"])
    max_rtl_chunks_per_file: int = 60
    chunk_max_chars: int = 2200
    chunk_overlap: int = 250


@dataclass
class RetrievalConfig:
    default_k_rtl: int = 3
    spec_candidates_k: int = 30
    pattern_candidates_k: int = 10
    max_spec_docs: int = 10
    max_pattern_docs: int = 2
    min_chunk_length: int = 200
    spec_sources: List[str] = field(default_factory=list)
    pattern_sources: List[str] = field(default_factory=list)


@dataclass
class GenerationConfig:
    model: str = "claude-haiku-4-5-20251001"
    max_tokens: int = 8192
    temperature: float = 0.2
    max_retries: int = 3
    retry_delay_seconds: int = 2


@dataclass
class OutputConfig:
    generated_rtl_dir: str = "outputs/generated_rtl"
    retrieved_context_dir: str = "outputs/retrieved_context"


@dataclass
class ServerConfig:
    host: str = "0.0.0.0"
    port: int = 8000


@dataclass
class AppConfig:
    embedding: EmbeddingConfig = field(default_factory=EmbeddingConfig)
    vectorstore: VectorStoreConfig = field(default_factory=VectorStoreConfig)
    corpus: CorpusConfig = field(default_factory=CorpusConfig)
    retrieval: RetrievalConfig = field(default_factory=RetrievalConfig)
    generation: GenerationConfig = field(default_factory=GenerationConfig)
    output: OutputConfig = field(default_factory=OutputConfig)
    server: ServerConfig = field(default_factory=ServerConfig)


def load_config(config_path: Path = CONFIG_PATH) -> AppConfig:
    """Load configuration from YAML file."""
    if not config_path.exists():
        logger.warning(f"Config file not found at {config_path}, using defaults")
        return AppConfig()

    with open(config_path, "r") as f:
        raw = yaml.safe_load(f)

    config = AppConfig(
        embedding=EmbeddingConfig(**raw.get("embedding", {})),
        vectorstore=VectorStoreConfig(**raw.get("vectorstore", {})),
        corpus=CorpusConfig(**raw.get("corpus", {})),
        retrieval=RetrievalConfig(**raw.get("retrieval", {})),
        generation=GenerationConfig(**raw.get("generation", {})),
        output=OutputConfig(**raw.get("output", {})),
        server=ServerConfig(**raw.get("server", {})),
    )

    logger.info(f"Config loaded from {config_path}")
    return config


# Global config instance
settings = load_config()
