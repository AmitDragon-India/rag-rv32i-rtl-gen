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

    // Opcode constants
    localparam logic [6:0] JALR_OPCODE = 7'b1100111;

    // Internal signals
    logic [31:0] opA_pre, opB_pre;
    logic [31:0] opA_fwd, opB_fwd;
    logic        branch_condition;
    logic [31:0] jal_target, jalr_target, branch_target;

    // alu_srcA mux
    always_comb begin
        unique case (alu_srcA)
            1'b0:    opA_pre = rs1_data;
            default: opA_pre = pc_in;
        endcase
    end

    // alu_srcB mux
    always_comb begin
        unique case (alu_srcB)
            2'b00:   opB_pre = rs2_data;
            2'b01:   opB_pre = imm;
            2'b10:   opB_pre = 32'd4;
            default: opB_pre = 32'd0;
        endcase
    end

    // forward_a mux
    always_comb begin
        unique case (forward_a)
            2'b10:   opA_fwd = ex_mem_alu_result;
            2'b01:   opA_fwd = wb_data;
            default: opA_fwd = opA_pre;
        endcase
    end

    // forward_b mux
    always_comb begin
        unique case (forward_b)
            2'b10:   opB_fwd = ex_mem_alu_result;
            2'b01:   opB_fwd = wb_data;
            default: opB_fwd = opB_pre;
        endcase
    end

    // forward_s mux
    always_comb begin
        unique case (forward_s)
            2'b10:   store_data_fwd = ex_mem_alu_result;
            2'b01:   store_data_fwd = wb_data;
            default: store_data_fwd = store_data;
        endcase
    end

    // ALU instantiation
    rv32i_alu alu_inst (
        .operand_a(opA_fwd),
        .operand_b(opB_fwd),
        .alu_op(alu_op),
        .alu_result(alu_result)
    );

    // Branch condition evaluation
    always_comb begin
        branch_condition = 1'b0;
        
        if (branch) begin
            unique case (funct3)
                3'b000:  branch_condition = (opA_fwd == opB_fwd);           // BEQ
                3'b001:  branch_condition = (opA_fwd != opB_fwd);           // BNE
                3'b100:  branch_condition = ($signed(opA_fwd) < $signed(opB_fwd)); // BLT
                3'b101:  branch_condition = ($signed(opA_fwd) >= $signed(opB_fwd)); // BGE
                3'b110:  branch_condition = (opA_fwd < opB_fwd);            // BLTU
                3'b111:  branch_condition = (opA_fwd >= opB_fwd);           // BGEU
                default: branch_condition = 1'b0;
            endcase
        end
    end

    // Branch taken
    assign branch_taken = branch & branch_condition;

    // Target calculations
    assign jal_target = pc_in + imm;
    assign jalr_target = (opA_fwd + imm) & ~32'h1;
    assign branch_target = pc_in + imm;

    // Redirect logic (if/else, NOT unique case)
    always_comb begin
        redirect_en = 1'b0;
        redirect_pc = 32'd0;
        
        if (jump && opcode == JALR_OPCODE) begin
            redirect_en = 1'b1;
            redirect_pc = jalr_target;
        end
        else if (jump) begin
            redirect_en = 1'b1;
            redirect_pc = jal_target;
        end
        else if (branch_taken) begin
            redirect_en = 1'b1;
            redirect_pc = branch_target;
        end
    end

    // Pass-through outputs
    assign pc_plus4_out = pc_plus4_in;
    assign imm_out = imm;
    assign reg_write_out = reg_write;
    assign rd_out = rd;
    assign wb_sel_out = wb_sel;
    assign mem_read_out = mem_read;
    assign mem_write_out = mem_write;

endmodule