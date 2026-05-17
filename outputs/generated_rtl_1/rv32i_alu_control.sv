module rv32i_alu_control (
    input  logic [6:0] opcode,
    input  logic [2:0] funct3,

    /* verilator lint_off UNUSED */
    input  logic [6:0] funct7,
    /* verilator lint_on UNUSED */

    output logic [3:0] alu_op
);

    // Opcode encoding (RV32I)
    localparam logic [6:0] OP_R_TYPE    = 7'b0110011;  // ADD, SUB, AND, OR, XOR, SLT, SLTU, SLL, SRL, SRA
    localparam logic [6:0] OP_I_TYPE    = 7'b0010011;  // ADDI, SLTI, SLTIU, ANDI, ORI, XORI, SLLI, SRLI, SRAI
    localparam logic [6:0] OP_LOAD      = 7'b0000011;  // LW, LH, LB, LHU, LBU
    localparam logic [6:0] OP_STORE     = 7'b0100011;  // SW, SH, SB
    localparam logic [6:0] OP_BRANCH    = 7'b1100011;  // BEQ, BNE, BLT, BGE, BLTU, BGEU
    localparam logic [6:0] OP_JAL       = 7'b1101111;  // JAL
    localparam logic [6:0] OP_JALR      = 7'b1100111;  // JALR
    localparam logic [6:0] OP_LUI       = 7'b0110111;  // LUI
    localparam logic [6:0] OP_AUIPC     = 7'b0010111;  // AUIPC

    // funct3 encoding for R-type and I-type
    localparam logic [2:0] F3_ADD_SUB   = 3'b000;
    localparam logic [2:0] F3_SLL       = 3'b001;
    localparam logic [2:0] F3_SLT       = 3'b010;
    localparam logic [2:0] F3_SLTU      = 3'b011;
    localparam logic [2:0] F3_XOR       = 3'b100;
    localparam logic [2:0] F3_SRL_SRA   = 3'b101;
    localparam logic [2:0] F3_OR        = 3'b110;
    localparam logic [2:0] F3_AND       = 3'b111;

    // ALU operation encoding
    localparam logic [3:0] ALU_ADD      = 4'b0000;
    localparam logic [3:0] ALU_SUB      = 4'b0001;
    localparam logic [3:0] ALU_AND      = 4'b0010;
    localparam logic [3:0] ALU_OR       = 4'b0011;
    localparam logic [3:0] ALU_XOR      = 4'b0100;
    localparam logic [3:0] ALU_SLT      = 4'b0101;
    localparam logic [3:0] ALU_SLTU     = 4'b0110;
    localparam logic [3:0] ALU_SLL      = 4'b0111;
    localparam logic [3:0] ALU_SRL      = 4'b1000;
    localparam logic [3:0] ALU_SRA      = 4'b1001;

    always_comb begin
        // Default: ALU_ADD
        alu_op = ALU_ADD;

        unique case (opcode)
            OP_R_TYPE: begin
                // R-type: use funct3 and funct7[5]
                unique case (funct3)
                    F3_ADD_SUB: begin
                        if (funct7[5])
                            alu_op = ALU_SUB;
                        else
                            alu_op = ALU_ADD;
                    end
                    F3_SLL: alu_op = ALU_SLL;
                    F3_SLT: alu_op = ALU_SLT;
                    F3_SLTU: alu_op = ALU_SLTU;
                    F3_XOR: alu_op = ALU_XOR;
                    F3_SRL_SRA: begin
                        if (funct7[5])
                            alu_op = ALU_SRA;
                        else
                            alu_op = ALU_SRL;
                    end
                    F3_OR: alu_op = ALU_OR;
                    F3_AND: alu_op = ALU_AND;
                    default: alu_op = ALU_ADD;
                endcase
            end

            OP_I_TYPE: begin
                // I-type: use funct3 and funct7[5] for shift operations
                unique case (funct3)
                    F3_ADD_SUB: alu_op = ALU_ADD;  // ADDI
                    F3_SLL: alu_op = ALU_SLL;      // SLLI
                    F3_SLT: alu_op = ALU_SLT;      // SLTI
                    F3_SLTU: alu_op = ALU_SLTU;    // SLTIU
                    F3_XOR: alu_op = ALU_XOR;      // XORI
                    F3_SRL_SRA: begin
                        if (funct7[5])
                            alu_op = ALU_SRA;      // SRAI
                        else
                            alu_op = ALU_SRL;      // SRLI
                    end
                    F3_OR: alu_op = ALU_OR;        // ORI
                    F3_AND: alu_op = ALU_AND;      // ANDI
                    default: alu_op = ALU_ADD;
                endcase
            end

            OP_LOAD: alu_op = ALU_ADD;     // Load: address calculation
            OP_STORE: alu_op = ALU_ADD;    // Store: address calculation
            OP_BRANCH: alu_op = ALU_SUB;   // Branch: comparison via subtraction
            OP_JAL: alu_op = ALU_ADD;      // JAL: PC+4 calculation
            OP_JALR: alu_op = ALU_ADD;     // JALR: address calculation
            OP_LUI: alu_op = ALU_ADD;      // LUI: not used by ALU, default to ADD
            OP_AUIPC: alu_op = ALU_ADD;    // AUIPC: address calculation

            default: alu_op = ALU_ADD;
        endcase
    end

endmodule
