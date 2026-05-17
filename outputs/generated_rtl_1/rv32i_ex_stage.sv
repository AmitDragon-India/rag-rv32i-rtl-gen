module rv32i_ex_stage (
    input  logic [31:0] rs1_data,
    input  logic [31:0] rs2_data,
    input  logic [31:0] store_data,
    input  logic [31:0] imm,
    input  logic [31:0] pc_in,
    input  logic [31:0] pc_plus4_in,
    input  logic        alu_srcA,
    input  logic [1:0]  alu_srcB,
    input  logic [3:0]  alu_op,
    input  logic [2:0]  funct3,
    input  logic        branch,
    input  logic        jump,
    input  logic [6:0]  opcode,
    input  logic [1:0]  forward_a,
    input  logic [1:0]  forward_b,
    input  logic [1:0]  forward_s,
    input  logic [31:0] ex_mem_alu_result,
    input  logic [31:0] wb_data,
    input  logic        mem_read,
    input  logic        mem_write,
    input  logic [1:0]  wb_sel,
    input  logic        reg_write,
    input  logic [4:0]  rd,
    output logic [31:0] alu_result,
    output logic [31:0] store_data_fwd,
    output logic [31:0] pc_plus4_out,
    output logic        branch_taken,
    output logic        redirect_en,
    output logic [31:0] redirect_pc,
    output logic        reg_write_out,
    output logic [4:0]  rd_out,
    output logic [1:0]  wb_sel_out,
    output logic        mem_read_out,
    output logic        mem_write_out,
    output logic [31:0] imm_out
);

    // Opcode localparams
    localparam logic [6:0] JALR_OP = 7'b1100111;

    // Internal signals
    logic [31:0] opA_pre;
    logic [31:0] opB_pre;
    logic [31:0] opA_fwd;
    logic [31:0] opB_fwd;
    logic [31:0] store_data_pre;

    // ============================================================================
    // Combinational Logic
    // ============================================================================

    always_comb begin
        // Default assignments for all outputs
        opA_pre = rs1_data;
        opB_pre = rs2_data;
        store_data_pre = store_data;
        opA_fwd = rs1_data;
        opB_fwd = rs2_data;
        store_data_fwd = store_data;
        branch_taken = 1'b0;
        redirect_en = 1'b0;
        redirect_pc = 32'd0;
        pc_plus4_out = pc_plus4_in;
        reg_write_out = reg_write;
        rd_out = rd;
        wb_sel_out = wb_sel;
        mem_read_out = mem_read;
        mem_write_out = mem_write;
        imm_out = imm;

        // ====================================================================
        // ALU Source A Mux
        // ====================================================================
        if (alu_srcA == 1'b1) begin
            opA_pre = pc_in;
        end else begin
            opA_pre = rs1_data;
        end

        // ====================================================================
        // ALU Source B Mux
        // ====================================================================
        unique case (alu_srcB)
            2'b00: opB_pre = rs2_data;
            2'b01: opB_pre = imm;
            2'b10: opB_pre = 32'd4;
            default: opB_pre = rs2_data;
        endcase

        // ====================================================================
        // Forward A Mux (operand A forwarding)
        // ====================================================================
        unique case (forward_a)
            2'b00: opA_fwd = opA_pre;
            2'b01: opA_fwd = wb_data;
            2'b10: opA_fwd = ex_mem_alu_result;
            default: opA_fwd = opA_pre;
        endcase

        // ====================================================================
        // Forward B Mux (operand B forwarding)
        // ====================================================================
        unique case (forward_b)
            2'b00: opB_fwd = opB_pre;
            2'b01: opB_fwd = wb_data;
            2'b10: opB_fwd = ex_mem_alu_result;
            default: opB_fwd = opB_pre;
        endcase

        // ====================================================================
        // Forward S Mux (store data forwarding)
        // ====================================================================
        unique case (forward_s)
            2'b00: store_data_fwd = store_data_pre;
            2'b01: store_data_fwd = wb_data;
            2'b10: store_data_fwd = ex_mem_alu_result;
            default: store_data_fwd = store_data_pre;
        endcase

        // ====================================================================
        // Branch Condition Evaluation
        // ====================================================================
        if (branch) begin
            unique case (funct3)
                3'b000: branch_taken = (opA_fwd == opB_fwd);           // BEQ
                3'b001: branch_taken = (opA_fwd != opB_fwd);           // BNE
                3'b100: branch_taken = ($signed(opA_fwd) < $signed(opB_fwd));  // BLT
                3'b101: branch_taken = ($signed(opA_fwd) >= $signed(opB_fwd)); // BGE
                3'b110: branch_taken = (opA_fwd < opB_fwd);            // BLTU
                3'b111: branch_taken = (opA_fwd >= opB_fwd);           // BGEU
                default: branch_taken = 1'b0;
            endcase
        end

        // ====================================================================
        // Redirect Logic (Branch/Jump Target Calculation)
        // ====================================================================
        if (jump && opcode == JALR_OP) begin
            redirect_en = 1'b1;
            redirect_pc = (opA_fwd + imm) & ~32'd1;
        end else if (jump) begin
            redirect_en = 1'b1;
            redirect_pc = pc_in + imm;
        end else if (branch_taken) begin
            redirect_en = 1'b1;
            redirect_pc = pc_in + imm;
        end
    end

    // ============================================================================
    // ALU Instantiation
    // ============================================================================
    rv32i_alu alu_inst (
        .operand_a(opA_fwd),
        .operand_b(opB_fwd),
        .alu_op(alu_op),
        .alu_result(alu_result)
    );

endmodule
