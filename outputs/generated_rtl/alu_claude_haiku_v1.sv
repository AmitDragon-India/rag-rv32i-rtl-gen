module rv32i_alu (
    input  logic [31:0] operand_a,
    input  logic [31:0] operand_b,
    input  logic [3:0]  alu_op,
    output logic [31:0] alu_result
);

    // alu_op encoding
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
        alu_result = 32'b0;
        
        unique case (alu_op)
            ALU_ADD: alu_result = operand_a + operand_b;
            ALU_SUB: alu_result = operand_a - operand_b;
            ALU_AND: alu_result = operand_a & operand_b;
            ALU_OR:  alu_result = operand_a | operand_b;
            ALU_XOR: alu_result = operand_a ^ operand_b;
            ALU_SLT: alu_result = ($signed(operand_a) < $signed(operand_b)) ? 32'd1 : 32'd0;
            ALU_SLTU: alu_result = (operand_a < operand_b) ? 32'd1 : 32'd0;
            ALU_SLL: alu_result = operand_a << operand_b[4:0];
            ALU_SRL: alu_result = operand_a >> operand_b[4:0];
            ALU_SRA: alu_result = $signed(operand_a) >>> operand_b[4:0];
            default: alu_result = 32'b0;
        endcase
    end

endmodule