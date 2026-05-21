module rv32i_regfile (
    input  logic        clk,
    input  logic        rst_n,
    input  logic [4:0]  rs1_addr,
    input  logic [4:0]  rs2_addr,
    output logic [31:0] rs1_data,
    output logic [31:0] rs2_data,
    input  logic [4:0]  rd_addr,
    input  logic [31:0] rd_data,
    input  logic        reg_write
);

    // 32 x 32-bit register array
    logic [31:0] registers [32];

    // Synchronous write on posedge clk with async active-low reset
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            // Clear all registers on reset
            for (int i = 0; i < 32; i++) begin
                registers[i] <= 32'b0;
            end
        end else if (reg_write && (rd_addr != 5'b0)) begin
            // Write to register file, but never write to x0
            registers[rd_addr] <= rd_data;
        end
    end

    // Combinational read with write-before-read bypass
    always_comb begin
        // Default: read from register array
        logic [31:0] rs1_raw, rs2_raw;
        
        // Read rs1
        if (rs1_addr == 5'b0) begin
            rs1_raw = 32'b0;  // x0 always reads as zero
        end else begin
            rs1_raw = registers[rs1_addr];
        end
        
        // Read rs2
        if (rs2_addr == 5'b0) begin
            rs2_raw = 32'b0;  // x0 always reads as zero
        end else begin
            rs2_raw = registers[rs2_addr];
        end
        
        // Write-before-read bypass: forward rd_data if write matches read address
        if (reg_write && (rd_addr != 5'b0) && (rd_addr == rs1_addr)) begin
            rs1_data = rd_data;
        end else begin
            rs1_data = rs1_raw;
        end
        
        if (reg_write && (rd_addr != 5'b0) && (rd_addr == rs2_addr)) begin
            rs2_data = rd_data;
        end else begin
            rs2_data = rs2_raw;
        end
    end

endmodule
