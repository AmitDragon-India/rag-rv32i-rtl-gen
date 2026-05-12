module rv32i_alu_control (
    input  logic [6:0] opcode,
    input  logic [2:0] funct3,
    input  logic [6:0] funct7,
    output logic [3:0] alu_op
);

    // Opcode encoding (RV32I)
    localparam logic [6:0] OP_R_TYPE    = 7'b0110011;
    localparam logic [6:0] OP_I_TYPE    = 7'b0010011;
    localparam logic [6:0] OP_LOAD      = 7'b0000011;
    localparam logic [6:0] OP_STORE     = 7'b0100011;
    localparam logic [6:0] OP_BRANCH    = 7'b1100011;
    localparam logic [6:0] OP_JAL       = 7'b1101111;
    localparam logic [6:0] OP_JALR      = 7'b1100111;
    localparam logic [6:0] OP_LUI       = 7'b0110111;
    localparam logic [6:0] OP_AUIPC     = 7'b0010111;

    // funct3 encoding
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
                        // ADD (funct7[5]=0) or SUB (funct7[5]=1)
                        alu_op = funct7[5] ? ALU_SUB : ALU_ADD;
                    end
                    F3_SLL: begin
                        alu_op = ALU_SLL;
                    end
                    F3_SLT: begin
                        alu_op = ALU_SLT;
                    end
                    F3_SLTU: begin
                        alu_op = ALU_SLTU;
                    end
                    F3_XOR: begin
                        alu_op = ALU_XOR;
                    end
                    F3_SRL_SRA: begin
                        // SRL (funct7[5]=0) or SRA (funct7[5]=1)
                        alu_op = funct7[5] ? ALU_SRA : ALU_SRL;
                    end
                    F3_OR: begin
                        alu_op = ALU_OR;
                    end
                    F3_AND: begin
                        alu_op = ALU_AND;
                    end
                    default: begin
                        alu_op = ALU_ADD;
                    end
                endcase
            end

            OP_I_TYPE: begin
                // I-type: use funct3 and funct7[5] for shift operations
                unique case (funct3)
                    F3_ADD_SUB: begin
                        // ADDI
                        alu_op = ALU_ADD;
                    end
                    F3_SLL: begin
                        // SLLI
                        alu_op = ALU_SLL;
                    end
                    F3_SLT: begin
                        // SLTI
                        alu_op = ALU_SLT;
                    end
                    F3_SLTU: begin
                        // SLTIU
                        alu_op = ALU_SLTU;
                    end
                    F3_XOR: begin
                        // XORI
                        alu_op = ALU_XOR;
                    end
                    F3_SRL_SRA: begin
                        // SRLI (funct7[5]=0) or SRAI (funct7[5]=1)
                        alu_op = funct7[5] ? ALU_SRA : ALU_SRL;
                    end
                    F3_OR: begin
                        // ORI
                        alu_op = ALU_OR;
                    end
                    F3_AND: begin
                        // ANDI
                        alu_op = ALU_AND;
                    end
                    default: begin
                        alu_op = ALU_ADD;
                    end
                endcase
            end

            OP_LOAD: begin
                // All load operations use ALU_ADD for address calculation
                alu_op = ALU_ADD;
            end

            OP_STORE: begin
                // All store operations use ALU_ADD for address calculation
                alu_op = ALU_ADD;
            end

            OP_BRANCH: begin
                // Branch operations use ALU_SUB for comparison
                alu_op = ALU_SUB;
            end

            OP_JAL: begin
                // JAL uses ALU_ADD (for PC+4 calculation in EX stage)
                alu_op = ALU_ADD;
            end

            OP_JALR: begin
                // JALR uses ALU_ADD for address calculation
                alu_op = ALU_ADD;
            end

            OP_LUI: begin
                // LUI uses ALU_ADD
                alu_op = ALU_ADD;
            end

            OP_AUIPC: begin
                // AUIPC uses ALU_ADD for PC + immediate
                alu_op = ALU_ADD;
            end

            default: begin
                // Default to ALU_ADD for any undefined opcodes
                alu_op = ALU_ADD;
            end
        endcase
    end

endmodule