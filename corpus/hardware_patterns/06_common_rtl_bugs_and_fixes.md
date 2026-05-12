# Common RTL Bugs and Fixes for RV32I RTL Generation

This document captures common RTL mistakes that frequently appear in HDL design
and LLM-generated Verilog/SystemVerilog. Each entry shows the bug, why it is
wrong, and the correct fix.

---

## 1. Blocking Assignment in Sequential Logic

### Bug
```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        q = '0;   // blocking in sequential — WRONG
    else
        q = d;
end
```

### Why it is wrong
- Blocking assignment (`=`) in `always_ff` causes simulation/synthesis mismatch
- Tools may not infer flip-flops correctly

### Fix
```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        q <= '0;
    else
        q <= d;
end
```

### Rule
- `always_ff` -> use `<=` (non-blocking)
- `always_comb` -> use `=` (blocking)

---

## 2. Missing Default Assignment in Combinational Logic

### Bug
```systemverilog
always_comb begin
    if (sel)
        y = a;
    // y is unassigned when sel = 0 -> infers latch
end
```

### Why it is wrong
- `y` has no value when `sel = 0`
- Synthesis tools infer an unintended latch to hold the value

### Fix
```systemverilog
always_comb begin
    y = '0;     // default first
    if (sel)
        y = a;
end
```

### Rule
Always assign safe defaults to every output at the top of every `always_comb`
block before any conditional logic.

---

## 3. Incomplete Case Statement (Missing Default)

### Bug
```systemverilog
always_comb begin
    case (opcode)
        7'b0110011: reg_write = 1'b1;
        7'b0000011: mem_read  = 1'b1;
        // no default -> latches for all unhandled opcodes
    endcase
end
```

### Fix
```systemverilog
always_comb begin
    // Safe defaults before case
    reg_write = 1'b0;
    mem_read  = 1'b0;
    mem_write = 1'b0;

    case (opcode)
        7'b0110011: reg_write = 1'b1;
        7'b0000011: mem_read  = 1'b1;
        default: ; // defaults already set above — no action needed
    endcase
end
```

---

## 4. Wrong Signed vs Unsigned Comparison

### Bug
```systemverilog
// Treats both operands as unsigned — wrong for SLT, BLT, BGE
result = (a < b) ? 32'd1 : 32'd0;
```

### Fix
```systemverilog
// Signed comparison — required for SLT, SLTI, BLT, BGE
result = ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
```

### Rule
- SLT, SLTI, BLT, BGE require `$signed()` cast on both operands
- SLTU, SLTIU, BLTU, BGEU use plain unsigned comparison — no cast needed

---

## 5. Arithmetic Right Shift Without $signed Cast

### Bug
```systemverilog
// >>> without $signed is still logical right shift in SystemVerilog
result = a >>> b[4:0];
```

### Why it is wrong
- In SystemVerilog, `>>>` only performs arithmetic shift if the left operand
  is declared as `signed` or cast with `$signed()`
- `logic` type is unsigned by default, so `>>>` behaves identically to `>>`

### Fix
```systemverilog
// Must cast to signed for arithmetic right shift (SRA, SRAI)
result = $signed(a) >>> b[4:0];
```

---

## 6. Wrong Shift Amount — Not Masked to 5 Bits

### Bug
```systemverilog
result = a << b;        // full 32-bit shift amount — WRONG for RV32I
result = a >> imm;      // imm may have upper bits set
```

### Fix
```systemverilog
result = a << b[4:0];           // R-type: lower 5 bits of rs2
result = a >> imm[4:0];         // I-type: lower 5 bits of shamt
result = $signed(a) >>> b[4:0]; // SRA: lower 5 bits, with signed cast
```

### Rule
RV32I shift amount is always 5 bits (`[4:0]`). Bits above bit 4 must be ignored.

---

## 7. Wrong Immediate Extraction

### Bug
Common errors: wrong bit positions, missing sign extension, wrong B/J scrambling.

### Fix — Use Exact Concatenations

```systemverilog
// I-type
assign imm_i = {{20{instr[31]}}, instr[31:20]};

// S-type
assign imm_s = {{20{instr[31]}}, instr[31:25], instr[11:7]};

// B-type (most error-prone — bits are scrambled)
assign imm_b = {{19{instr[31]}}, instr[31], instr[7],
                instr[30:25], instr[11:8], 1'b0};

// U-type
assign imm_u = {instr[31:12], 12'b0};

// J-type (most error-prone — bits are scrambled)
assign imm_j = {{11{instr[31]}}, instr[31], instr[19:12],
                instr[20], instr[30:21], 1'b0};
```

### Notes
- B-type and J-type bit positions are scrambled in the encoding — always verify
- B-type and J-type immediates always have LSB = `1'b0` (2-byte aligned)
- U-type lower 12 bits are always zero
- All immediates except U-type are sign-extended from bit 31

---

## 8. Missing x0 Guard in Register File Write

### Bug
```systemverilog
// Writes to x0 are not suppressed — x0 can become non-zero
always_ff @(posedge clk) begin
    if (reg_write)
        regs[rd] <= wb_data;
end
```

### Why it is wrong
- x0 must always read as zero in RV32I
- Without the guard, a write to rd=0 corrupts x0
- Many instructions (like branches and stores) produce rd=0 or have rd=x0 as a target

### Fix
```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 32; i++)
            regs[i] <= 32'b0;
    end else if (reg_write && rd != 5'b0) begin
        regs[rd] <= wb_data;
    end
end

// Read port must also always return 0 for x0
assign rs1_data = (rs1_addr == 5'b0) ? 32'b0 : regs[rs1_addr];
assign rs2_data = (rs2_addr == 5'b0) ? 32'b0 : regs[rs2_addr];
```

