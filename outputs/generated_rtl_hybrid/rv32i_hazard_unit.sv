module rv32i_hazard_unit (
    /* verilator lint_off UNUSED */
    input  logic [31:0] if_id_instr,
    /* verilator lint_on UNUSEDSIGNAL */

    input  logic [4:0]  id_ex_rd,
    input  logic        id_ex_mem_read,
    input  logic        redirect_en,
    output logic        stall_pc,
    output logic        stall_if_id,
    output logic        flush_if_id,
    output logic        flush_id_ex
);

    // Extract rs1 and rs2 from instruction
    // RV32I format: rs1=[19:15], rs2=[24:20]
    logic [4:0] rs1, rs2;
    
    assign rs1 = if_id_instr[19:15];
    assign rs2 = if_id_instr[24:20];
    
    // Detect load-use hazard
    // Condition: id_ex_mem_read=1 AND id_ex_rd != 0 AND id_ex_rd matches rs1 or rs2
    logic load_use_hazard;
    
    assign load_use_hazard = id_ex_mem_read && 
                             (id_ex_rd != 5'b00000) && 
                             ((id_ex_rd == rs1) || (id_ex_rd == rs2));
    
    // Output logic with priority: load_use_hazard takes priority over redirect_en
    always_comb begin
        // Default all outputs to 0
        stall_pc    = 1'b0;
        stall_if_id = 1'b0;
        flush_if_id = 1'b0;
        flush_id_ex = 1'b0;
        
        if (load_use_hazard) begin
            // Load-use hazard: stall PC and IF/ID, flush ID/EX
            stall_pc    = 1'b1;
            stall_if_id = 1'b1;
            flush_if_id = 1'b0;
            flush_id_ex = 1'b1;
        end
        else if (redirect_en) begin
            // Control hazard (redirect): flush IF/ID and ID/EX, do not stall
            stall_pc    = 1'b0;
            stall_if_id = 1'b0;
            flush_if_id = 1'b1;
            flush_id_ex = 1'b1;
        end
    end

endmodule
