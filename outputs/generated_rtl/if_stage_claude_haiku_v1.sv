module rv32i_if_stage #(
    parameter int    MEM_DEPTH = 8192,
    parameter string MEM_FILE  = ""
) (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        stall,
    input  logic        flush,
    input  logic [31:0] pc_next,
    output logic [31:0] instr,
    output logic [31:0] pc_out,
    output logic [31:0] pc_plus4
);

    // Instruction memory
    logic [31:0] imem [0:MEM_DEPTH-1];

    // PC register
    logic [31:0] pc;

    // Load instruction memory
    initial begin
        if (MEM_FILE != "") begin
            $readmemh(MEM_FILE, imem);
        end else begin
            string load_file;
            if ($value$plusargs("load=%s", load_file)) begin
                $readmemh(load_file, imem);
            end
        end
    end

    // PC update logic (sequential)
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            pc <= 32'h00000000;
        end else if (!stall) begin
            pc <= pc_next;
        end
    end

    // PC + 4 computation (combinational)
    assign pc_plus4 = pc + 32'd4;

    // PC output (combinational)
    assign pc_out = pc;

    // Instruction fetch from memory (combinational)
    // Word address = pc[clog2(MEM_DEPTH)+1:2]
    assign instr = flush ? 32'h00000013 : imem[pc[$clog2(MEM_DEPTH)+1:2]];

endmodule