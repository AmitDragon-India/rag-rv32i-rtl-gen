# RAG for RISC-V RTL — Submission

| Field           | Details                                                                 |
|-----------------|-------------------------------------------------------------------------|
| **Name**        | Amit Chaudhari                                                          |
| **Email**       | amit37578@gmail.com                                                     |
| **Phone**       | +919586932537                                                           |
| **Country**     | India                                                                   |
| **Date**        | 30/04/2026                                                              |
| **LinkedIn**    | [Amit-Chaudhari](https://www.linkedin.com/in/amit-chaudhari-21b6ab157/)  |
| **GitHub**      | [AmitDragon-India](https://github.com/AmitDragon-India/) |

---

## A. Corpus & Knowledge Base

### Sources

The knowledge base was built entirely from hand-authored specification documents covering every aspect of an RV32I 5-stage in-order pipeline, based on the official RISC-V ISA specification. The corpus is organized into three categories:

**1. Specification Documents (`corpus/docs/`)** — 8 Markdown files describing the RV32I ISA and pipeline architecture:

| File | Contents | Origin |
|------|----------|--------|
| `01_formats.md` | RV32I instruction encoding, field positions, opcode map | Hand-authored from RISC-V ISA spec |
| `02_Instruction.md` | All RV32I instructions with operands and semantics | Hand-authored from RISC-V ISA spec |
| `03_pipeline_stages.md` | Per-stage interfaces, pipeline register specs, flush/stall rules | Human + AI collaboration |
| `04_architecture.md` | Control signal table, immediate generation, decode rules | Hand-authored from RISC-V ISA spec |
| `07_regfile_spec.md` | Register file behavioral spec including write-before-read bypass | Human + AI collaboration |
| `08_alu_spec.md` | ALU and ALU control behavioral specification | Human + AI collaboration |
| `09_hazard_control.md` | Forwarding unit, hazard unit, load-use and control hazard specs | Human + AI collaboration |
| `10_top_integration.md` | Top-level wiring rules, pipeline register flush policy, dmem spec | Human + AI collaboration |

**2. Hardware Patterns (`corpus/hardware_patterns/`)** — 2 files covering SystemVerilog coding conventions:

| File | Contents | Origin |
|------|----------|--------|
| `05_verilog_design_patterns.md` | always_comb, unique case, blocking/non-blocking guidelines | Base hand-authored, enhanced with AI |
| `06_common_rtl_bugs_and_fixes.md` | 18 common RTL mistakes with fixes | Base hand-authored, enhanced with AI |

**3. Reference RTL (`corpus/reference_rtl/ibex_rtl_only/`)** — 3 minimal reference files from the open-source Ibex RV32I core, used sparingly as structural reference only:
- `ibex_alu.sv`, `ibex_decoder.sv`, `ibex_register_file_ff.sv`

The reference RTL was kept deliberately minimal — the primary goal was spec-driven generation, not copying existing implementations. It served only as a sanity check on interface conventions.

All documents were **iteratively updated throughout the project** as simulation revealed specification gaps. For example, the write-before-read bypass requirement was added to `07_regfile_spec.md` only after `rv32ui-p-add` failed, and the EX/MEM flush policy was clarified in `10_top_integration.md` after branch tests regressed.

### Chunking

Documents were chunked using a custom pipeline in `scripts/chunk_corpus.py` and `scripts/utils.py`. Two different strategies were used depending on file type:

**Markdown files (spec documents):**
- Split on Markdown headings (`#`, `##`, `###`) using `split_markdown_by_headers()`
- Sections larger than 4000 characters are sub-chunked using `chunk_large_text()` with 3500 char max and 200 char overlap
- Code blocks (` ```systemverilog `) are never split mid-block — the chunker detects unbalanced backtick fences and adjusts boundaries
- Each chunk is then passed through `enrich_markdown_chunk()` which injects domain-specific keyword tags before embedding. For example a chunk containing `byte_off`, `LB`, `dmem_byte_en` gets tagged `Keywords: RV32I memory load store byte enable sign extension byte_off sub-word dmem`. This significantly improves retrieval for module-specific queries where pure semantic similarity is weak.

**RTL files (Ibex reference):**
- Split first by module boundary using `split_rtl_by_module()` on `endmodule` markers
- Each module is then split semantically using `split_rtl_module_semantically()` into: header/port declarations, individual `always_ff`/`always_comb`/`initial` blocks, and `assign` groups
- Capped at 60 chunks per file to prevent large reference files from dominating the FAISS index

Total: ~180 chunks across all documents.

### Embedding & Vector Store

- **Embedding model**: `BAAI/bge-base-en-v1.5` (768-dim, via `langchain_community.embeddings.HuggingFaceEmbeddings`) — chosen for strong technical retrieval performance and local execution without GPU
- **Vector store**: FAISS flat L2 index, built by `scripts/build_index.py` and saved to `vectorstore/faiss_index/`

### Retrieval Approach

Three-stage structured retrieval implemented in `scripts/retrieve.py` via `retrieve_structured()`:

1. **Spec search** — FAISS similarity search with top-30 candidates, filtered to spec documents only, capped at 10 chunks
2. **Pattern search** — separate FAISS query targeting `05_verilog_design_patterns.md` and `06_common_rtl_bugs_and_fixes.md` only, capped at 2 chunks
3. **RTL reference search** — FAISS candidates filtered to `.sv` type, capped at `k_rtl` (reduced to 2 for decode/ALU control queries to avoid Ibex decoder noise)

Results from all three stages are deduplicated and combined. The spec search uses a fixed priority re-ranking by source file to ensure architectural constraints (`04_architecture.md`, `02_Instruction.md`) always appear early in the context window, regardless of semantic similarity scores.

---

## B. Pipeline Design

### Architecture

```
┌──────────────────────────────────────────────────────┐
│                   RAG RTL Pipeline                   │
│                                                      │
│  Corpus (10 MD + 3 SV files)                         │
│       │                                              │
│       ▼                                              │
│  chunk_corpus.py  ──► chunks.json (~180 chunks)      │
│       │                                              │
│       ▼                                              │
│  build_index.py   ──► FAISS index                    │
│                       (BAAI/bge-base-en-v1.5)        │
│                                                      │
│  For each of 11 modules:                             │
│       │                                              │
│       ▼                                              │
│  retrieve.py      ──► Top-10 chunks                  │
│  (FAISS cosine + priority re-rank)                   │
│       │                                              │
│       ▼                                              │
│  generate_rtl.py  ──► Claude Haiku API               │
│  (task prompt + retrieved context)                   │
│       │                                              │
│       ▼                                              │
│  outputs/generated_rtl/*.sv                          │
└──────────────────────────────────────────────────────┘
```

### Tools and Models

| Component | Choice | Reason |
|-----------|--------|--------|
| LLM | Claude Haiku (`claude-haiku-4-5-20251001`) | Fast, cost-efficient, strong structured code generation |
| Embedding | `BAAI/bge-base-en-v1.5` (768-dim) | Strong technical retrieval, runs locally without GPU |
| Vector store | FAISS via `langchain_community.vectorstores` | Simple integration, no server needed, 180 chunks fits in memory |
| Embedding wrapper | `HuggingFaceEmbeddings` via `langchain_community` | Clean interface to sentence-transformers models |
| Framework | LangChain Community + Custom Python | LangChain for FAISS/embedding integration; custom logic for retrieval priority re-ranking and prompt assembly |
| Simulator | Verilator 5.048 | Industry-standard open-source RTL simulator |
| RISC-V toolchain | riscv64-unknown-elf-gcc 10.2.0 | Test compilation and hex conversion |

### Key Design Decisions

**1. Spec-only corpus — no full RTL examples**
Including Ibex reference RTL in the corpus was deliberately kept minimal. Full RTL risks the LLM copying rather than generating from specification. The three Ibex files were included only as structural reference for interface conventions — chunked separately and contributing far fewer index entries than the spec documents.

**2. Per-module task prompts with explicit interfaces**
Each of the 11 modules has a dedicated retrieval query and task prompt. Port names, widths, and types are spelled out explicitly in every task. Without explicit interfaces, the LLM invented port names that caused Verilator connection errors at integration time.

**3. Three-stage structured retrieval**
Pure FAISS retrieval sometimes returned generic Verilog patterns instead of module-specific architectural constraints. The three-stage retrieval separates spec docs, hardware patterns, and RTL reference into independent searches — each capped independently — ensuring balanced context coverage across all three categories.

**4. Top module uses prescriptive task**
The top-level integration module (`rv32i_top`) is pure wiring with no behavioral logic. Behavioral task descriptions produced incorrect port connections and missing signals. The top task therefore includes complete wire declarations and submodule instantiations as hard constraints. This is a deliberate trade-off: for structural integration, prescriptive prompting is necessary.

**5. Keyword enrichment improves retrieval precision**
Raw FAISS retrieval for memory-specific queries returned irrelevant chunks. Adding domain-specific keyword tags via `enrich_markdown_chunk()` in `utils.py` significantly improved precision for memory, forwarding, and hazard queries.

**6. Manual regeneration loop**
When generated RTL failed simulation, the debugging process was:
- Inspect the retrieved context to check if the right chunks were returned
- Determine whether the failure was a corpus gap (wrong/missing spec) or a model error
- If corpus gap: update the relevant spec document and re-chunk/re-index
- If model error: refine the task prompt with more explicit constraints
- Regenerate the module

This iterative human-in-the-loop process was responsible for most of the improvement from initial generation to the final 40/42 result. Full automation of the debug-and-regenerate loop — where the pipeline itself detects Verilator errors, identifies the failing spec, updates the corpus, and regenerates without human intervention — remains an open direction for future work.

---

## C. Generated RTL

All generated RTL is available at: **https://github.com/AmitDragon-India/rag-rv32i-rtl-gen**

The final pipeline consists of 11 SystemVerilog modules generated by Claude Haiku (`claude-haiku-4-5-20251001`) from retrieved corpus context. Below are three example traces showing the full prompt → retrieval → sort → generation flow.

### How the Pipeline Works

For each module, the pipeline follows these steps:

```
1. Task prompt (module name + interface + behavior constraints)
       │
       ▼
2. FAISS similarity search → top-30 candidates
       │
       ▼
3. Three-stage filter (spec docs / hardware patterns / RTL reference)
       │
       ▼
4. Priority sort (architectural specs first, patterns last)
       │
       ▼
5. Top ~12 chunks assembled into context window
       │
       ▼
6. Claude Haiku generates complete .sv module
```

---

### Trace 1: Register File (`rv32i_regfile`)

**Task Prompt (key behavior constraints):**
```
- Reads must use always_comb (NOT assign) to support write-before-read bypass
- Write-before-read bypass: if reg_write=1 and rd_addr matches rs1_addr
  or rs2_addr, return rd_data directly (not stale register value)
- Write only when reg_write=1 AND rd_addr != 5'b0
```

Each module has a dedicated **retrieval query** and **task prompt** defined in `scripts/generate_rtl.py`. To generate a module, set the `target` variable and run the script:

```python
target = "regfile"  # change to generate other modules
# Options: "id_stage", "alu", "alu_control", "regfile", "if_stage",
#          "ex_stage", "mem_stage", "hazard_unit", "forward_unit", "wb_stage", "top"
```

The retrieval query for that module is passed to `retrieve_structured()` in `scripts/retrieve.py` which uses it for FAISS similarity search. All 11 retrieval queries are defined in `RETRIEVAL_QUERIES`:

```python
RETRIEVAL_QUERIES = {
    "regfile":      "RV32I register file x0 hardwired read write port rd_addr reg_write \
                     write-before-read bypass rs1_addr rs2_addr always_comb",
    "hazard_unit":  "RV32I hazard unit load use hazard stall_pc stall_if_id flush_if_id \
                     flush_id_ex id_ex_mem_read id_ex_rd if_id_instr redirect_en ...",
    "mem_stage":    "RV32I memory stage load store byte enable sign extension LB LBU LH \
                     LHU LW SB SH SW byte_off dmem_byte_en dmem_wdata sub-word addressing",
    # ... (full queries in scripts/generate_rtl.py)
}
```

The retrieved chunks are combined with the task prompt and sent to Claude Haiku.

- **Full retrieval queries and task prompts**: defined in `scripts/generate_rtl.py` (`RETRIEVAL_QUERIES` and `TASKS` dictionaries)
- **Retrieved context per module**: saved in `outputs/retrieved_context/` (e.g., `regfile_claude_haiku_v1_context.txt`)
- **Generated RTL**: saved in `outputs/generated_rtl/rv32i_*.sv`

All available in the GitHub repository for complete reproducibility.

---

**Retrieved → Sorted Context (12 chunks):**

| Priority | Source | Chunk | Key Content |
|----------|--------|-------|-------------|
| 1 | `07_regfile_spec.md` | `sec_1` | Overview — x0 hardwired, two read ports |
| 2 | `07_regfile_spec.md` | `sec_3` | **Read port + write-before-read bypass requirement** |
| 3 | `07_regfile_spec.md` | `sec_4` | Write port — sync write, x0 guard |
| 4 | `07_regfile_spec.md` | `sec_5` | **Bypass timing explanation** |
| 5 | `07_regfile_spec.md` | `sec_6` | **Common bugs — missing bypass, using assign** |
| 6 | `07_regfile_spec.md` | `sec_2` | Interface definition |
| 7 | `04_architecture.md` | `sec_25` | store_data separate output |
| 8 | `04_architecture.md` | `sec_10` | alu_srcB encoding |
| 9 | `03_pipeline_stages.md` | `sec_7` | ID stage responsibility |
| 10 | `10_top_integration.md` | `sec_16` | Wiring rules |
| 11 | `05_verilog_design_patterns.md` | `sec_13` | Async reset convention |
| 12 | `05_verilog_design_patterns.md` | `sec_3` | Register with enable |

After priority sorting, all 6 `07_regfile_spec.md` chunks appear first — the LLM sees the bypass requirement before any generic patterns.

**Generated Output (key excerpt):**
```systemverilog
always_comb begin
    rs1_data = registers[rs1_addr];
    rs2_data = registers[rs2_addr];
    if (reg_write && (rd_addr == rs1_addr)) rs1_data = rd_data;
    if (reg_write && (rd_addr == rs2_addr)) rs2_data = rd_data;
    if (rs1_addr == 5'b0) rs1_data = 32'b0;
    if (rs2_addr == 5'b0) rs2_data = 32'b0;
end
```

✅ Correct — `always_comb`, bypass, x0 override all present. First generation passed.

---

### Trace 2: Memory Stage (`rv32i_mem_stage`)

**Task Prompt (key behavior constraints):**
```
- Use byte_off = alu_result[1:0] for sub-word byte addressing
- On mem_read: sign/zero extend based on funct3 and byte_off for LB/LBU/LH/LHU/LW
- On mem_write: generate dmem_byte_en and dmem_wdata based on funct3 and byte_off
```

**Retrieved → Sorted Context (top chunks):**

| Priority | Source | Chunk | Key Content |
|----------|--------|-------|-------------|
| 1 | `03_pipeline_stages.md` | `sec_20` | MEM stage interface and spec |
| 2 | `03_pipeline_stages.md` | `sec_21` | **byte_off encoding for all load/store variants** |
| 3 | `02_Instruction.md` | `sec_32–40` | LB/LBU/LH/LHU/LW/SB/SH/SW semantics |
| 4 | `05_verilog_design_patterns.md` | `sec_x` | unique case pattern |

After sorting, `03_pipeline_stages.md` byte_off spec appears before raw instruction semantics — LLM sees exact byte offset encoding first.

**Generated Output (key excerpt):**
```systemverilog
logic [1:0] byte_off;
assign byte_off = alu_result[1:0];

FUNCT3_LB: begin
    unique case (byte_off)
        2'b00: mem_rdata_extended = {{24{dmem_rdata[7]}},  dmem_rdata[7:0]};
        2'b01: mem_rdata_extended = {{24{dmem_rdata[15]}}, dmem_rdata[15:8]};
        2'b10: mem_rdata_extended = {{24{dmem_rdata[23]}}, dmem_rdata[23:16]};
        2'b11: mem_rdata_extended = {{24{dmem_rdata[31]}}, dmem_rdata[31:24]};
    endcase
end
```

✅ Correct. Tests lb, lbu, lh, lhu, lw all pass.

---

### Trace 3: Hazard Unit (`rv32i_hazard_unit`)

**Task Prompt (key behavior constraints):**
```
- On load_use_hazard: stall_pc=1, stall_if_id=1, flush_if_id=0, flush_id_ex=1
- On redirect_en: stall_pc=0, stall_if_id=0, flush_if_id=1, flush_id_ex=1
- flush_id_ex=1 on BOTH load-use AND redirect — this is critical
```

**Retrieved → Sorted Context (top chunks):**

| Priority | Source | Chunk | Key Content |
|----------|--------|-------|-------------|
| 1 | `09_hazard_control.md` | `sec_7` | **Output truth table — flush_id_ex=1 on both** |
| 2 | `09_hazard_control.md` | `sec_9` | Hazard unit behavior spec |
| 3 | `09_hazard_control.md` | `sec_3` | Load-use detection |
| 4 | `09_hazard_control.md` | `sec_1` | Overview |
| 5 | `03_pipeline_stages.md` | `sec_28` | Hazard summary table |
| 6 | `03_pipeline_stages.md` | `sec_32` | Pipeline register flush rules |

After sorting, `09_hazard_control.md` chunks dominate — the truth table with `flush_id_ex=1` on both conditions appears first.

**Generated Output (key excerpt):**
```systemverilog
if (load_use_hazard) begin
    stall_pc    = 1'b1;
    stall_if_id = 1'b1;
    flush_if_id = 1'b0;
    flush_id_ex = 1'b1;
end else if (redirect_en) begin
    stall_pc    = 1'b0;
    stall_if_id = 1'b0;
    flush_if_id = 1'b1;
    flush_id_ex = 1'b1;
end
```

✅ Correct. All branch tests pass after corpus was updated with explicit truth table.

---

### Generation Approach and Iterations

The pipeline used a **component-by-component** approach — each of the 11 modules was generated, verified for Verilator compilation, and tested independently before integration. This was deliberately chosen over holistic generation because module-level errors are easier to isolate, each module can be regenerated independently when its corpus spec is updated, and integration bugs are separated from generation bugs.

**Most modules generated correctly in a single iteration (~99%).** The IF stage and MEM stage occasionally required a second iteration — the IF stage due to parameter and plusarg loading edge cases, and the MEM stage because internal signal declarations (e.g., intermediate byte-extended values) were sometimes generated as unused signals when not explicitly constrained in the task.

The top module (`rv32i_top`) is the exception — it required switching from a behavioral task description to a prescriptive task with explicit wire declarations and submodule instantiations. Structural wiring cannot be adequately described behaviorally, and LLM-generated port connections were unreliable without exact constraints.

---

## D. Simulation Results

### Verilator Setup

- **Simulator**: Verilator 5.048
- **OS**: Ubuntu 22.04 (WSL2 on Windows)
- **Compile flags**: `--cc --exe --build -j 4 --Wall`

**Build command:**
```bash
verilator --cc --exe --build -j 4 --Wall \
   outputs/generated_rtl/rv32i_top.sv \
   outputs/generated_rtl/rv32i_if_stage.sv \
   outputs/generated_rtl/rv32i_id_stage.sv \
   outputs/generated_rtl/rv32i_alu_control.sv \
   outputs/generated_rtl/rv32i_alu.sv \
   outputs/generated_rtl/rv32i_regfile.sv \
   outputs/generated_rtl/rv32i_ex_stage.sv \
   outputs/generated_rtl/rv32i_mem_stage.sv \
   outputs/generated_rtl/rv32i_wb_stage.sv \
   outputs/generated_rtl/rv32i_hazard_unit.sv \
   outputs/generated_rtl/rv32i_forward_unit.sv \
   --exe sim_main.cpp --top-module rv32i_top -o sim
```

### Testbench Approach

The testbench (`sim_main.cpp`) uses two mechanisms to detect test completion:

**1. ecall interception** — riscv-tests signal pass/fail via `ecall`. When the testbench detects `ecall` in the EX stage (`id_ex_opcode=0x73, funct3=0, imm=0`), it reads `gp` (x3) directly from the regfile:
- `gp = 1` → PASS
- `gp = (test_num << 1) | 1` → FAIL at test_num

**2. tohost polling** — checks `dmem[0x1000>>2]` every cycle after cycle 10 to catch tests that write directly to the tohost memory-mapped register.

**Hex preparation pipeline:**
```bash
# 1. Relocate from 0x80000000 to 0x00000000
riscv64-unknown-elf-objcopy --change-addresses -0x80000000 test test-relocated

# 2. Convert ELF to Verilog hex
riscv64-unknown-elf-objcopy -O verilog test-relocated test-relocated.hex

# 3. Convert byte-addressed hex to word-addressed hex preserving @ markers
python3 convert_hex_preserve_addr.py test-relocated.hex
```

Step 3 is critical — preserving `@` address markers ensures `$readmemh` places the data section (e.g., `begin_signature` at byte `0x2000` = word index `0x800`) at the correct dmem location.

### ISA Test Results

**Final: 40/42 rv32ui-p tests passing (95.2%)**

| Test | Result | Cycles | CPI | Test | Result | Cycles | CPI |
|------|--------|--------|-----|------|--------|--------|-----|
| add | ✅ PASS | 538 | 1.27 | sb | ✅ PASS | 537 | 1.35 |
| addi | ✅ PASS | 297 | 1.08 | sh | ✅ PASS | 590 | 1.37 |
| and | ✅ PASS | 558 | 1.35 | simple | ✅ PASS | 82 | 0.78 |
| andi | ✅ PASS | 253 | 1.13 | sll | ✅ PASS | 566 | 1.25 |
| auipc | ✅ PASS | 105 | 0.81 | slli | ✅ PASS | 296 | 1.08 |
| beq | ✅ PASS | 386 | 1.34 | slt | ✅ PASS | 532 | 1.27 |
| bge | ✅ PASS | 422 | 1.35 | slti | ✅ PASS | 292 | 1.09 |
| bgeu | ✅ PASS | 447 | 1.38 | sltiu | ✅ PASS | 292 | 1.09 |
| blt | ✅ PASS | 386 | 1.34 | sltu | ✅ PASS | 532 | 1.27 |
| bltu | ✅ PASS | 411 | 1.37 | sra | ✅ PASS | 585 | 1.24 |
| bne | ✅ PASS | 390 | 1.35 | srai | ✅ PASS | 311 | 1.09 |
| fence_i | ❌ FAIL | 117 | — | srl | ✅ PASS | 579 | 1.25 |
| jal | ✅ PASS | 102 | 0.77 | srli | ✅ PASS | 305 | 1.09 |
| jalr | ✅ PASS | 182 | 1.05 | st_ld | ✅ PASS | 526 | 0.95 |
| lb | ✅ PASS | 311 | 1.16 | sub | ✅ PASS | 530 | 1.27 |
| lbu | ✅ PASS | 311 | 1.16 | sw | ✅ PASS | 597 | 1.38 |
| ld_st | ✅ PASS | 1155 | 1.12 | xor | ✅ PASS | 560 | 1.35 |
| lh | ✅ PASS | 327 | 1.17 | xori | ✅ PASS | 262 | 1.12 |
| lhu | ✅ PASS | 336 | 1.17 | ma_data | ❌ FAIL | 93 | — |
| lui | ✅ PASS | 108 | 0.79 | | | | |
| lw | ✅ PASS | 341 | 1.17 | | | | |
| or | ✅ PASS | 561 | 1.35 | | | | |
| ori | ✅ PASS | 260 | 1.13 | | | | |

### Additional Metrics

| Metric | Value |
|--------|-------|
| Total tests | 42 |
| Passing | 40 (95.2%) |
| Failing | 2 |
| Timeouts | 0 |
| Average CPI (passing tests) | ~1.20 |
| Best CPI (st_ld) | 0.95 |
| Worst CPI (sw, bgeu) | ~1.38 |
| Simple ALU avg CPI (addi, andi, slli) | ~1.10 |
| Branch-heavy avg CPI | ~1.35 |
| Load/store avg CPI | ~1.20 |

The CPI > 1.0 reflects the 2-cycle branch flush penalty and occasional load-use stalls. `st_ld` achieves near-1.0 CPI because it has many sequential stores/loads with few taken branches.

---

## E. Failure Analysis

### Failing Tests

**fence_i — FENCE.I not implemented**

`rv32ui-p-fence_i` failed because FENCE.I support was not implemented. The target core does not model instruction-cache synchronization or self-modifying code behavior. Our design uses a unified memory model with no instruction cache, so FENCE.I is treated as unsupported and out of scope for a basic RV32I integer core.

**ma_data — Misaligned word access not supported**

`rv32ui-p-ma_data` failed because the current data memory interface supports aligned word accesses and byte/halfword lane selection within a 32-bit word, but does not support cross-word misaligned word loads/stores. For example, `lw a4, 1(sp)` reads bytes from two adjacent 32-bit words — this requires either a second memory read port or a two-cycle memory access state machine, neither of which is implemented.

---

### Pipeline Generation Failures — Root Causes and Fixes

Most bugs were caused by **gaps or incorrect information in the corpus**, not model errors. The pipeline generated exactly what the corpus specified — when the spec was wrong or incomplete, the RTL was wrong.

---

**1. Missing write-before-read bypass in register file**

- **Symptom**: `rv32ui-p-add` failed at test 9 — ADD results were wrong when the previous instruction wrote to the same register
- **Root cause**: The original `07_regfile_spec.md` showed `assign` statements for reads — no bypass was possible. The corpus was missing the bypass requirement entirely.
- **Debug**: Added cycle-accurate debug prints showing `id_ex_rs1_data` had a stale value even though `regfile[rs1]` was correct. This pointed to a timing gap between WB write and ID read in the same cycle.
- **Fix**: Updated `07_regfile_spec.md` to explicitly require `always_comb` with write-before-read bypass. Regenerated — first generation after corpus update was correct.
- **Corpus or model?** Corpus gap.

---

**2. flush_id_ex=0 on redirect in hazard unit**

- **Symptom**: Branches were taken (correct redirect_pc) but wrong instructions continued executing — all branch tests failed at test 2
- **Root cause**: The corpus hazard spec mentioned `flush_if_id=1` on redirect but never explicitly stated `flush_id_ex=1` on redirect. The generated hazard unit set `flush_id_ex=1'b0` — the instruction already in ID/EX was not squashed.
- **Debug**: Added PC monitoring showing the branch fired correctly (`redirect_en=1`, `redirect_pc=0x10`) but at the next cycle the wrong instruction from ID/EX still executed.
- **Fix**: Added explicit truth table to `09_hazard_control.md` showing `flush_id_ex=1` on BOTH load-use AND redirect. Added `flush_id_ex=1 on BOTH conditions — this is critical` to task prompt.
- **Corpus or model?** Corpus gap — the asymmetric flush policy was never documented.

---

**3. EX/MEM pipeline register flushed on redirect**

- **Symptom**: JAL test regressed after the flush_id_ex fix — JAL was no longer writing back its return address
- **Root cause**: The generated top module used `!rst_n || flush_id_ex` as the EX/MEM flush condition. When a branch/jump fired `flush_id_ex=1`, the JAL instruction in EX/MEM had its writeback zeroed out — losing `pc_plus4`.
- **Debug**: Traced that JAL was executing correctly in EX but `mem_wb_reg_write=0` — the EX/MEM register had been flushed.
- **Fix**: Updated `10_top_integration.md` with explicit table showing EX/MEM resets on `rst_n` ONLY — never flushed.
- **Corpus or model?** Corpus gap — flush policy for EX/MEM was never specified.

---

**4. LUI and JAL forwarding wrong values**

- **Symptom**: R-type tests (and, or, xor) failed when a LUI preceded the R-type instruction — the forwarded value was wrong
- **Root cause**: The forwarding unit correctly asserted `forward_a=2'b10` for EX/MEM forwarding. But `ex_mem_alu_result` for LUI was `0 + imm_u` (via ADD) and for JAL was the jump target, not `pc_plus4`. The EX stage was forwarding the wrong value.
- **Debug**: Cycle-accurate tracing showed `id_ex_rs1_data=0x80000000` instead of `0xff010000` — the forwarded LUI value was incorrect.
- **Fix**: Added `ex_mem_fwd_data` mux to `rv32i_top.sv`:
  ```systemverilog
  case (ex_mem_wb_sel)
      2'b10:   ex_mem_fwd_data = ex_mem_pc_plus4;  // JAL/JALR
      2'b11:   ex_mem_fwd_data = ex_mem_imm_u;     // LUI
      default: ex_mem_fwd_data = ex_mem_alu_result; // others
  endcase
  ```
- **Corpus or model?** Corpus gap — the concept that wb_sel determines what gets forwarded was never documented.

---

### Common Failure Modes Summary

| Failure Mode | Cause | Fix |
|---|---|---|
| Wrong port names in top module | LLM invented names not in spec | Prescriptive task with explicit wire declarations |
| Missing pipeline register fields | Corpus didn't list all required fields | Added complete field list to `03_pipeline_stages.md` |
| Incorrect flush policy | Corpus spec was incomplete | Added explicit truth tables to corpus |
| Wrong forwarding value for LUI/JAL | Implicit design decision not in corpus | Added `ex_mem_fwd_data` mux and documented in corpus |
| Missing write-before-read bypass | Corpus used `assign` in example | Rewrote spec to require `always_comb` with bypass |

---

### Manual Fixes — Verilator Lint Suppression

The generated RTL compiled cleanly except for Verilator `UNUSED` signal warnings in 3 files. No logic was changed — only Verilator lint suppression pragmas were added manually:

```systemverilog
/* verilator lint_off UNUSED */
<signal declaration>
/* verilator lint_on UNUSED */
```

| File | Signal | Why unused |
|------|--------|------------|
| `rv32i_alu_control.sv` | `funct7[6:0]` | Only bit `[5]` is used to distinguish ADD/SUB and SRL/SRA |
| `rv32i_hazard_unit.sv` | `if_id_instr[31:0]` | Only bits `[19:15]` (rs1) and `[24:20]` (rs2) are extracted |
| `rv32i_top.sv` | `id_funct7` | Decoded in ID stage but not needed downstream |
| `rv32i_top.sv` | `ex_branch_taken` | Branch redirect handled via `redirect_en` — `branch_taken` is internal |
| `rv32i_top.sv` | `dmem_addr` | Verilator flagged as unused despite being connected to mem_stage |
| `rv32i_top.sv` | `dmem_re` | Read enable computed by MEM stage but dmem read is asynchronous assign |

These are architectural properties of the design, not generation errors. The suppression pragmas were added to allow clean compilation without modifying the generated logic.

---

## F. Reflection

### Hardest Part

The hardest part was debugging — specifically, tracing a failing ISA test back to the exact specification gap in the corpus. Hardware bugs are rarely obvious: a test fails 100 cycles in, you add debug prints, trace the PC cycle by cycle, find the wrong value in a register, then work backwards to find whether the issue was a wrong corpus spec, a missing corpus spec, or a retrieval problem. Having a strong background in RTL design helped significantly here — understanding what a 5-stage pipeline should do at each cycle made it possible to interpret the debug traces quickly. Without that domain knowledge, the debugging process would have been much slower.

The key lesson: **the quality of the corpus directly determines the quality of the generated RTL**. Every human mistake in the corpus — a missing signal, a wrong flush policy, an undocumented design decision — showed up as a pipeline bug. Writing a correct, complete corpus is harder than it looks.

---

### What I Would Do Differently

**1. Spend more time on corpus quality from the start.**
This project had human errors in the corpus that only surfaced during simulation debugging. The EX/MEM flush policy, the LUI forwarding correction, and the write-before-read bypass requirement were all missing or incorrect in early corpus versions. A more disciplined corpus review process — treating each spec document as a formal contract — would have prevented several debugging cycles.

**2. Start with stronger LLM models immediately.**
I initially spent 4-5 days with a local Llama model which produced poor RTL quality. Then switched to DeepSeek Coder which was better but slow, occasionally crashed, and required significant local GPU resources — costing another 2 days. Switching to Claude Haiku via API resolved both quality and reliability issues immediately. For future RAG-for-RTL projects, I would skip local models entirely and start with cloud API models from day one.

**3. Build golden test cases per module.**
Before full ISA testing, having a small set of directed test vectors per module (e.g., test the regfile bypass with back-to-back writes and reads to the same register) would catch module-level bugs earlier and reduce the integration debugging surface.

---

### Limits of RAG for Hardware Generation

This project revealed a fundamental tension in RAG for hardware: **hardware correctness is a global property but RAG retrieves and generates locally.**

RAG works well when:
- The module has clear, self-contained behavioral semantics (ALU, register file, MEM stage)
- The spec can be fully captured in natural language documents
- The behavior maps directly to what appears in the corpus

RAG struggles when:
- Design decisions span multiple modules (e.g., what value gets forwarded depends on wb_sel set in ID, forwarded in top, consumed in EX)
- Structural integration is required — the top module is pure wiring and LLMs consistently got port names wrong
- Implicit design invariants are involved — things every hardware engineer knows but never writes down, like "EX/MEM must never be flushed on redirect"

The deeper issue is that **the model cannot infer common sense about hardware behavior**. Every signal, every value, every interaction between modules must be explicitly documented in the corpus. If a spec says "forward the ALU result" but the correct behavior is "forward whichever value will actually be written to rd (which depends on wb_sel)", the model generates what the spec says — not what the designer intended.

This suggests that RAG for RTL generation needs to be augmented with either formal interface specifications, module-level golden tests, or automated integration testing — to catch the cross-module interactions that live in the designer's head rather than in any document.

---

*Submission prepared for Fermions ML Intern Application.*
*All results reproducible from: https://github.com/AmitDragon-India/rag-rv32i-rtl-gen*
