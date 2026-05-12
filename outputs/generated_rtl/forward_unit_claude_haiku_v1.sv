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
    localparam logic [1:0] FWD_NONE    = 2'b00;
    localparam logic [1:0] FWD_MEM_WB  = 2'b01;
    localparam logic [1:0] FWD_EX_MEM  = 2'b10;

    always_comb begin
        // Default all outputs to no forward
        forward_a = FWD_NONE;
        forward_b = FWD_NONE;
        forward_s = FWD_NONE;

        // forward_a: checks id_ex_rs1 against ex_mem_rd then mem_wb_rd
        // EX/MEM has priority over MEM/WB
        // Never forward when rd == 5'b0
        if (ex_mem_reg_write && (ex_mem_rd != 5'b0) && (id_ex_rs1 == ex_mem_rd)) begin
            forward_a = FWD_EX_MEM;
        end else if (mem_wb_reg_write && (mem_wb_rd != 5'b0) && (id_ex_rs1 == mem_wb_rd)) begin
            forward_a = FWD_MEM_WB;
        end

        // forward_b: checks id_ex_rs2 against ex_mem_rd then mem_wb_rd
        // ONLY when id_ex_alu_srcB == 2'b00 (R-type, branch)
        // EX/MEM has priority over MEM/WB
        // Never forward when rd == 5'b0
        if (id_ex_alu_srcB == 2'b00) begin
            if (ex_mem_reg_write && (ex_mem_rd != 5'b0) && (id_ex_rs2 == ex_mem_rd)) begin
                forward_b = FWD_EX_MEM;
            end else if (mem_wb_reg_write && (mem_wb_rd != 5'b0) && (id_ex_rs2 == mem_wb_rd)) begin
                forward_b = FWD_MEM_WB;
            end
        end

        // forward_s: checks id_ex_rs2 against ex_mem_rd then mem_wb_rd
        // No guard, always applies (for store write data)
        // EX/MEM has priority over MEM/WB
        // Never forward when rd == 5'b0
        if (ex_mem_reg_write && (ex_mem_rd != 5'b0) && (id_ex_rs2 == ex_mem_rd)) begin
            forward_s = FWD_EX_MEM;
        end else if (mem_wb_reg_write && (mem_wb_rd != 5'b0) && (id_ex_rs2 == mem_wb_rd)) begin
            forward_s = FWD_MEM_WB;
        end
    end

endmodule