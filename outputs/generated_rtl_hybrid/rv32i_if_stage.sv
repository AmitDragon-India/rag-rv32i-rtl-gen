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

    // Internal PC register
    logic [31:0] pc_current;

    // Word address calculation
    localparam int ADDR_WIDTH = $clog2(MEM_DEPTH) + 2;

    // =========================================================================
    // Memory Initialization
    // =========================================================================
    initial begin
        // Initialize memory to zeros
        for (int i = 0; i < MEM_DEPTH; i++) begin
            imem[i] = 32'h00000000;
        end

        // Load from MEM_FILE parameter if provided
        if (MEM_FILE != "") begin
            $readmemh(MEM_FILE, imem);
        end else begin
            // Try to load from +load= plusarg
            string load_file;
            if ($value$plusargs("load=%s", load_file)) begin
                $readmemh(load_file, imem);
            end
        end
    end

    // =========================================================================
    // Sequential Logic: PC Update
    // =========================================================================
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            pc_current <= 32'h00000000;
        end else if (!stall) begin
            pc_current <= pc_next;
        end
        // else: hold pc_current when stall=1
    end

    // =========================================================================
    // Combinational Logic: Instruction Fetch and PC Outputs
    // =========================================================================
    
    // PC output
    assign pc_out = pc_current;

    // PC + 4
    assign pc_plus4 = pc_current + 32'h00000004;

    // Instruction fetch from memory
    // Word address: pc_current[ADDR_WIDTH-1:2]
    logic [31:0] instr_fetched;
    assign instr_fetched = imem[pc_current[ADDR_WIDTH-1:2]];

    // Flush logic: output NOP (32'h00000013) when flush=1
    assign instr = flush ? 32'h00000013 : instr_fetched;

endmodule
