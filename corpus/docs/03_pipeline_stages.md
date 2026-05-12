# RV32I Pipeline Stage Specifications

---

## Pipeline Overview

```
IF -> ID -> EX -> MEM -> WB
```

Pipeline registers: IF/ID, ID/EX, EX/MEM, MEM/WB

Only IF/ID and ID/EX are flushed on redirect. EX/MEM and MEM/WB are never flushed.

---

## Stage 1: Instruction Fetch (IF)

### Responsibility
- Read instruction from instruction memory at current PC
- Compute PC + 4
- Support stall (hold PC) and flush (output NOP)

### Interface

```systemverilog
module rv32i_if_stage #(
    parameter int    MEM_DEPTH = 8192,
    parameter string MEM_FILE  = ""
) (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        stall,
    input  logic        flush,
    input  logic [31:0] pc_next,
    output logic [31:0] instr,
    output logic [31:0] pc_out,
    output logic [31:0] pc_plus4
);
```

### Key Behavior
- PC resets to 0x00000000
- When stall=1, PC holds its current value
- When flush=1, instruction output is forced to NOP (32'h00000013)
- Memory loaded via `$readmemh` from `+load=` plusarg at runtime
- Word address = pc[clog2(MEM_DEPTH)+1:2]

---

## Stage 2: Instruction Decode (ID)

### Responsibility
- Decode instruction fields (opcode, funct3, funct7, rs1, rs2, rd)
- Generate all immediate types (I, S, B, U, J)
- Generate control signals (reg_write, mem_read, mem_write, branch, jump, alu_srcA, alu_srcB, wb_sel)
- Read register file
- Pass store_data (= rs2_data) as a SEPARATE output for the MEM write data path

### store_data separation
`store_data = rs2_data_in` — carried independently through ID/EX to EX stage so the forwarding unit can apply `forward_s` to it separately from `opB` (which may use immediate for store address).

---

## Stage 3: Execute (EX)

### Interface

```systemverilog
module rv32i_ex_stage (
    input  logic [31:0] rs1_data, rs2_data, store_data,
    input  logic [31:0] imm, pc_in, pc_plus4_in,
    input  logic        alu_srcA,
    input  logic [1:0]  alu_srcB,
    input  logic [3:0]  alu_op,
    input  logic [2:0]  funct3,
    input  logic        branch, jump,
    input  logic [6:0]  opcode,
    input  logic [1:0]  forward_a, forward_b, forward_s,
    input  logic [31:0] ex_mem_alu_result,  // corrected fwd value (see top-level mux)
    input  logic [31:0] wb_data,
    input  logic        mem_read, mem_write,
    input  logic [1:0]  wb_sel,
    input  logic        reg_write,
    input  logic [4:0]  rd,
    output logic [31:0] alu_result, store_data_fwd, pc_plus4_out, redirect_pc,
    output logic        branch_taken, redirect_en, reg_write_out, mem_read_out, mem_write_out,
    output logic [4:0]  rd_out,
    output logic [1:0]  wb_sel_out,
    output logic [31:0] imm_out
);
```

### Data Paths

```
rs1_data → alu_srcA mux → opA_pre → forward_a mux → opA_fwd → ALU operand_a
pc_in    →

rs2_data → alu_srcB mux → opB_pre → forward_b mux → opB_fwd → ALU operand_b
imm      →
32'd4    →

store_data ─────────────────────────→ forward_s mux → store_data_fwd → MEM write data
```

### alu_srcA encoding
- 1'b0 = rs1_data
- 1'b1 = pc_in (used by AUIPC, JAL)

### alu_srcB encoding
- 2'b00 = rs2_data (R-type, branch)
- 2'b01 = imm (I-type, load, store, AUIPC, LUI)
- 2'b10 = 32'd4 (not commonly used directly)

### Forward select encoding
- 2'b00 = no forward (use pre-mux value)
- 2'b01 = forward from MEM/WB (wb_data)
- 2'b10 = forward from EX/MEM (ex_mem_alu_result)

### EX/MEM Forwarding Value (top-level correction)
The `ex_mem_alu_result` input to EX stage is actually a mux in rv32i_top that selects:
- `ex_mem_alu_result` for R/I-type (wb_sel=00)
- `ex_mem_pc_plus4`  for JAL/JALR  (wb_sel=10)
- `ex_mem_imm_u`     for LUI       (wb_sel=11)

This ensures the forwarded value matches what will actually be written to rd.

### Redirect Logic (if/else — NOT unique case)
```
if (jump && opcode == JALR): redirect_en=1, redirect_pc = (opA_fwd + imm) & ~1
else if (jump):              redirect_en=1, redirect_pc = pc_in + imm
else if (branch_taken):      redirect_en=1, redirect_pc = pc_in + imm
```

### Branch Condition
Uses forwarded opA_fwd and opB_fwd. funct3 encoding: BEQ=000, BNE=001, BLT=100, BGE=101, BLTU=110, BGEU=111.

---

## Stage 4: Memory Access (MEM)

### Interface

```systemverilog
module rv32i_mem_stage (
    input  logic [31:0] alu_result, store_data, pc_plus4_in, imm_u_in,
    input  logic        mem_read, mem_write,
    input  logic [2:0]  funct3,
    input  logic [1:0]  wb_sel,
    input  logic        reg_write,
    input  logic [4:0]  rd_in,
    output logic [31:0] dmem_addr, dmem_wdata,
    output logic [3:0]  dmem_byte_en,
    output logic        dmem_we, dmem_re,
    input  logic [31:0] dmem_rdata,
    output logic [31:0] pc_plus4_out, imm_u_out, mem_rdata_out, alu_result_out,
    output logic        reg_write_out,
    output logic [4:0]  rd_out,
    output logic [1:0]  wb_sel_out
);
```

### Store (byte-addressed with byte_off)

```
byte_off = alu_result[1:0]

SB (funct3=000): dmem_byte_en = 4'b0001 << byte_off
                 dmem_wdata   = store_data << (8 * byte_off)

SH (funct3=001): dmem_byte_en = byte_off[1] ? 4'b1100 : 4'b0011
                 dmem_wdata   = store_data << (8 * byte_off)

SW (funct3=010): dmem_byte_en = 4'b1111
                 dmem_wdata   = store_data
```

### Load (byte-addressed with byte_off)

```
byte_off = alu_result[1:0]

LB  (funct3=000): sign-extend byte at byte_off position
LBU (funct3=100): zero-extend byte at byte_off position
LH  (funct3=001): sign-extend halfword (lower if byte_off[1]=0, upper if byte_off[1]=1)
LHU (funct3=101): zero-extend halfword
LW  (funct3=010): full 32-bit word (aligned)
```

### Memory Interface in Top Module

```systemverilog
// Synchronous write with byte enables
always_ff @(posedge clk) begin
    if (dmem_we) begin
        if (dmem_byte_en[0]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][7:0]   <= dmem_wdata[7:0];
        if (dmem_byte_en[1]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][15:8]  <= dmem_wdata[15:8];
        if (dmem_byte_en[2]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][23:16] <= dmem_wdata[23:16];
        if (dmem_byte_en[3]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][31:24] <= dmem_wdata[31:24];
    end
end
assign dmem_rdata = dmem[dmem_addr[DMEM_ADDR_BITS+1:2]];
```

### Limitation
Misaligned word loads crossing a 32-bit boundary are not supported. LW requires word-aligned address.

---

## Stage 5: Writeback (WB)

### Interface

```systemverilog
module rv32i_wb_stage (
    input  logic [31:0] alu_result,
    input  logic [31:0] mem_rdata,
    input  logic [31:0] pc_plus4_in,
    input  logic [31:0] imm_u_in,
    input  logic [1:0]  wb_sel,
    output logic [31:0] wb_data
);
```

### wb_sel encoding
- 2'b00 = ALU result (R/I-type, load address, AUIPC)
- 2'b01 = memory read data (loads)
- 2'b10 = PC+4 (JAL/JALR return address)
- 2'b11 = imm_u (LUI)

---

## Pipeline Registers

### IF/ID
Flushed on redirect or reset. Stalled on load-use hazard.
Holds: instr, pc, pc_plus4

### ID/EX
Flushed on redirect or reset. Holds all ID outputs including store_data.

### EX/MEM
**Never flushed** (only reset). Holds: alu_result, store_data_fwd, rd, funct3, wb_sel, reg_write, mem_read, mem_write, pc_plus4, imm_u.

### MEM/WB
**Never flushed** (only reset). Holds: alu_result, mem_rdata, rd, wb_sel, reg_write, pc_plus4, imm_u.

---

## Hazard Control Summary

| Condition       | stall_pc | stall_if_id | flush_if_id | flush_id_ex |
|----------------|----------|-------------|-------------|-------------|
| load-use hazard | 1        | 1           | 0           | 1           |
| redirect_en     | 0        | 0           | 1           | 1           |
| normal          | 0        | 0           | 0           | 0           |
