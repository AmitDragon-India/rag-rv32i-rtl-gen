# RAG-Driven RV32I RTL Generation

A RAG pipeline that generates synthesizable SystemVerilog for a 5-stage in-order RV32I processor from behavioral specifications.

## Results
**40/42 rv32ui-p ISA tests passing (95.2%)** — Verilator simulation

| Metric | Value |
|--------|-------|
| ISA tests passing | 40/42 (95.2%) |
| Failing | fence_i (FENCE not implemented), ma_data (misaligned cross-word load) |
| LLM | Claude Haiku (claude-haiku-4-5-20251001) |
| Embedding | BAAI/bge-base-en-v1.5 |
| Vector store | FAISS |
| Simulator | Verilator 5.048 |

---

## Setup

### 1. Install Python dependencies

```bash
pip install -r requirement.txt
```

### 2. Build the RAG index

```bash
python3 scripts/chunk_corpus.py   # chunk corpus docs into ~180 chunks
python3 scripts/build_index.py    # embed chunks and build FAISS index
```

### 3. Generate RTL

```bash
python3 scripts/generate_rtl.py   # generates all 11 .sv files
```

Generated files appear in `outputs/generated_rtl/rv32i_*.sv`

---

## Simulation Setup

### 1. Install Verilator

```bash
# Ubuntu/WSL
sudo apt-get install verilator
# Or build from source for latest version:
# https://verilator.org/guide/latest/install.html
```

### 2. Install RISC-V toolchain

```bash
sudo apt-get install gcc-riscv64-unknown-elf
```

### 3. Clone and build riscv-tests

```bash
cd ~/
git clone https://github.com/riscv/riscv-tests
cd riscv-tests
git submodule update --init --recursive
autoconf
./configure --prefix=/usr/local
make
```

---

## Hex File Preparation

The riscv-tests binaries are linked at base address `0x80000000` but our processor starts execution at `0x00000000`. The memory also uses word-addressed hex format. Three steps are required:

### Step 1 — Relocate from 0x80000000 to 0x00000000

```bash
riscv64-unknown-elf-objcopy \
    --change-addresses -0x80000000 \
    rv32ui-p-add \
    rv32ui-p-add-relocated
```

This subtracts `0x80000000` from all addresses in the ELF binary. Code that was at `0x80000000` now starts at `0x00000000`, which is where our instruction memory begins.

### Step 2 — Convert ELF to Verilog hex format

```bash
riscv64-unknown-elf-objcopy \
    -O verilog \
    rv32ui-p-add-relocated \
    rv32ui-p-add-relocated.hex
```

This produces a hex file with `@ADDRESS` markers and byte-per-line format:
```
@00000000
6F 00 00 05 73 2F 20 34 ...
@00001000
FF 00 F0 0F 00 00 00 00 ...
```

The `@00001000` section contains the test data (e.g., `tohost` variable at `0x1000`, `begin_signature` at `0x2000`).

### Step 3 — Convert to word-addressed hex preserving address markers

```bash
python3 convert_hex_preserve_addr.py rv32ui-p-add-relocated.hex
```

This converts the byte-addressed hex to 32-bit word-addressed format **while preserving the `@` address markers**. This is critical — without preserving markers, `$readmemh` loads all data sequentially from index 0, so the data section at `0x2000` would end up at the wrong memory location.

Output: `rv32ui-p-add-relocated_word.hex` with format:
```
@00000000
0500006f
...
@00000400
0ff00f00
...
```

Word address = byte address / 4. So byte `0x2000` → word index `0x800`.

### Batch convert all tests

```bash
cd ~/riscv-tests/isa

for f in rv32ui-p-*; do
    if [ -f "$f" ] && [[ "$f" != *.hex ]] && [[ "$f" != *-relocated* ]]; then
        riscv64-unknown-elf-objcopy --change-addresses -0x80000000 "$f" "${f}-relocated"
        riscv64-unknown-elf-objcopy -O verilog "${f}-relocated" "${f}-relocated.hex"
        python3 ~/path/to/convert_hex_preserve_addr.py "${f}-relocated.hex"
        echo "Done: $f"
    fi
done
```

---

## Build and Run Simulation

### Compile with Verilator

```bash
cd ~/Amit/rag_rtl_project

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
   --exe sim_main.cpp \
   --top-module rv32i_top \
   -o sim
```

### Run a single test

```bash
./obj_dir/sim +load=/path/to/rv32ui-p-add-relocated_word.hex
```

### Run all tests

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

---

## Project Structure

```
rag-rv32i-rtl-gen/
├── corpus/
│   ├── docs/                    # RV32I spec documents (8 MD files)
│   ├── hardware_patterns/       # Verilog patterns and common bugs
│   └── reference_rtl/           # Minimal Ibex reference RTL
├── scripts/
│   ├── chunk_corpus.py          # Chunk and enrich corpus documents
│   ├── build_index.py           # Build FAISS vector index
│   ├── retrieve.py              # Three-stage structured retrieval
│   ├── generate_rtl.py          # RTL generation via Claude Haiku API
│   └── utils.py                 # Chunking and enrichment utilities
├── outputs/
│   ├── generated_rtl/           # Final generated SystemVerilog files
│   └── retrieved_context/       # Saved retrieval contexts per module
├── sim_main.cpp                 # Verilator testbench
├── convert_hex_preserve_addr.py # Hex conversion with address preservation
├── convert_hex_to_word.py       # Simple byte-to-word hex converter
└── requirement.txt              # Python dependencies
```

---

## Environment

- OS: Ubuntu 22.04 (WSL2)
- Python: 3.10+
- Verilator: 5.048
- RISC-V toolchain: riscv64-unknown-elf-gcc 10.2.0
