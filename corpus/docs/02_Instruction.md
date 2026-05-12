# RISC-V RV32I Instruction Set with Encoding (RAG Optimized)

---

## Instruction Format Reference

| Format | opcode   | Notes                        |
|--------|----------|------------------------------|
| R-Type | 0110011  | Register-register arithmetic |
| I-Type | 0010011  | Register-immediate arithmetic|
| Load   | 0000011  | Memory load                  |
| Store  | 0100011  | Memory store                 |
| Branch | 1100011  | Conditional branch           |
| JAL    | 1101111  | Jump and link (J-type)       |
| JALR   | 1100111  | Jump and link register (I-type) |
| LUI    | 0110111  | Load upper immediate (U-type)|
| AUIPC  | 0010111  | Add upper imm to PC (U-type) |

---

## Notes
- XLEN = 32 (RV32I)
- All registers are 32-bit
- Immediate values are sign-extended unless specified
- imm[31:12] is already positioned in bits [31:12] of the instruction for U-type
- For shift instructions, only lower 5 bits of shift amount are used (RV32I)

### Shift Rules
- I-type shifts use imm[4:0] as shift amount (shamt)
- R-type shifts use rs2[4:0] (lower 5 bits only)

### Comparison Operators
- `<s`  : signed comparison
- `<u`  : unsigned comparison
- `>=s` : signed greater-than-or-equal
- `>=u` : unsigned greater-than-or-equal

### Shift Operators
- `<<`  : logical left shift
- `>>u` : logical right shift (zero fill)
- `>>s` : arithmetic right shift (sign extend)

---

# Integer Register-Immediate Instructions (I-Type)
opcode = 7'b0010011

## ADDI
- category: arithmetic
- type: I-type
- opcode: 0010011
- funct3: 000
- format: addi rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] + sext(imm[11:0])

## SLTI
- category: comparison-signed
- type: I-type
- opcode: 0010011
- funct3: 010
- format: slti rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = (x[rs1] <s sext(imm[11:0])) ? 1 : 0

## SLTIU
- category: comparison-unsigned
- type: I-type
- opcode: 0010011
- funct3: 011
- format: sltiu rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = (x[rs1] <u sext(imm[11:0])) ? 1 : 0

## XORI
- category: logical
- type: I-type
- opcode: 0010011
- funct3: 100
- format: xori rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] ^ sext(imm[11:0])

## ORI
- category: logical
- type: I-type
- opcode: 0010011
- funct3: 110
- format: ori rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] | sext(imm[11:0])

## ANDI
- category: logical
- type: I-type
- opcode: 0010011
- funct3: 111
- format: andi rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] & sext(imm[11:0])

---

## Shift Immediate Instructions (I-Type Special)
opcode = 7'b0010011
Note: For shift-immediate, bits [31:25] encode funct7, and bits [24:20] encode shamt (shift amount).

## SLLI
- category: shift
- type: I-type
- opcode: 0010011
- funct3: 001
- funct7: 0000000
- format: slli rd, rs1, shamt[4:0]
- encoding: [31:25]=funct7 | [24:20]=shamt | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] << shamt

## SRLI
- category: shift
- type: I-type
- opcode: 0010011
- funct3: 101
- funct7: 0000000
- format: srli rd, rs1, shamt[4:0]
- encoding: [31:25]=funct7 | [24:20]=shamt | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] >>u shamt

## SRAI
- category: shift
- type: I-type
- opcode: 0010011
- funct3: 101
- funct7: 0100000
- format: srai rd, rs1, shamt[4:0]
- encoding: [31:25]=funct7 | [24:20]=shamt | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] >>s shamt
- note: SRLI and SRAI share funct3=101; distinguished by funct7 bit [30] (0=SRLI, 1=SRAI)

---

# Upper Immediate Instructions (U-Type)
Note: U-type has no rs1, rs2, or funct3. Bits [19:15] and [14:12] are part of the immediate.

