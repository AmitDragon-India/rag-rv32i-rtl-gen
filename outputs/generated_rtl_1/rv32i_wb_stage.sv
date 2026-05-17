module rv32i_wb_stage (
    input  logic [31:0] alu_result,
    input  logic [31:0] mem_rdata,
    input  logic [31:0] pc_plus4_in,
    input  logic [31:0] imm_u_in,
    input  logic [1:0]  wb_sel,
    output logic [31:0] wb_data
);

    // Writeback selection encoding
    localparam WB_SEL_ALU   = 2'b00;  // ALU result
    localparam WB_SEL_MEM   = 2'b01;  // Memory read data
    localparam WB_SEL_PC4   = 2'b10;  // PC + 4
    localparam WB_SEL_IMM_U = 2'b11;  // Immediate (LUI)

    always_comb begin
        // Default: ALU result
        wb_data = alu_result;

        unique case (wb_sel)
            WB_SEL_ALU:   wb_data = alu_result;
            WB_SEL_MEM:   wb_data = mem_rdata;
            WB_SEL_PC4:   wb_data = pc_plus4_in;
            WB_SEL_IMM_U: wb_data = imm_u_in;
        endcase
    end

endmodule
