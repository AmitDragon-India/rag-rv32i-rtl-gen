# RV32I Architecture Specification

---

## RV32I Opcode Map

```systemverilog
localparam logic [6:0] R_TYPE = 7'b0110011;
localparam logic [6:0] I_TYPE = 7'b0010011;
localparam logic [6:0] LOAD   = 7'b0000011;
localparam logic [6:0] STORE  = 7'b0100011;
localparam logic [6:0] BRANCH = 7'b1100011;
localparam logic [6:0] JAL    = 7'b1101111;
localparam logic [6:0] JALR   = 7'b1100111;
localparam logic [6:0] LUI    = 7'b0110111;
localparam logic [6:0] AUIPC  = 7'b0010111;
```

---

## Pipeline Contract

The processor follows a 5-stage in-order pipeline:

```
IF -> ID -> EX -> MEM -> WB
```

---

## Instruction Decode (ID) Stage

### Responsibilities
- Extract instruction fields (opcode, rd, rs1, rs2, funct3, funct7)
- Generate immediate values (imm_i, imm_s, imm_b, imm_u, imm_j)
- Generate high-level control signals
- Pass `rs1_data` and `rs2_data` to EX stage
- Select ALU operand sources (`alu_srcA`, `alu_srcB`)
- Select writeback source (`wb_sel`)

### Does NOT
- Perform ALU operations
- Access memory
- Update PC
- Resolve branches or jumps
- Generate `alu_op` (handled by a separate ALU control block)

### Decode Rules (always_comb)

1. Extract fields first: opcode, rd, funct3, rs1, rs2, funct7
2. Assign safe defaults for every output signal
3. Pass rs1_data and rs2_data through unchanged
4. Decode using `unique case(opcode)`
5. Always include a `default` case that holds safe values

---

## Control Signal Widths

```systemverilog
logic        reg_write;
logic        mem_read;
logic        mem_write;
logic        branch;
logic        jump;
logic        alu_srcA;
logic [1:0]  alu_srcB;
logic [1:0]  wb_sel;
```

---

## ALU Operand Selection

### alu_srcA (1 bit)
- `0` -> `rs1_data`
- `1` -> `pc_in`

### alu_srcB (2 bits)
- `2'b00` -> `rs2_data`
- `2'b01` -> `immediate` (selected per instruction type)
- `2'b10` -> constant `32'd4`

---

## Writeback Selection (wb_sel)

- `2'b00` -> ALU result
- `2'b01` -> Memory read data
- `2'b10` -> PC + 4 (passed through pipeline from IF stage)
- `2'b11` -> Immediate (imm_u passthrough — used for LUI only)

---

## Control Signal Table

| Instruction | reg_write | mem_read | mem_write | branch | jump | alu_srcA | alu_srcB | wb_sel |
|-------------|-----------|----------|-----------|--------|------|----------|----------|--------|
| R-type      | 1         | 0        | 0         | 0      | 0    | 0        | 00       | 00     |
| I-type      | 1         | 0        | 0         | 0      | 0    | 0        | 01       | 00     |
| Load        | 1         | 1        | 0         | 0      | 0    | 0        | 01       | 01     |
| Store       | 0         | 0        | 1         | 0      | 0    | 0        | 01       | 00     |
| Branch      | 0         | 0        | 0         | 1      | 0    | 0        | 00       | 00     |
| JAL         | 1         | 0        | 0         | 0      | 1    | 1        | 01       | 10     |
| JALR        | 1         | 0        | 0         | 0      | 1    | 0        | 01       | 10     |
| LUI         | 1         | 0        | 0         | 0      | 0    | 0        | 01       | 11     |
| AUIPC       | 1         | 0        | 0         | 0      | 0    | 1        | 01       | 00     |

### Notes on specific instructions

**LUI:** Does not use `rs1`. The result is purely `imm_u`.  
`wb_sel = 2'b11` passes `imm_u` directly to writeback, bypassing the ALU result.  
`alu_srcA` and `alu_srcB` are don't-care; set to safe defaults (`0`, `01`).

**AUIPC:** Result is `pc + imm_u`. Sets `alu_srcA = 1` (pc_in) and `alu_srcB = 01` (imm_u).

**JAL:** Writes `pc + 4` to `rd` (`wb_sel = 2'b10`). Target address is computed separately.  
`pc_plus4` is computed in the IF stage and passed through all pipeline registers.
The ALU is NOT used for the writeback value.
`alu_srcA = 1`(pc_in) and `alu_srcB = 2'b01` (imm_j) used for target calculation in EX stage.