## LUI
- category: immediate
- type: U-type
- opcode: 0110111
- format: lui rd, imm[31:12]
- encoding: [31:12]=imm[31:12] | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = {imm[31:12], 12'b0}
- note: Does not use rs1. Lower 12 bits of result are always zero.

## AUIPC
- category: immediate
- type: U-type
- opcode: 0010111
- format: auipc rd, imm[31:12]
- encoding: [31:12]=imm[31:12] | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = pc + {imm[31:12], 12'b0}
- note: Adds upper immediate to current PC. Used for PC-relative addressing.

---

# Register-Register Instructions (R-Type)
opcode = 7'b0110011

## ADD
- category: arithmetic
- type: R-type
- opcode: 0110011
- funct3: 000
- funct7: 0000000
- format: add rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] + x[rs2]

## SUB
- category: arithmetic
- type: R-type
- opcode: 0110011
- funct3: 000
- funct7: 0100000
- format: sub rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] - x[rs2]
- note: ADD and SUB share funct3=000; distinguished by funct7 bit [30] (0=ADD, 1=SUB)

## SLL
- category: shift
- type: R-type
- opcode: 0110011
- funct3: 001
- funct7: 0000000
- format: sll rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] << (x[rs2] & 5'h1F)

## SLT
- category: comparison-signed
- type: R-type
- opcode: 0110011
- funct3: 010
- funct7: 0000000
- format: slt rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = ($signed(x[rs1]) <s $signed(x[rs2])) ? 32'd1 : 32'd0

## SLTU
- category: comparison-unsigned
- type: R-type
- opcode: 0110011
- funct3: 011
- funct7: 0000000
- format: sltu rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = (x[rs1] <u x[rs2]) ? 32'd1 : 32'd0

## XOR
- category: logical
- type: R-type
- opcode: 0110011
- funct3: 100
- funct7: 0000000
- format: xor rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] ^ x[rs2]

## SRL
- category: shift
- type: R-type
- opcode: 0110011
- funct3: 101
- funct7: 0000000
- format: srl rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] >>u (x[rs2] & 5'h1F)

## SRA
- category: shift
- type: R-type
- opcode: 0110011
- funct3: 101
- funct7: 0100000
- format: sra rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] >>s (x[rs2] & 5'h1F)
- note: SRL and SRA share funct3=101; distinguished by funct7 bit [30] (0=SRL, 1=SRA)

## OR
- category: logical
- type: R-type
- opcode: 0110011
- funct3: 110
- funct7: 0000000
- format: or rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] | x[rs2]

## AND
- category: logical
- type: R-type
- opcode: 0110011
- funct3: 111
- funct7: 0000000
- format: and rd, rs1, rs2
- encoding: [31:25]=funct7 | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = x[rs1] & x[rs2]

---

# Load Instructions (I-Type)
opcode = 7'b0000011
Address = x[rs1] + sext(imm[11:0])

## LB
- category: load
- type: I-type
- opcode: 0000011
- funct3: 000
- format: lb rd, imm(rs1)
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = sext(M[x[rs1] + sext(imm)][7:0])

## LH
- category: load
- type: I-type
- opcode: 0000011
- funct3: 001
- format: lh rd, imm(rs1)
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = sext(M[x[rs1] + sext(imm)][15:0])

## LW
- category: load
- type: I-type
- opcode: 0000011
- funct3: 010
- format: lw rd, imm(rs1)
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = M[x[rs1] + sext(imm)][31:0]

## LBU
- category: load
- type: I-type
- opcode: 0000011
- funct3: 100
- format: lbu rd, imm(rs1)
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = zext(M[x[rs1] + sext(imm)][7:0])
- note: Zero-extends byte, does NOT sign-extend

## LHU
- category: load
- type: I-type
- opcode: 0000011
- funct3: 101
- format: lhu rd, imm(rs1)
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- operation: x[rd] = zext(M[x[rs1] + sext(imm)][15:0])
- note: Zero-extends halfword, does NOT sign-extend

---

# Store Instructions (S-Type)
opcode = 7'b0100011
Address = x[rs1] + sext({imm[11:5], imm[4:0]})
Note: rd field does not exist in S-type. Bits [11:7] are imm[4:0].

