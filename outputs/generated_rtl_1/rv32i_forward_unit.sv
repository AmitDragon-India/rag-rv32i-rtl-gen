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
    localparam logic [1:0] FWD_NONE    = 2'b00;  // No forward (use ID/EX value)
    localparam logic [1:0] FWD_MEM_WB  = 2'b01;  // Forward from MEM/WB (wb_data)
    localparam logic [1:0] FWD_EX_MEM  = 2'b10;  // Forward from EX/MEM (ex_mem_alu_result)

    // x0 register (always zero, never written)
    localparam logic [4:0] X0 = 5'b00000;

    always_comb begin
        // Safe defaults
        forward_a = FWD_NONE;
        forward_b = FWD_NONE;
        forward_s = FWD_NONE;

        // =====================================================================
        // forward_a: ALU operand A (rs1)
        // No guard condition — always check for forwarding
        // =====================================================================
        
        // EX/MEM priority: if ex_mem_rd matches rs1 and ex_mem_reg_write is set
        if (ex_mem_reg_write && (ex_mem_rd != X0) && (ex_mem_rd == id_ex_rs1)) begin
            forward_a = FWD_EX_MEM;
        end
        // MEM/WB: only if EX/MEM did not match
        else if (mem_wb_reg_write && (mem_wb_rd != X0) && (mem_wb_rd == id_ex_rs1)) begin
            forward_a = FWD_MEM_WB;
        end

        // =====================================================================
        // forward_b: ALU operand B (rs2)
        // Guard: only forward when id_ex_alu_srcB == 2'b00 (R-type, branch)
        // For stores, alu_srcB=01 (immediate for address), so forward_b must not assert
        // =====================================================================
        
        if (id_ex_alu_srcB == 2'b00) begin
            // EX/MEM priority
            if (ex_mem_reg_write && (ex_mem_rd != X0) && (ex_mem_rd == id_ex_rs2)) begin
                forward_b = FWD_EX_MEM;
            end
            // MEM/WB: only if EX/MEM did not match
            else if (mem_wb_reg_write && (mem_wb_rd != X0) && (mem_wb_rd == id_ex_rs2)) begin
                forward_b = FWD_MEM_WB;
            end
        end

        // =====================================================================
        // forward_s: Store write data (rs2)
        // No guard condition — always check for forwarding
        // Separate path for store_data independent of alu_srcB
        // =====================================================================
        
        // EX/MEM priority
        if (ex_mem_reg_write && (ex_mem_rd != X0) && (ex_mem_rd == id_ex_rs2)) begin
            forward_s = FWD_EX_MEM;
        end
        // MEM/WB: only if EX/MEM did not match
        else if (mem_wb_reg_write && (mem_wb_rd != X0) && (mem_wb_rd == id_ex_rs2)) begin
            forward_s = FWD_MEM_WB;
        end
    end

endmodule