**JALR:** Writes `pc + 4` to `rd` (`wb_sel = 2'b10`). Target is `(rs1 + imm_i) & ~1`, computed separately.
`pc_plus4` is computed in the IF stage and passed through all pipeline registers.
The ALU is NOT used for the writeback value.  
`alu_srcA = 0` and `alu_srcB = 2'b01` (imm_i) used for target calculation in EX stage.



**Branch:** Does not write back. `wb_sel` is don't-care; set to `2'b00` (safe default).

---

## Control Signal Rule

Never use `x`, `z`, or don't-care values in generated RTL.

Even when a signal is logically unused for an instruction, assign a deterministic safe value:

```systemverilog
// Safe defaults at top of always_comb
reg_write = 1'b0;
mem_read  = 1'b0;
mem_write = 1'b0;
branch    = 1'b0;
jump      = 1'b0;
alu_srcA  = 1'b0;
alu_srcB  = 2'b00;
wb_sel    = 2'b00;
```

---

## Immediate Selection

| Instruction | Immediate Used |
|-------------|----------------|
| R-type      | 32'd0          |
| I-type      | imm_i          |
| Load        | imm_i          |
| JALR        | imm_i          |
| Store       | imm_s          |
| Branch      | imm_b          |
| LUI         | imm_u          |
| AUIPC       | imm_u          |
| JAL         | imm_j          |

---

## Immediate Generation

Use exact SystemVerilog concatenation syntax:

```systemverilog
logic [31:0] imm_i, imm_s, imm_b, imm_u, imm_j;

