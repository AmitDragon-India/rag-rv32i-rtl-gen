# RV32I Top-Level Integration Specification

---

## Overview

The top-level module (`rv32i_top`) instantiates and connects all pipeline stages,
pipeline registers, data memory, register file, hazard unit, and forwarding unit.

---

## Module Hierarchy

```
rv32i_top
├── rv32i_if_stage       (Instruction Fetch — includes internal imem)
├── rv32i_id_stage       (Instruction Decode)
├── rv32i_alu_control    (ALU operation decoder)
├── rv32i_regfile        (Register File — read in ID, write in WB)
├── rv32i_ex_stage       (Execute — includes rv32i_alu internally)
├── rv32i_mem_stage      (Memory Access)
├── rv32i_wb_stage       (Writeback — wb_sel mux)
├── rv32i_hazard_unit    (Stall and flush control)
├── rv32i_forward_unit   (Forwarding control)
├── Data memory          (logic array inside rv32i_top)
└── Pipeline registers   (IF/ID, ID/EX, EX/MEM, MEM/WB — inside rv32i_top)
```

---

## Top-Level Interface

```systemverilog
module rv32i_top #(
    parameter int    MEM_DEPTH = 8192,
    parameter string MEM_FILE  = ""
) (
    input  logic clk,
    input  logic rst_n
);
```

Notes:
- MEM_DEPTH default is 8192 (32KB) — covers code at 0x0000 and data at 0x2000
- No external memory ports — instruction memory is inside rv32i_if_stage
- Data memory array is inside rv32i_top

---

## EX/MEM Forwarding Value Correction

A critical mux in rv32i_top corrects the forwarded value from EX/MEM to match
what will actually be written to rd:

```systemverilog
logic [31:0] ex_mem_fwd_data;

always_comb begin
    case (ex_mem_wb_sel)
        2'b10:   ex_mem_fwd_data = ex_mem_pc_plus4;   // JAL/JALR writes pc_plus4
        2'b11:   ex_mem_fwd_data = ex_mem_imm_u;      // LUI writes imm_u
        default: ex_mem_fwd_data = ex_mem_alu_result;  // R/I/Load writes alu_result
    endcase
end
```

The EX stage receives `ex_mem_fwd_data` (not raw `ex_mem_alu_result`) as its
`.ex_mem_alu_result` input. Without this, forwarding from LUI or JAL/JALR
produces wrong operand values.

---

## Pipeline Register Flush Rules

| Register | Flush condition          | Reset condition |
|----------|--------------------------|-----------------|
| IF/ID    | flush_if_id OR rst_n     | rst_n           |
| ID/EX    | flush_id_ex OR rst_n     | rst_n           |
| EX/MEM   | rst_n ONLY               | rst_n           |
| MEM/WB   | rst_n ONLY               | rst_n           |

EX/MEM and MEM/WB are **never flushed** — only reset. The instruction in EX
(branch/jump source) must complete and write back correctly.

---

## Data Memory

```systemverilog
localparam int DMEM_ADDR_BITS = $clog2(MEM_DEPTH);

logic [31:0] dmem [0:MEM_DEPTH-1];

// Load hex file and clear tohost at simulation start
initial begin
    string load_file;
    if (MEM_FILE != "")
        $readmemh(MEM_FILE, dmem);
    else if ($value$plusargs("load=%s", load_file))
        $readmemh(load_file, dmem);
    // Clear tohost (0x1000>>2 = 0x400) to avoid false positive at cycle 0
    dmem[32'h1000 >> 2] = 32'b0;
end

// Synchronous byte-enable write
always_ff @(posedge clk) begin
    if (dmem_we) begin
        if (dmem_byte_en[0]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][7:0]   <= dmem_wdata[7:0];
        if (dmem_byte_en[1]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][15:8]  <= dmem_wdata[15:8];
        if (dmem_byte_en[2]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][23:16] <= dmem_wdata[23:16];
        if (dmem_byte_en[3]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][31:24] <= dmem_wdata[31:24];
    end
end

// Asynchronous read
assign dmem_rdata = dmem[dmem_addr[DMEM_ADDR_BITS+1:2]];
```

---

## pc_next Selection

```systemverilog
always_comb begin
    if (ex_redirect_en)
        pc_next = ex_redirect_pc;
    else
        pc_next = if_pc_plus4;
end
```

---

## IF/ID Pipeline Register

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n || flush_if_id) begin
        if_id_instr    <= 32'h00000013;  // NOP
        if_id_pc       <= 32'b0;
        if_id_pc_plus4 <= 32'b0;
    end else if (!stall_if_id) begin
        if_id_instr    <= if_instr;
        if_id_pc       <= if_pc_out;
        if_id_pc_plus4 <= if_pc_plus4;
    end
