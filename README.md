# RAG-Driven RV32I RTL Generation

A production-grade RAG pipeline that generates synthesizable SystemVerilog for a 5-stage in-order RV32I processor from behavioral specifications.

**40/42 rv32ui ISA tests passing (95.2%)** — validated under Verilator simulation.

## Architecture

```
                    ┌─────────────────────────────────────────────┐
                    │        Corpus: 335 chunks from 13 files     │
                    │  8 spec docs · design patterns · Ibex RTL   │
                    └──────────┬──────────────────────┬───────────┘
                               │                      │
                    ┌──────────▼──────────┐ ┌─────────▼──────────┐
                    │  FAISS Dense Index  │ │  BM25 Sparse Index │
                    │  (BGE embeddings)   │ │ (keyword matching) │
                    └──────────┬──────────┘ └─────────┬──────────┘
                               │                      │
                    ┌──────────▼──────────────────────▼──────────┐
User Query ──────►  │     Reciprocal Rank Fusion (RRF, k=60)     │
                    └──────────────────────┬─────────────────────┘
                                           │
                    ┌──────────────────────▼─────────────────────┐
                    │  Cross-Encoder Reranker (ms-marco-MiniLM)  │
                    └──────────────────────┬─────────────────────┘
                                           │
                    ┌──────────────────────▼─────────────────────┐
                    │   Structured Filtering (Spec→Pattern→RTL)  │
                    └──────────────────────┬─────────────────────┘
                                           │
                    ┌──────────────────────▼─────────────────────┐
                    │       Claude Haiku → SystemVerilog RTL     │
                    └────────────────────────────────────────────┘
```

**Key design decisions:**
- **Hybrid retrieval** — FAISS (semantic similarity) + BM25 (exact keyword matching), combined via Reciprocal Rank Fusion. FAISS catches semantic relationships (`register storage` ≈ `regfile`), BM25 catches exact signal names (`always_comb`, `stall_pc`)
- **Cross-encoder reranking** — `ms-marco-MiniLM-L-6-v2` jointly scores (query, document) pairs for final precision
- **3-stage structured filtering** — spec documents, design patterns, and RTL references retrieved with independent strategies per category
- **Semantic RTL chunking** — splits SystemVerilog by module boundaries and behavioral blocks (`always_ff`, `always_comb`), not arbitrary character counts
- **Hardware-aware BM25 tokenizer** — preserves underscored identifiers (`always_comb`, `alu_srcA`, `ex_mem_rd`) as single tokens instead of splitting on underscores

## Results

| Metric | Value |
|--------|-------|
| ISA tests passing | 40/42 (95.2%) |
| Failing tests | `fence_i` (FENCE not implemented), `ma_data` (misaligned cross-word load) |
| Modules generated | 11 SystemVerilog modules |
| Corpus | 335 chunks (261 markdown, 74 RTL) from 13 source files |
| LLM | Claude Haiku (`claude-haiku-4-5-20251001`) |
| Embedding model | BAAI/bge-base-en-v1.5 |
| Reranker | cross-encoder/ms-marco-MiniLM-L-6-v2 |
| Simulator | Verilator 5.048 |

Both dense-only (FAISS) and hybrid (FAISS + BM25 + reranker) retrieval produce 40/42 passing RTL. The hybrid pipeline adds BM25 keyword matching and cross-encoder reranking for improved context selection, while both achieve the same functional correctness.

## Quick Start

### 1. Install dependencies

```bash
pip install -r requirements.txt
```

### 2. Set API key

```bash
export ANTHROPIC_API_KEY="your-key-here"
```

### 3. Build the RAG index

```bash
python scripts/chunk_corpus.py    # chunk corpus → outputs/chunks.json
python scripts/build_index.py     # embed + build FAISS index
```

### 4. Generate RTL

```bash
# Generate all 11 modules (hybrid retrieval)
python scripts/generate_hybrid.py

# Generate a single module
python scripts/generate_hybrid.py --module alu
```

### 5. Run the API server

```bash
python main.py api
# → http://localhost:8000/docs (Swagger UI)
```

### 6. Or launch the Gradio UI

```bash
python main.py ui
# → http://localhost:7860
```

### 7. Or use Docker

```bash
docker build -t rag-rv32i .
docker run -p 8000:8000 -e ANTHROPIC_API_KEY=$ANTHROPIC_API_KEY rag-rv32i
```

## API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| `GET` | `/health` | Health check with retrieval mode, uptime |
| `POST` | `/retrieve` | Hybrid retrieval with timing breakdown |
| `POST` | `/generate` | Full pipeline: retrieve → generate → validate |
| `POST` | `/evaluate` | Run evaluation across all 11 modules |
| `GET` | `/config` | Current pipeline configuration |

### Example: Retrieve context

```bash
curl -X POST http://localhost:8000/retrieve \
  -H "Content-Type: application/json" \
  -d '{
    "query": "RV32I ALU operations ADD SUB alu_op encoding",
    "k_rtl": 2,
    "rerank": true
  }'
```

## Evaluation

The primary evaluation metric is **functional correctness**: does the generated RTL pass RISC-V ISA tests? Both dense-only and hybrid retrieval achieve 40/42 (95.2%).

For retrieval quality, the project includes an auto-calibration framework that captures baseline retrieval results and compares when switching strategies — no manually curated ground truth needed.

