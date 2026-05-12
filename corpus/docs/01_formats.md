# RISC-V RV32I Instruction Formats

## R-Type
- opcode: [6:0]
- rd: [11:7]
- funct3: [14:12]
- rs1: [19:15]
- rs2: [24:20]
- funct7: [31:25]

## I-Type
- opcode: [6:0]
- rd: [11:7]
- funct3: [14:12]
- rs1: [19:15]
- imm[11:0]: [31:20] 

## S-Type
- opcode: [6:0]
- imm[4:0]: [11:7]
- funct3: [14:12]
- rs1: [19:15]
- rs2: [24:20]
- imm[11:5]: [31:25]

## B-Type
- opcode: [6:0]
- imm[11]: [7]
- imm[4:1]: [11:8]
- funct3: [14:12]
- rs1: [19:15]
- rs2: [24:20]
- imm[10:5]: [30:25]
- imm[12]: [31]

## U-Type
- opcode: [6:0]
- rd: [11:7]
- imm[31:12]: [31:12] 

## J-Type
- opcode: [6:0]
- rd: [11:7]
- imm[19:12]: [19:12]
- imm[11]: [20]
- imm[10:1]: [30:21]
- imm[20]: [31]