end
```

---

## ID/EX Pipeline Register

Flushed on `rst_n` or `flush_id_ex`. Carries all ID stage outputs including
`store_data` (separate from rs2_data).

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n || flush_id_ex) begin
        // Zero all control signals and data
        id_ex_reg_write <= 1'b0; id_ex_mem_read <= 1'b0;
        // ... all fields zeroed
    end else begin
        id_ex_reg_write  <= id_reg_write;
        id_ex_store_data <= id_store_data;  // separate store path
        id_ex_imm        <= id_imm;
        // ... all fields captured
    end
end
```

---

## EX/MEM Pipeline Register

Reset on `rst_n` ONLY — **never flushed on redirect**.
`ex_mem_imm_u` is captured from `ex_imm_out` (EX stage imm_out port), NOT `id_ex_imm`.

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        ex_mem_reg_write <= 1'b0; // ... all zeroed
    end else begin
        ex_mem_reg_write  <= ex_reg_write_out;   // from EX stage output
        ex_mem_mem_read   <= ex_mem_read_out;    // from EX stage output
        ex_mem_mem_write  <= ex_mem_write_out;   // from EX stage output
        ex_mem_wb_sel     <= ex_wb_sel_out;      // from EX stage output
        ex_mem_alu_result <= ex_alu_result;      // from EX stage
        ex_mem_store_data <= ex_store_data_fwd;  // forwarded store data from EX
        ex_mem_rd         <= ex_rd_out;          // from EX stage
        ex_mem_funct3     <= id_ex_funct3;       // from ID/EX register
        ex_mem_pc_plus4   <= ex_pc_plus4_out;    // from EX stage
        ex_mem_imm_u      <= ex_imm_out;         // from EX stage imm_out (NOT id_ex_imm)
    end
end
```

---

## MEM/WB Pipeline Register

Reset on `rst_n` ONLY — **never flushed**.

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        mem_wb_reg_write <= 1'b0; // ... all zeroed
    end else begin
        mem_wb_reg_write  <= mem_reg_write_out;
        mem_wb_wb_sel     <= mem_wb_sel_out;
        mem_wb_alu_result <= mem_alu_result_out;
        mem_wb_mem_rdata  <= mem_rdata_out;      // sign/zero extended from MEM stage
        mem_wb_rd         <= mem_rd_out;
        mem_wb_pc_plus4   <= mem_pc_plus4_out;
        mem_wb_imm_u      <= mem_imm_u_out;
    end
end
```

---

## Wiring Rules

1. IF stage has internal imem — no external imem ports on rv32i_top
2. `alu_control` reads opcode/funct3/funct7 from IF/ID register bits directly
3. `regfile` rs1_addr = if_id_instr[19:15], rs2_addr = if_id_instr[24:20]
4. `wb_data` feeds back to ex_stage.wb_data for MEM/WB forwarding
5. `ex_mem_fwd_data` (not raw ex_mem_alu_result) feeds back to ex_stage.ex_mem_alu_result
6. pc_next = ex_redirect_en ? ex_redirect_pc : if_pc_plus4
7. `stall_pc` connects to if_stage.stall
8. `flush_if_id` connects to if_stage.flush AND IF/ID register flush
9. `flush_id_ex` flushes ID/EX only — EX/MEM is NOT flushed on redirect
10. EX/MEM captures reg_write/mem_read/mem_write/wb_sel from EX stage outputs (not ID/EX)
11. `ex_mem_imm_u` is captured from `ex_imm_out` (EX stage passes imm through)
12. dmem word address uses `dmem_addr[DMEM_ADDR_BITS+1:2]` where DMEM_ADDR_BITS=$clog2(MEM_DEPTH)

---

## Common Integration Bugs

- Using MEM_DEPTH=4096 — too small for riscv-tests data section at 0x2000
- Forgetting `ex_mem_fwd_data` mux — LUI/JAL forwarding gives wrong values
- Flushing EX/MEM on redirect — loses branch/jump writeback result
- Using `ex_mem_alu_result` directly instead of `ex_mem_fwd_data` for EX forwarding
- Using `id_ex_imm` instead of `ex_imm_out` for ex_mem_imm_u — loses correct imm after flush
- Forgetting wb_data feedback to EX stage — MEM/WB forwarding breaks
- Taking reg_write/mem_read/mem_write from ID/EX instead of EX stage outputs
- Not clearing tohost in dmem initial block — false FAIL at cycle 0 from preloaded data
- dmem address using [31:2] instead of parameterized [DMEM_ADDR_BITS+1:2]
