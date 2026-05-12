# RV32I Hazard Control Specification

---

## Overview

A 5-stage in-order pipeline has two classes of hazards:
- **Control hazards** — caused by branches and jumps redirecting the PC
- **Data hazards** — caused by instructions reading a register before a prior instruction writes it

This pipeline implements:
- **Forwarding unit** — eliminates most RAW hazards without stalling
- **Hazard unit** — handles load-use hazards (1 stall) and control hazards (flush)

Only IF/ID and ID/EX registers are flushed on redirect. EX/MEM and MEM/WB are never flushed.

---

## Control Hazards

When `redirect_en=1` from EX stage:
- Flush IF/ID: insert NOP (32'h00000013)
- Flush ID/EX: zero all control signals
- Do NOT stall PC — allow fetch from redirect_pc

Penalty: 2 cycles per taken branch or jump.

---

## Load-Use Hazard

A load produces its result in MEM stage. The next instruction cannot be forwarded in time — one stall cycle is required.

Detection: `id_ex_mem_read=1` AND `id_ex_rd != 0` AND `id_ex_rd` matches rs1 or rs2 of instruction in IF/ID.

Action:
- Stall PC (hold current PC)
- Stall IF/ID (hold current instruction)
- Flush ID/EX (insert bubble)

---

## Hazard Unit

### Module: rv32i_hazard_unit

### Interface

```systemverilog
module rv32i_hazard_unit (
    input  logic [31:0] if_id_instr,     // raw instruction — rs1=[19:15], rs2=[24:20]
    input  logic [4:0]  id_ex_rd,
    input  logic        id_ex_mem_read,
    input  logic        redirect_en,
    output logic        stall_pc,
    output logic        stall_if_id,
    output logic        flush_if_id,
    output logic        flush_id_ex
);
```

### Behavior

- Extract rs1 and rs2 from `if_id_instr` using `assign` statements outside always_comb
- Compute `load_use_hazard` as a combinational `assign`
- Use `always_comb` with if/else for output assignments
- Priority: load_use_hazard takes priority over redirect_en

### Output Truth Table

| Condition          | stall_pc | stall_if_id | flush_if_id | flush_id_ex |
|--------------------|----------|-------------|-------------|-------------|
| load_use_hazard    | 1        | 1           | 0           | 1           |
| redirect_en        | 0        | 0           | 1           | 1           |
| neither            | 0        | 0           | 0           | 0           |

### Key Rules
- `flush_id_ex=1` on BOTH load-use hazard AND redirect — the ID/EX must always be flushed in both cases
- `flush_if_id=1` only on redirect (not on load-use — IF/ID is stalled, not flushed)
- `stall_pc` and `stall_if_id` only on load-use hazard (not on redirect)
- Never simultaneously stall and flush the same register

---

## Forwarding Unit

### Module: rv32i_forward_unit

### Interface

```systemverilog
module rv32i_forward_unit (
    input  logic [4:0] id_ex_rs1,
    input  logic [4:0] id_ex_rs2,
    input  logic [1:0] id_ex_alu_srcB,    // guard for forward_b
    input  logic       ex_mem_reg_write,
    input  logic [4:0] ex_mem_rd,
    input  logic       mem_wb_reg_write,
    input  logic [4:0] mem_wb_rd,
    output logic [1:0] forward_a,
    output logic [1:0] forward_b,
    output logic [1:0] forward_s
);
```

### Forward Select Encoding

```
2'b00 = no forward (use ID/EX pipeline register value)
2'b01 = forward from MEM/WB (wb_data)
2'b10 = forward from EX/MEM (ex_mem_fwd_data)
```

EX/MEM always takes priority over MEM/WB. Never forward when rd=x0.

### Three Independent Forwarding Paths

| Signal      | Checks  | Guard                    | Purpose                        |
|-------------|---------|--------------------------|--------------------------------|
| forward_a   | rs1     | None                     | ALU operand A                  |
| forward_b   | rs2     | id_ex_alu_srcB == 2'b00  | ALU operand B (R-type only)    |
| forward_s   | rs2     | None                     | Store write data (always rs2)  |

### Why forward_b is Guarded

For store instructions: `alu_srcB=01` so opB carries the immediate (address offset).
Forwarding to opB would corrupt the address. store_data carries rs2 separately via forward_s.
forward_b must only assert when `alu_srcB==00` (R-type, branch — rs2 is ALU operand B).

### EX/MEM Forwarding Value Correction

The value forwarded from EX/MEM must match what will actually be written to rd.
In rv32i_top, a corrected mux selects the right value:

```systemverilog
always_comb begin
    case (ex_mem_wb_sel)
        2'b10:   ex_mem_fwd_data = ex_mem_pc_plus4;   // JAL/JALR writes pc_plus4
        2'b11:   ex_mem_fwd_data = ex_mem_imm_u;      // LUI writes imm_u
        default: ex_mem_fwd_data = ex_mem_alu_result;  // others write alu_result
    endcase
end
```

EX stage receives `ex_mem_fwd_data` (not raw `ex_mem_alu_result`) as its forwarding input.
This is critical — without this fix, forwarding from LUI or JAL/JALR gives wrong values.

---

## Summary Table

| Hazard Type     | Detection                               | PC       | IF/ID  | ID/EX  |
|-----------------|-----------------------------------------|----------|--------|--------|
| Load-use        | EX is load AND rd matches ID rs1/rs2    | Stall    | Stall  | Flush  |
| Branch/Jump     | redirect_en=1                           | Redirect | Flush  | Flush  |
| RAW (non-load)  | Resolved by forwarding                  | Normal   | Normal | Normal |

---

## NOP Encoding

```systemverilog
localparam logic [31:0] NOP = 32'h00000013;  // ADDI x0, x0, 0
```

Never use 32'b0 as NOP — it decodes as a valid instruction (LOAD with opcode=0).
