module rv32i_mem_stage (
    input  logic [31:0] alu_result,
    input  logic [31:0] store_data,
    input  logic [31:0] pc_plus4_in,
    input  logic [31:0] imm_u_in,
    input  logic        mem_read,
    input  logic        mem_write,
    input  logic [2:0]  funct3,
    input  logic [1:0]  wb_sel,
    input  logic        reg_write,
    input  logic [4:0]  rd_in,
    output logic [31:0] dmem_addr,
    output logic [31:0] dmem_wdata,
    output logic [3:0]  dmem_byte_en,
    output logic        dmem_we,
    output logic        dmem_re,
    input  logic [31:0] dmem_rdata,
    output logic [31:0] pc_plus4_out,
    output logic [31:0] imm_u_out,
    output logic [31:0] mem_rdata_out,
    output logic [31:0] alu_result_out,
    output logic        reg_write_out,
    output logic [4:0]  rd_out,
    output logic [1:0]  wb_sel_out
);

    // Local parameters for funct3 encoding
    localparam logic [2:0] FUNCT3_LB  = 3'b000;
    localparam logic [2:0] FUNCT3_LH  = 3'b001;
    localparam logic [2:0] FUNCT3_LW  = 3'b010;
    localparam logic [2:0] FUNCT3_LBU = 3'b100;
    localparam logic [2:0] FUNCT3_LHU = 3'b101;
    localparam logic [2:0] FUNCT3_SB  = 3'b000;
    localparam logic [2:0] FUNCT3_SH  = 3'b001;
    localparam logic [2:0] FUNCT3_SW  = 3'b010;

    logic [1:0]  byte_off;
    logic [31:0] mem_rdata_processed;

    always_comb begin
        // Safe defaults for all outputs
        dmem_addr      = 32'h0;
        dmem_wdata     = 32'h0;
        dmem_byte_en   = 4'b0000;
        dmem_we        = 1'b0;
        dmem_re        = 1'b0;
        mem_rdata_out  = 32'h0;
        pc_plus4_out   = pc_plus4_in;
        imm_u_out      = imm_u_in;
        alu_result_out = alu_result;
        reg_write_out  = reg_write;
        rd_out         = rd_in;
        wb_sel_out     = wb_sel;

        // Extract byte offset from lower 2 bits of ALU result
        byte_off = alu_result[1:0];

        // Set memory address
        dmem_addr = alu_result;

        // Handle store operations
        if (mem_write) begin
            dmem_we = 1'b1;
            unique case (funct3)
                FUNCT3_SB: begin
                    // Store byte at byte_off position
                    dmem_byte_en = 4'b0001 << byte_off;
                    dmem_wdata   = store_data << (8 * byte_off);
                end
                FUNCT3_SH: begin
                    // Store halfword
                    dmem_byte_en = byte_off[1] ? 4'b1100 : 4'b0011;
                    dmem_wdata   = store_data << (8 * byte_off);
                end
                FUNCT3_SW: begin
                    // Store word (aligned)
                    dmem_byte_en = 4'b1111;
                    dmem_wdata   = store_data;
                end
                default: begin
                    dmem_we      = 1'b0;
                    dmem_byte_en = 4'b0000;
                    dmem_wdata   = 32'h0;
                end
            endcase
        end

        // Handle load operations
        if (mem_read) begin
            dmem_re = 1'b1;
            unique case (funct3)
                FUNCT3_LB: begin
                    // Load byte with sign extension
                    case (byte_off)
                        2'b00: mem_rdata_processed = {{24{dmem_rdata[7]}},  dmem_rdata[7:0]};
                        2'b01: mem_rdata_processed = {{24{dmem_rdata[15]}}, dmem_rdata[15:8]};
                        2'b10: mem_rdata_processed = {{24{dmem_rdata[23]}}, dmem_rdata[23:16]};
                        2'b11: mem_rdata_processed = {{24{dmem_rdata[31]}}, dmem_rdata[31:24]};
                    endcase
                end
                FUNCT3_LH: begin
                    // Load halfword with sign extension
                    case (byte_off[1])
                        1'b0: mem_rdata_processed = {{16{dmem_rdata[15]}}, dmem_rdata[15:0]};
                        1'b1: mem_rdata_processed = {{16{dmem_rdata[31]}}, dmem_rdata[31:16]};
                    endcase
                end
                FUNCT3_LW: begin
                    // Load word (aligned)
                    mem_rdata_processed = dmem_rdata;
                end
                FUNCT3_LBU: begin
                    // Load byte with zero extension
                    case (byte_off)
                        2'b00: mem_rdata_processed = {24'h0, dmem_rdata[7:0]};
                        2'b01: mem_rdata_processed = {24'h0, dmem_rdata[15:8]};
                        2'b10: mem_rdata_processed = {24'h0, dmem_rdata[23:16]};
                        2'b11: mem_rdata_processed = {24'h0, dmem_rdata[31:24]};
                    endcase
                end
                FUNCT3_LHU: begin
                    // Load halfword with zero extension
                    case (byte_off[1])
                        1'b0: mem_rdata_processed = {16'h0, dmem_rdata[15:0]};
                        1'b1: mem_rdata_processed = {16'h0, dmem_rdata[31:16]};
                    endcase
                end
                default: begin
                    mem_rdata_processed = 32'h0;
                end
            endcase
            mem_rdata_out = mem_rdata_processed;
        end
    end

endmodule