```bash
# Capture baseline from working retrieval
python -m app.calibrate --mode baseline

# Compare hybrid against baseline
python -m app.calibrate --mode compare

# Dense vs hybrid evaluation
python main.py eval
```

## Simulation

### Compile with Verilator

```bash
verilator --cc --exe --build -j 4 --Wall \
   outputs/generated_rtl/rv32i_*.sv \
   --exe sim_main.cpp \
   --top-module rv32i_top -o sim
```

### Run ISA tests

```bash
pass=0; fail=0; timeout=0
for hex in ~/riscv-tests/isa/rv32ui-p-*-relocated_word.hex; do
    result=$(./obj_dir/sim +load=$hex 2>&1)
    echo "$result"
    if echo "$result" | grep -q "^PASS"; then ((pass++))
    elif echo "$result" | grep -q "^FAIL"; then ((fail++))
    else ((timeout++)); fi
done
echo "Results: PASS=$pass FAIL=$fail TIMEOUT=$timeout"
```

## Hex File Preparation

The riscv-tests binaries are linked at `0x80000000` but the processor starts at `0x00000000`. Three conversion steps:

```bash
# Relocate addresses
riscv64-unknown-elf-objcopy --change-addresses -0x80000000 rv32ui-p-add rv32ui-p-add-relocated

# Convert to Verilog hex format
riscv64-unknown-elf-objcopy -O verilog rv32ui-p-add-relocated rv32ui-p-add-relocated.hex

# Convert to word-addressed hex (preserving @address markers)
python3 convert_hex_preserve_addr.py rv32ui-p-add-relocated.hex
```

Batch convert all tests:

```bash
cd ~/riscv-tests/isa
for f in rv32ui-p-*; do
    if [ -f "$f" ] && [[ "$f" != *.hex ]] && [[ "$f" != *-relocated* ]]; then
        riscv64-unknown-elf-objcopy --change-addresses -0x80000000 "$f" "${f}-relocated"
        riscv64-unknown-elf-objcopy -O verilog "${f}-relocated" "${f}-relocated.hex"
        python3 ~/path/to/convert_hex_preserve_addr.py "${f}-relocated.hex"
    fi
done
```

## Project Structure

```
rag-rv32i-rtl-gen/
├── app/
│   ├── config.py                  # YAML config loader
│   ├── retriever.py               # Dense-only FAISS retriever
│   ├── bm25_index.py              # BM25 sparse keyword index
│   ├── reranker.py                # Cross-encoder reranker
│   ├── hybrid_retriever.py        # FAISS + BM25 + RRF + reranker
│   ├── generator.py               # RTL generator with retry + validation
│   ├── api.py                     # FastAPI backend
│   ├── ui.py                      # Gradio interactive UI
│   ├── evaluate.py                # Retrieval evaluation framework
│   └── calibrate.py               # Auto-calibration (baseline → compare)
├── scripts/
│   ├── chunk_corpus.py            # Semantic RTL + enriched markdown chunking
│   ├── build_index.py             # FAISS index builder
│   ├── generate_rtl_2.py          # RTL generation (dense retrieval)
│   ├── generate_hybrid.py         # RTL generation (hybrid retrieval)
│   └── utils.py                   # Chunking and RTL parsing utilities
├── configs/
│   └── config.yaml                # Centralized pipeline configuration
├── corpus/
│   ├── docs/                      # RV32I spec documents (8 markdown files)
│   ├── hardware_patterns/         # Verilog design patterns and common bugs
│   └── reference_rtl/             # Minimal Ibex reference RTL
├── outputs/
│   ├── generated_rtl_2/           # Generated SystemVerilog (dense)
|   ├── retrieved_context_2/       # Saved retrieval contexts per module (dense)
│   ├── generated_rtl_hybrid/      # Generated SystemVerilog (hybrid)
|   └── retrieved_context_hybrid/  # Saved retrieval contexts per module (hybrid)   
├── vectorstore/                   # FAISS index files
├── convert_hex_preserve_addr.py   # Hex conversion with address preservation
├── convert_hex_to_word.py         # Simple byte-to-word hex converter
├── main.py                        # Entry point (api / ui / eval)
├── Dockerfile                     # Container deployment
├── sim_main.cpp                   # Verilator testbench
├── requirements.txt               # Python dependencies
└── README.md
```

## Tech Stack

- **Retrieval:** FAISS, BM25, LangChain, HuggingFace Embeddings (BGE)
- **Reranking:** sentence-transformers (cross-encoder/ms-marco-MiniLM-L-6-v2)
- **Generation:** Anthropic Claude API
- **API:** FastAPI, Pydantic, Uvicorn
- **UI:** Gradio
- **Deployment:** Docker
- **Validation:** Verilator, RISC-V ISA test suite

## Environment

- OS: Ubuntu 22.04 (WSL2)
- Python: 3.10+
- Verilator: 5.048
- RISC-V toolchain: riscv64-unknown-elf-gcc 10.2.0

## License

The project code (`app/`, `scripts/`, `configs/`, `main.py`) is licensed under the [MIT License](LICENSE).

The Ibex RISC-V reference RTL used as retrieval context is Copyright (c) lowRISC contributors, licensed under [Apache 2.0](https://github.com/lowRISC/ibex/blob/master/LICENSE). See [NOTICE](NOTICE) for full attribution.