assign imm_i = {{20{instr[31]}}, instr[31:20]};
assign imm_s = {{20{instr[31]}}, instr[31:25], instr[11:7]};
assign imm_b = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};
assign imm_u = {instr[31:12], 12'b0};
assign imm_j = {{11{instr[31]}}, instr[31], instr[19:12], instr[20], instr[30:21], 1'b0};
```

Notes:
- `imm_b` and `imm_j` are the most error-prone — always verify bit positions
- All immediates are sign-extended to 32 bits except `imm_u`
- `imm_u` zero-pads the lower 12 bits

---

## Execution (EX) Stage

### Branch and Jump Resolution

Branch comparison and target address generation are separate operations.

#### Branch comparison
- Uses `rs1_data` and `rs2_data`
- Comparison result (`branch_taken`) determines whether branch redirects PC

#### Branch target
```
branch_target = pc_in + imm_b
```

#### Jump targets
```
JAL  target = pc_in + imm_j
JALR target = (rs1_data + imm_i) & ~32'h1   // clear LSB
```

#### Redirect logic
- Jumps always redirect the PC
- Branches redirect only when `branch_taken = 1`
- On redirect: flush IF and ID pipeline registers (insert NOP = `32'h00000013`)

---

## ALU Control Generation

ALU control is a separate block from the ID-stage control decode.

### Inputs
- `opcode` [6:0]
- `funct3` [2:0]
- `funct7` [6:0]

### Output
- `alu_op` [3:0] — passed to EX stage

### alu_op Encoding

```systemverilog
localparam logic [3:0] ALU_ADD  = 4'b0000;
localparam logic [3:0] ALU_SUB  = 4'b0001;
localparam logic [3:0] ALU_AND  = 4'b0010;
localparam logic [3:0] ALU_OR   = 4'b0011;
localparam logic [3:0] ALU_XOR  = 4'b0100;
localparam logic [3:0] ALU_SLT  = 4'b0101;  // signed
localparam logic [3:0] ALU_SLTU = 4'b0110;  // unsigned
localparam logic [3:0] ALU_SLL  = 4'b0111;
localparam logic [3:0] ALU_SRL  = 4'b1000;
localparam logic [3:0] ALU_SRA  = 4'b1001;
```

### Decoding Rules

| Instruction       | opcode  | funct3 | funct7  | alu_op   |
|-------------------|---------|--------|---------|----------|
| ADD               | 0110011 | 000    | 0000000 | ALU_ADD  |
| SUB               | 0110011 | 000    | 0100000 | ALU_SUB  |
| AND               | 0110011 | 111    | 0000000 | ALU_AND  |
| OR                | 0110011 | 110    | 0000000 | ALU_OR   |
| XOR               | 0110011 | 100    | 0000000 | ALU_XOR  |
| SLT               | 0110011 | 010    | 0000000 | ALU_SLT  |
| SLTU              | 0110011 | 011    | 0000000 | ALU_SLTU |
| SLL               | 0110011 | 001    | 0000000 | ALU_SLL  |
| SRL               | 0110011 | 101    | 0000000 | ALU_SRL  |
| SRA               | 0110011 | 101    | 0100000 | ALU_SRA  |
| ADDI              | 0010011 | 000    | -       | ALU_ADD  |
| SLTI              | 0010011 | 010    | -       | ALU_SLT  |
| SLTIU             | 0010011 | 011    | -       | ALU_SLTU |
| ANDI              | 0010011 | 111    | -       | ALU_AND  |
| ORI               | 0010011 | 110    | -       | ALU_OR   |
| XORI              | 0010011 | 100    | -       | ALU_XOR  |
| SLLI              | 0010011 | 001    | 0000000 | ALU_SLL  |
| SRLI              | 0010011 | 101    | 0000000 | ALU_SRL  |
| SRAI              | 0010011 | 101    | 0100000 | ALU_SRA  |
| Load/Store/AUIPC  | varies  | -      | -       | ALU_ADD  |


## ID Stage Interface — Complete

```systemverilog
module rv32i_id_stage (
    input  logic [31:0] instr,
    input  logic [31:0] pc_in,
    input  logic [31:0] pc_plus4_in,
    input  logic [31:0] rs1_data_in,
    input  logic [31:0] rs2_data_in,
    input  logic [3:0]  alu_op_in,       // passed through from rv32i_alu_control

    output logic [6:0]  opcode,
    output logic [2:0]  funct3,
    output logic [6:0]  funct7,
    output logic [4:0]  rs1,
    output logic [4:0]  rs2,
    output logic [4:0]  rd,
    output logic [31:0] imm,

    output logic [31:0] pc_out,
    output logic [31:0] pc_plus4_out,
    output logic [31:0] rs1_data_out,
    output logic [31:0] rs2_data_out,
    output logic [31:0] store_data,      // = rs2_data_in, separate port for MEM write path

    output logic        reg_write,
    output logic        mem_read,
    output logic        mem_write,
    output logic        branch,
    output logic        jump,
    output logic        alu_srcA,
    output logic [1:0]  alu_srcB,
    output logic [1:0]  wb_sel,

    output logic [3:0]  alu_op_out       // pass-through of alu_op_in
);
```

### store_data Separate Output

`store_data = rs2_data_in` — a separate output port carrying the rs2 value independently
for the MEM write data path. Even though it holds the same value as rs2_data_out,
it travels through a separate pipeline register field (id_ex_store_data) so that
forward_s can be applied to it independently of forward_b in the EX stage.
This is required because for store instructions, alu_srcB=01 (imm for address),
so opB carries the immediate — rs2 (write data) must travel separately.

### alu_op Pass-Through

The ID stage does NOT generate alu_op. It is computed by a separate rv32i_alu_control
module and passed into id_stage as alu_op_in, then passed through unchanged as alu_op_out.

---

## Imm_b Encoding Note

Both of the following are equivalent correct encodings for imm_b:
```systemverilog
assign imm_b = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};   // implementation
assign imm_b = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0}; // spec
```
Both produce identical 32-bit results. Either form is acceptable.

---

## LUI Forwarding Warning

For LUI (opcode=0x37), instruction bits [19:15] are part of the U-type immediate,
NOT an rs1 register address. The forwarding unit must not forward based on rs1 for LUI.

More importantly: LUI uses wb_sel=WB_IMM_U, meaning the actual writeback value is imm_u,
NOT the ALU result. When the EX/MEM forwarding mux forwards the "result" of a LUI
instruction, it must forward imm_u (not alu_result).

In rv32i_top, a corrected forwarding mux selects:
```systemverilog
always_comb begin
    case (ex_mem_wb_sel)
        2'b10:   ex_mem_fwd_data = ex_mem_pc_plus4;   // JAL/JALR
        2'b11:   ex_mem_fwd_data = ex_mem_imm_u;      // LUI
        default: ex_mem_fwd_data = ex_mem_alu_result;  // R/I/Load/Store/Branch
    endcase
end
```
The EX stage receives ex_mem_fwd_data (not raw ex_mem_alu_result) as its forwarding input.
