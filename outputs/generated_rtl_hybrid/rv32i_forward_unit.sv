module rv32i_forward_unit (
    input  logic [4:0]  id_ex_rs1,
    input  logic [4:0]  id_ex_rs2,
    input  logic [1:0]  id_ex_alu_srcB,
    input  logic        ex_mem_reg_write,
    input  logic [4:0]  ex_mem_rd,
    input  logic        mem_wb_reg_write,
    input  logic [4:0]  mem_wb_rd,
    output logic [1:0]  forward_a,
    output logic [1:0]  forward_b,
    output logic [1:0]  forward_s
);

    // Forward select encoding
    localparam logic [1:0] FWD_NONE    = 2'b00;  // no forward (use ID/EX value)
    localparam logic [1:0] FWD_MEM_WB  = 2'b01;  // forward from MEM/WB (wb_data)
    localparam logic [1:0] FWD_EX_MEM  = 2'b10;  // forward from EX/MEM (ex_mem_alu_result)

    // ALU srcB encoding
    localparam logic [1:0] ALU_SRCB_RS2 = 2'b00;  // R-type, branch — rs2 is ALU operand B

    always_comb begin
        // Default: no forwarding
        forward_a = FWD_NONE;
        forward_b = FWD_NONE;
        forward_s = FWD_NONE;

        // ===== forward_a: ALU operand A (rs1) =====
        // EX/MEM takes priority over MEM/WB
        // Never forward when rd=x0
        if (ex_mem_reg_write && (ex_mem_rd != 5'b00000) && (ex_mem_rd == id_ex_rs1)) begin
            forward_a = FWD_EX_MEM;
        end else if (mem_wb_reg_write && (mem_wb_rd != 5'b00000) && (mem_wb_rd == id_ex_rs1)) begin
            forward_a = FWD_MEM_WB;
        end

        // ===== forward_b: ALU operand B (rs2) =====
        // Only forward when alu_srcB == 2'b00 (R-type, branch)
        // For store instructions, alu_srcB=01 so opB carries immediate (address offset)
        // EX/MEM takes priority over MEM/WB
        // Never forward when rd=x0
        if ((id_ex_alu_srcB == ALU_SRCB_RS2)) begin
            if (ex_mem_reg_write && (ex_mem_rd != 5'b00000) && (ex_mem_rd == id_ex_rs2)) begin
                forward_b = FWD_EX_MEM;
            end else if (mem_wb_reg_write && (mem_wb_rd != 5'b00000) && (mem_wb_rd == id_ex_rs2)) begin
                forward_b = FWD_MEM_WB;
            end
        end

        // ===== forward_s: Store write data (rs2) =====
        // Always checks rs2, independent of alu_srcB
        // store_data carries rs2 separately for MEM write data path
        // EX/MEM takes priority over MEM/WB
        // Never forward when rd=x0
        if (ex_mem_reg_write && (ex_mem_rd != 5'b00000) && (ex_mem_rd == id_ex_rs2)) begin
            forward_s = FWD_EX_MEM;
        end else if (mem_wb_reg_write && (mem_wb_rd != 5'b00000) && (mem_wb_rd == id_ex_rs2)) begin
            forward_s = FWD_MEM_WB;
        end
    end

endmodule