## SB
- category: store
- type: S-type
- opcode: 0100011
- funct3: 000
- format: sb rs2, imm(rs1)
- encoding: [31:25]=imm[11:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=imm[4:0] | [6:0]=opcode
- imm = {{20{instr[31]}}, instr[31:25], instr[11:7]}
- operation: M[x[rs1] + sext(imm)][7:0] = x[rs2][7:0]

## SH
- category: store
- type: S-type
- opcode: 0100011
- funct3: 001
- format: sh rs2, imm(rs1)
- encoding: [31:25]=imm[11:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=imm[4:0] | [6:0]=opcode
- imm = {{20{instr[31]}}, instr[31:25], instr[11:7]}
- operation: M[x[rs1] + sext(imm)][15:0] = x[rs2][15:0]

## SW
- category: store
- type: S-type
- opcode: 0100011
- funct3: 010
- format: sw rs2, imm(rs1)
- encoding: [31:25]=imm[11:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:7]=imm[4:0] | [6:0]=opcode
- imm = {{20{instr[31]}}, instr[31:25], instr[11:7]}
- operation: M[x[rs1] + sext(imm)][31:0] = x[rs2][31:0]

---

# Branch Instructions (B-Type)
opcode = 7'b1100011
Note: B-type reorders immediate bits. imm[0] is always 0 (2-byte aligned target).
imm = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0}

## BEQ
- category: branch
- type: B-type
- opcode: 1100011
- funct3: 000
- format: beq rs1, rs2, imm
- encoding: [31]=imm[12] | [30:25]=imm[10:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:8]=imm[4:1] | [7]=imm[11] | [6:0]=opcode
- operation: if (x[rs1] == x[rs2]) pc = pc + sext(imm)

## BNE
- category: branch
- type: B-type
- opcode: 1100011
- funct3: 001
- format: bne rs1, rs2, imm
- encoding: [31]=imm[12] | [30:25]=imm[10:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:8]=imm[4:1] | [7]=imm[11] | [6:0]=opcode
- operation: if (x[rs1] != x[rs2]) pc = pc + sext(imm)

## BLT
- category: branch
- type: B-type
- opcode: 1100011
- funct3: 100
- format: blt rs1, rs2, imm
- encoding: [31]=imm[12] | [30:25]=imm[10:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:8]=imm[4:1] | [7]=imm[11] | [6:0]=opcode
- operation: if ($signed(x[rs1]) <s $signed(x[rs2])) pc = pc + sext(imm)

## BGE
- category: branch
- type: B-type
- opcode: 1100011
- funct3: 101
- format: bge rs1, rs2, imm
- encoding: [31]=imm[12] | [30:25]=imm[10:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:8]=imm[4:1] | [7]=imm[11] | [6:0]=opcode
- operation: if ($signed(x[rs1]) >=s $signed(x[rs2])) pc = pc + sext(imm)

## BLTU
- category: branch
- type: B-type
- opcode: 1100011
- funct3: 110
- format: bltu rs1, rs2, imm
- encoding: [31]=imm[12] | [30:25]=imm[10:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:8]=imm[4:1] | [7]=imm[11] | [6:0]=opcode
- operation: if (x[rs1] <u x[rs2]) pc = pc + sext(imm)

## BGEU
- category: branch
- type: B-type
- opcode: 1100011
- funct3: 111
- format: bgeu rs1, rs2, imm
- encoding: [31]=imm[12] | [30:25]=imm[10:5] | [24:20]=rs2 | [19:15]=rs1 | [14:12]=funct3 | [11:8]=imm[4:1] | [7]=imm[11] | [6:0]=opcode
- operation: if (x[rs1] >=u x[rs2]) pc = pc + sext(imm)

---

# Jump Instructions

## JAL
- category: jump
- type: J-type
- opcode: 1101111
- format: jal rd, imm
- encoding: [31]=imm[20] | [30:21]=imm[10:1] | [20]=imm[11] | [19:12]=imm[19:12] | [11:7]=rd | [6:0]=opcode
- imm = {{11{instr[31]}}, instr[31], instr[19:12], instr[20], instr[30:21], 1'b0}
- operation: x[rd] = pc + 4; pc = pc + sext(imm)
- note: J-type has no funct3 field. Bits [14:12] are part of imm[19:12].

## JALR
- category: jump
- type: I-type
- opcode: 1100111
- funct3: 000
- format: jalr rd, rs1, imm[11:0]
- encoding: [31:20]=imm[11:0] | [19:15]=rs1 | [14:12]=funct3 | [11:7]=rd | [6:0]=opcode
- imm = {{20{instr[31]}}, instr[31:20]}
- operation: t = pc + 4; pc = (x[rs1] + sext(imm[11:0])) & ~32'h1; x[rd] = t
- note: LSB of target address is always cleared to ensure alignment.

---

