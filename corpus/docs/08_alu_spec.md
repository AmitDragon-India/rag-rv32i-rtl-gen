# RV32I ALU Specification

---

## Overview

The ALU is a purely combinational module. It takes two 32-bit operands and a 4-bit
operation code and produces a 32-bit result. It has no memory access, no state, and
no clock or reset ports.

---

## alu_op Encoding

```systemverilog
localparam logic [3:0] ALU_ADD  = 4'b0000;
localparam logic [3:0] ALU_SUB  = 4'b0001;
localparam logic [3:0] ALU_AND  = 4'b0010;
localparam logic [3:0] ALU_OR   = 4'b0011;
localparam logic [3:0] ALU_XOR  = 4'b0100;
localparam logic [3:0] ALU_SLT  = 4'b0101;  // signed less-than
localparam logic [3:0] ALU_SLTU = 4'b0110;  // unsigned less-than
localparam logic [3:0] ALU_SLL  = 4'b0111;  // logical left shift
localparam logic [3:0] ALU_SRL  = 4'b1000;  // logical right shift
localparam logic [3:0] ALU_SRA  = 4'b1001;  // arithmetic right shift
```

---

## rv32i_alu Interface

```systemverilog
module rv32i_alu (
    input  logic [31:0] operand_a,
    input  logic [31:0] operand_b,
    input  logic [3:0]  alu_op,
    output logic [31:0] alu_result
);
```

## rv32i_alu Behavior

- Use `always_comb` with `unique case (alu_op)`
- Default `alu_result = 32'b0` at top of always_comb
- ADD:  `operand_a + operand_b`
- SUB:  `operand_a - operand_b`
- AND:  `operand_a & operand_b`
- OR:   `operand_a | operand_b`
- XOR:  `operand_a ^ operand_b`
- SLT:  `($signed(operand_a) < $signed(operand_b)) ? 32'd1 : 32'd0`
- SLTU: `(operand_a < operand_b) ? 32'd1 : 32'd0`
- SLL:  `operand_a << operand_b[4:0]`
- SRL:  `operand_a >> operand_b[4:0]`
- SRA:  `$signed(operand_a) >>> operand_b[4:0]`

---

## rv32i_alu_control Interface

```systemverilog
module rv32i_alu_control (
    input  logic [6:0] opcode,
    input  logic [2:0] funct3,
    input  logic [6:0] funct7,
    output logic [3:0] alu_op
);
```

## rv32i_alu_control Behavior

- Default `alu_op = ALU_ADD`
- Use `unique case (opcode)` outer decode
- For R_TYPE: use nested `unique case (funct3)`, use `funct7[5]` to distinguish ADD/SUB and SRL/SRA
- For I_TYPE: use nested `unique case (funct3)`, use `funct7[5]` to distinguish SRLI/SRAI
- LOAD, STORE, JAL, JALR, LUI, AUIPC: `alu_op = ALU_ADD`
- BRANCH: `alu_op = ALU_SUB` (comparison done in EX stage using opA_fwd/opB_fwd directly)

### funct3 encoding for R-type and I-type

```
3'b000 = ADD/SUB/ADDI      (use funct7[5] for ADD vs SUB)
3'b001 = SLL/SLLI
3'b010 = SLT/SLTI
3'b011 = SLTU/SLTIU
3'b100 = XOR/XORI
3'b101 = SRL/SRA/SRLI/SRAI (use funct7[5] for logical vs arithmetic)
3'b110 = OR/ORI
3'b111 = AND/ANDI
```

---

## Common Bugs

- Using `>>` instead of `>>>` for SRA — `>>` is always logical shift in SystemVerilog
- Forgetting `$signed()` cast for SLT and SRA
- Not masking shift amount to 5 bits — always use `operand_b[4:0]` for shifts
- Confusing funct7[5]=0 (ADD, SRL) with funct7[5]=1 (SUB, SRA)