---

## 9. Missing Reset in Sequential Logic

### Bug
```systemverilog
always_ff @(posedge clk) begin
    pc <= pc + 32'd4;  // no reset — PC starts at unknown value
end
```

### Fix
```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        pc <= 32'h00000000;
    else
        pc <= pc + 32'd4;
end
```

---

## 10. Misuse of $readmemh

### Bug
```systemverilog
// $readmemh outside initial block — not valid for synthesis or simulation
if (MEMFILE != "") begin
    $readmemh(MEMFILE, mem);
end
```

### Fix
```systemverilog
initial begin
    if (MEMFILE != "") begin
        $readmemh(MEMFILE, mem);
    end
end
```

---

## 11. Unsized Constants

### Bug
```systemverilog
pc <= pc + 4;       // unsized constant — width depends on context
imm = imm + 1;
```

### Fix
```systemverilog
pc <= pc + 32'd4;   // explicitly sized and typed
imm = imm + 32'd1;
```

---

## 12. Manual Sensitivity List

### Bug
```systemverilog
always @(a or b) begin
    y = a & b;
end
```

### Why it is wrong
- If a signal is added to the block but forgotten in the sensitivity list,
  simulation will not update `y` when that signal changes
- Causes simulation/synthesis mismatch

### Fix
```systemverilog
always_comb begin
    y = a & b;
end
```

---

## 13. casex With Don't-Care Bits

### Bug
```systemverilog
casex (opcode)
    7'b011xxxx: ctrl = 1'b1;
    default:    ctrl = 1'b0;
endcase
```

### Why it is wrong
- `casex` treats `x` and `z` as wildcards
- Unknown (`x`) values in simulation can match unintended cases
- Hides decoder bugs during simulation

### Fix
```systemverilog
unique case (opcode)
    7'b0110011: ctrl = 1'b1;
    7'b0010011: ctrl = 1'b1;
    default:    ctrl = 1'b0;
endcase
```

---

## 14. Unnecessary Self-Assignment

### Bug
```systemverilog
always_ff @(posedge clk) begin
    pc <= pc;   // register already holds its value — redundant
end
```

### Fix
Remove the self-assignment entirely. A register holds its value automatically
when not driven.

---

## 15. Word Address vs Byte Address in Memory

### Bug
```systemverilog
// PC is byte-addressed but memory is word-indexed
assign instr = imem[pc];       // reads wrong location if pc = 4, 8, 12...
```

### Fix
```systemverilog
// Shift right by 2 to convert byte address to word index
assign instr = imem[pc[31:2]];
```

### Notes
- RV32I PC increments by 4 (bytes)
- Array memory is word-indexed (index 0 = address 0, index 1 = address 4)
- Always use `addr[31:2]` to index a 32-bit wide memory array

---

## 16. Runtime Hex Loading with $value$plusargs

When simulation needs to load different hex files at runtime (without recompiling),
use `$value$plusargs` inside the initial block alongside the compile-time parameter.

### Pattern

```systemverilog
initial begin
    // Load from compile-time parameter (synthesis flow)
    if (MEM_FILE != "")
        $readmemh(MEM_FILE, imem);
    // Load from runtime plusarg (simulation flow: +load=filename)
    else begin
        string load_file;
        if ($value$plusargs("load=%s", load_file))
            $readmemh(load_file, imem);
    end
end
```

### Usage
```bash
./sim +load=/path/to/test.hex
```

### Notes
- `$value$plusargs` reads command-line plusargs at simulation runtime
- This allows the same compiled binary to run different test programs
- The hex file must use word-addressed format (one 32-bit word per line)
- Use `~` expansion carefully — `$readmemh` does not expand shell `~`; use absolute paths

---

## 17. Flush vs Stall in IF Stage

### Flush
When `flush=1`, the instruction output is forced to NOP regardless of PC:

```systemverilog
assign instr = flush ? 32'h00000013 : imem[pc_reg[$clog2(MEM_DEPTH)+1:2]];
```

- PC still updates normally (advances to redirect_pc)
- Only the instruction register output is replaced with NOP
- Used to squash wrong-path instructions on branch/jump

### Stall
When `stall=1`, the PC register holds its current value:

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        pc_reg <= 32'h0;
    else if (!stall)      // stall=1 holds PC
        pc_reg <= pc_next;
end
```

- Both PC and instruction output are frozen
- Used for load-use hazard to insert a bubble

### Rule
Never simultaneously stall and flush the IF stage — stall holds the instruction,
flush replaces it with NOP. They serve opposite purposes.

---

## 18. Memory Depth and Address Bit Width

When parameterizing instruction memory depth:

```systemverilog
parameter int MEM_DEPTH = 8192;  // number of 32-bit words

// Word address index from byte-addressed PC
assign instr = imem[pc_reg[$clog2(MEM_DEPTH)+1:2]];
// For MEM_DEPTH=8192: $clog2(8192)=13, so bits [14:2] — 13 bits for 8192 words
```

### Rule
- `$clog2(MEM_DEPTH)` gives the number of address bits needed
- PC bit slice: `pc[$clog2(MEM_DEPTH)+1:2]` converts byte address to word index
- Memory array must be large enough to hold both code (starts at 0x0000) and
  data sections (e.g. begin_signature at 0x2000 = word index 0x800)
- For riscv-tests: MEM_DEPTH=8192 covers addresses 0x0000 to 0x7FFF (32KB)

