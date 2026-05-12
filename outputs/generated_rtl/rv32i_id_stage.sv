module rv32i_id_stage (
    input  logic [31:0] instr,
    input  logic [31:0] pc_in,
    input  logic [31:0] pc_plus4_in,
    input  logic [31:0] rs1_data_in,
    input  logic [31:0] rs2_data_in,
    input  logic [3:0]  alu_op_in,
    output logic [6:0]  opcode,
    output logic [2:0]  funct3,
    output logic [6:0]  funct7,
    output logic [4:0]  rs1,
    output logic [4:0]  rs2,
    output logic [4:0]  rd,
    output logic [31:0] imm,
    output logic [31:0] pc_out,
    output logic [31:0] pc_plus4_out,
    output logic [31:0] rs1_data_out,
    output logic [31:0] rs2_data_out,
    output logic [31:0] store_data,
    output logic        reg_write,
    output logic        mem_read,
    output logic        mem_write,
    output logic        branch,
    output logic        jump,
    output logic        alu_srcA,
    output logic [1:0]  alu_srcB,
    output logic [1:0]  wb_sel,
    output logic [3:0]  alu_op_out
);

    // Opcode definitions
    localparam logic [6:0] OP_R_TYPE   = 7'b0110011;  // ADD, SUB, AND, OR, XOR, SLL, SRL, SRA
    localparam logic [6:0] OP_I_TYPE   = 7'b0010011;  // ADDI, ANDI, ORI, XORI, SLTI, SLTIU, SLLI, SRLI, SRAI
    localparam logic [6:0] OP_LOAD     = 7'b0000011;  // LW, LH, LB, LHU, LBU
    localparam logic [6:0] OP_STORE    = 7'b0100011;  // SW, SH, SB
    localparam logic [6:0] OP_BRANCH   = 7'b1100011;  // BEQ, BNE, BLT, BGE, BLTU, BGEU
    localparam logic [6:0] OP_JAL      = 7'b1101111;  // JAL
    localparam logic [6:0] OP_JALR     = 7'b1100111;  // JALR
    localparam logic [6:0] OP_LUI      = 7'b0110111;  // LUI
    localparam logic [6:0] OP_AUIPC    = 7'b0010111;  // AUIPC

    // Writeback selection encoding
    localparam logic [1:0] WB_ALU   = 2'b00;
    localparam logic [1:0] WB_MEM   = 2'b01;
    localparam logic [1:0] WB_PC4   = 2'b10;
    localparam logic [1:0] WB_IMM_U = 2'b11;

    // Immediate generation (combinational)
    logic [31:0] imm_i;
    logic [31:0] imm_s;
    logic [31:0] imm_b;
    logic [31:0] imm_u;
    logic [31:0] imm_j;

    // I-type immediate: sign-extend bits [31:20]
    assign imm_i = {{20{instr[31]}}, instr[31:20]};

    // S-type immediate: sign-extend bits [31:25] and [11:7]
    assign imm_s = {{20{instr[31]}}, instr[31:25], instr[11:7]};

    // B-type immediate: sign-extend bits [31], [7], [30:25], [11:8]
    assign imm_b = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};

    // U-type immediate: bits [31:12] shifted left by 12
    assign imm_u = {instr[31:12], 12'b0};

    // J-type immediate: sign-extend bits [31], [19:12], [20], [30:21]
    assign imm_j = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};

    // Instruction field extraction (combinational)
    always_comb begin
        // Safe defaults for all outputs
        opcode      = 7'b0;
        funct3      = 3'b0;
        funct7      = 7'b0;
        rs1         = 5'b0;
        rs2         = 5'b0;
        rd          = 5'b0;
        imm         = 32'b0;
        pc_out      = 32'b0;
        pc_plus4_out = 32'b0;
        rs1_data_out = 32'b0;
        rs2_data_out = 32'b0;
        store_data  = 32'b0;
        reg_write   = 1'b0;
        mem_read    = 1'b0;
        mem_write   = 1'b0;
        branch      = 1'b0;
        jump        = 1'b0;
        alu_srcA    = 1'b0;
        alu_srcB    = 2'b0;
        wb_sel      = 2'b0;
        alu_op_out  = 4'b0;

        // Extract instruction fields
        opcode = instr[6:0];
        rd     = instr[11:7];
        funct3 = instr[14:12];
        rs1    = instr[19:15];
        rs2    = instr[24:20];
        funct7 = instr[31:25];

        // Pass through data and control
        rs1_data_out  = rs1_data_in;
        rs2_data_out  = rs2_data_in;
        pc_out        = pc_in;
        pc_plus4_out  = pc_plus4_in;
        alu_op_out    = alu_op_in;
        store_data    = rs2_data_in;

        // Decode instruction and set control signals
        unique case (opcode)
            OP_R_TYPE: begin
                // R-type: ADD, SUB, AND, OR, XOR, SLL, SRL, SRA
                reg_write = 1'b1;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b00;
                wb_sel    = WB_ALU;
                imm       = 32'b0;
            end

            OP_I_TYPE: begin
                // I-type: ADDI, ANDI, ORI, XORI, SLTI, SLTIU, SLLI, SRLI, SRAI
                reg_write = 1'b1;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b01;
                wb_sel    = WB_ALU;
                imm       = imm_i;
            end

            OP_LOAD: begin
                // Load: LW, LH, LB, LHU, LBU
                reg_write = 1'b1;
                mem_read  = 1'b1;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b01;
                wb_sel    = WB_MEM;
                imm       = imm_i;
            end

            OP_STORE: begin
                // Store: SW, SH, SB
                reg_write = 1'b0;
                mem_read  = 1'b0;
                mem_write = 1'b1;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b01;
                wb_sel    = 2'b00;
                imm       = imm_s;
            end

            OP_BRANCH: begin
                // Branch: BEQ, BNE, BLT, BGE, BLTU, BGEU
                reg_write = 1'b0;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b1;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b00;
                wb_sel    = 2'b00;
                imm       = imm_b;
            end

            OP_JAL: begin
                // JAL
                reg_write = 1'b1;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b1;
                alu_srcA  = 1'b1;
                alu_srcB  = 2'b01;
                wb_sel    = WB_PC4;
                imm       = imm_j;
            end

            OP_JALR: begin
                // JALR
                reg_write = 1'b1;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b1;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b01;
                wb_sel    = WB_PC4;
                imm       = imm_i;
            end

            OP_LUI: begin
                // LUI
                reg_write = 1'b1;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b01;
                wb_sel    = WB_IMM_U;
                imm       = imm_u;
            end

            OP_AUIPC: begin
                // AUIPC
                reg_write = 1'b1;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b1;
                alu_srcB  = 2'b01;
                wb_sel    = WB_ALU;
                imm       = imm_u;
            end

            default: begin
                // Unknown opcode - safe defaults already set
                reg_write = 1'b0;
                mem_read  = 1'b0;
                mem_write = 1'b0;
                branch    = 1'b0;
                jump      = 1'b0;
                alu_srcA  = 1'b0;
                alu_srcB  = 2'b0;
                wb_sel    = 2'b0;
                imm       = 32'b0;
            end
        endcase
    end

endmodule
