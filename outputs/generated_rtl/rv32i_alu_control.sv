module rv32i_alu_control (
    input  logic [6:0] opcode,
    input  logic [2:0] funct3,

    /* verilator lint_off UNUSED */
    input  logic [6:0] funct7,
    /* verilator lint_on UNUSED */
    output logic [3:0] alu_op
);

    // Opcode encoding (RV32I)
    localparam logic [6:0] OPCODE_R_TYPE    = 7'b0110011;
    localparam logic [6:0] OPCODE_I_TYPE    = 7'b0010011;
    localparam logic [6:0] OPCODE_LOAD      = 7'b0000011;
    localparam logic [6:0] OPCODE_STORE     = 7'b0100011;
    localparam logic [6:0] OPCODE_BRANCH    = 7'b1100011;
    localparam logic [6:0] OPCODE_JAL       = 7'b1101111;
    localparam logic [6:0] OPCODE_JALR      = 7'b1100111;
    localparam logic [6:0] OPCODE_LUI       = 7'b0110111;
    localparam logic [6:0] OPCODE_AUIPC     = 7'b0010111;

    // funct3 encoding
    localparam logic [2:0] FUNCT3_ADD_SUB   = 3'b000;
    localparam logic [2:0] FUNCT3_SLL       = 3'b001;
    localparam logic [2:0] FUNCT3_SLT       = 3'b010;
    localparam logic [2:0] FUNCT3_SLTU      = 3'b011;
    localparam logic [2:0] FUNCT3_XOR       = 3'b100;
    localparam logic [2:0] FUNCT3_SRL_SRA   = 3'b101;
    localparam logic [2:0] FUNCT3_OR        = 3'b110;
    localparam logic [2:0] FUNCT3_AND       = 3'b111;

    // ALU operation encoding
    localparam logic [3:0] ALU_ADD  = 4'b0000;
    localparam logic [3:0] ALU_SUB  = 4'b0001;
    localparam logic [3:0] ALU_AND  = 4'b0010;
    localparam logic [3:0] ALU_OR   = 4'b0011;
    localparam logic [3:0] ALU_XOR  = 4'b0100;
    localparam logic [3:0] ALU_SLT  = 4'b0101;
    localparam logic [3:0] ALU_SLTU = 4'b0110;
    localparam logic [3:0] ALU_SLL  = 4'b0111;
    localparam logic [3:0] ALU_SRL  = 4'b1000;
    localparam logic [3:0] ALU_SRA  = 4'b1001;

    always_comb begin
        // Default: ALU_ADD
        alu_op = ALU_ADD;

        unique case (opcode)
            OPCODE_R_TYPE: begin
                // R-type: use funct3 and funct7[5]
                unique case (funct3)
                    FUNCT3_ADD_SUB: begin
                        if (funct7[5])
                            alu_op = ALU_SUB;
                        else
                            alu_op = ALU_ADD;
                    end
                    FUNCT3_SLL: begin
                        alu_op = ALU_SLL;
                    end
                    FUNCT3_SLT: begin
                        alu_op = ALU_SLT;
                    end
                    FUNCT3_SLTU: begin
                        alu_op = ALU_SLTU;
                    end
                    FUNCT3_XOR: begin
                        alu_op = ALU_XOR;
                    end
                    FUNCT3_SRL_SRA: begin
                        if (funct7[5])
                            alu_op = ALU_SRA;
                        else
                            alu_op = ALU_SRL;
                    end
                    FUNCT3_OR: begin
                        alu_op = ALU_OR;
                    end
                    FUNCT3_AND: begin
                        alu_op = ALU_AND;
                    end
                    default: begin
                        alu_op = ALU_ADD;
                    end
                endcase
            end

            OPCODE_I_TYPE: begin
                // I-type: use funct3 and funct7[5] for shift operations
                unique case (funct3)
                    FUNCT3_ADD_SUB: begin
                        alu_op = ALU_ADD;
                    end
                    FUNCT3_SLL: begin
                        alu_op = ALU_SLL;
                    end
                    FUNCT3_SLT: begin
                        alu_op = ALU_SLT;
                    end
                    FUNCT3_SLTU: begin
                        alu_op = ALU_SLTU;
                    end
                    FUNCT3_XOR: begin
                        alu_op = ALU_XOR;
                    end
                    FUNCT3_SRL_SRA: begin
                        if (funct7[5])
                            alu_op = ALU_SRA;
                        else
                            alu_op = ALU_SRL;
                    end
                    FUNCT3_OR: begin
                        alu_op = ALU_OR;
                    end
                    FUNCT3_AND: begin
                        alu_op = ALU_AND;
                    end
                    default: begin
                        alu_op = ALU_ADD;
                    end
                endcase
            end

            OPCODE_LOAD: begin
                alu_op = ALU_ADD;
            end

            OPCODE_STORE: begin
                alu_op = ALU_ADD;
            end

            OPCODE_BRANCH: begin
                alu_op = ALU_SUB;
            end

            OPCODE_JAL: begin
                alu_op = ALU_ADD;
            end

            OPCODE_JALR: begin
                alu_op = ALU_ADD;
            end

            OPCODE_LUI: begin
                alu_op = ALU_ADD;
            end

            OPCODE_AUIPC: begin
                alu_op = ALU_ADD;
            end

            default: begin
                alu_op = ALU_ADD;
            end
        endcase
    end

endmodule
