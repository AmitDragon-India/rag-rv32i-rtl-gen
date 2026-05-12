# Verilog Design Patterns for RV32I RTL Generation

This document captures common RTL coding patterns used in a simple in-order RV32I processor.  
The goal is to provide retrieval-friendly examples of correct, synthesizable hardware structure.

---

## 1. Sequential Logic with Asynchronous Active-Low Reset

Use this pattern for:
- program counter
- pipeline registers
- counters
- state-holding registers

### Basic Register

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        q <= '0;
    else
        q <= d;
end
```

---

### Register with Enable (Stall Support)

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        q <= '0;
    else if (en)
        q <= d;
end
```

Notes:
- `en = 0` -> register holds its current value (stall condition)
- `en = 1` -> register captures new input

---

### Pipeline Register with Stall and Flush

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        out_valid <= 1'b0;
        out_data  <= '0;
    end else if (flush) begin
        out_valid <= 1'b0;
        out_data  <= '0;
    end else if (!stall) begin
        out_valid <= in_valid;
        out_data  <= in_data;
    end
end
```

Notes:
- `flush` inserts a bubble (kills wrong-path instruction)
- `stall` holds the current value unchanged
- Used in IF/ID, ID/EX, EX/MEM, MEM/WB pipeline registers

---

## 2. Combinational Logic with always_comb

Use for:
- decoders
- ALU logic
- immediate generation
- multiplexers
- control signal generation

```systemverilog
always_comb begin
    // DEFAULT VALUES FIRST - prevents unintended latch inference
    out_valid  = 1'b0;
    out_data   = '0;
    alu_result = '0;

    // CONDITIONAL LOGIC
    if (en) begin
        out_valid = in_valid;
        out_data  = in_data;
    end
end
```

Notes:
- Always assign safe defaults at the top of the block
- Use `always_comb`, not manual sensitivity lists like `always @(a or b)`
- Prevents latches caused by incomplete assignments

---

## 3. Combinational Default Assignment Pattern

Whenever a signal is conditionally assigned, always set its default first:

```systemverilog
always_comb begin
    // Defaults
    a = '0;
    b = '0;

    // Conditional overrides
    if (en) begin
        a = x;
        b = y;
    end
end
```

Notes:
- Even if `en` is always 1 in practice, always write the default
- This prevents latches and makes intent explicit to tools

---

## 4. Nonblocking vs Blocking Assignment Rule

```
always_ff  (sequential) -> use <=  (non-blocking)
always_comb (combinational) -> use =  (blocking)
```

Never mix blocking and non-blocking in the same always block.

---

## 5. Mux Pattern

### 2-input Mux

```systemverilog
always_comb begin
    unique case (sel)
        1'b0: y = a;
        1'b1: y = b;
        default: y = '0;
    endcase
end
```

### 4-input Mux

```systemverilog
always_comb begin
    unique case (sel)
        2'b00: y = a;
        2'b01: y = b;
        2'b10: y = c;
        default: y = '0;
    endcase
end
```

Notes:
- Use `unique case` for decoder and mux logic to enable tool warnings on overlap
- Always include a `default` to cover undefined states

---

## 6. FSM Template

```systemverilog
typedef enum logic [1:0] {
    IDLE  = 2'b00,
    READ  = 2'b01,
    EXEC  = 2'b10,
    WRITE = 2'b11
} state_t;

state_t state, next_state;

// State register
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        state <= IDLE;
    else
        state <= next_state;
end

// Next-state logic
always_comb begin
    next_state = state; // default: hold

    unique case (state)
        IDLE:  if (start) next_state = READ;
        READ:  next_state = EXEC;
        EXEC:  next_state = WRITE;
        WRITE: next_state = IDLE;
        default: next_state = IDLE;
    endcase
end
```

---

## 7. Valid/Ready Handshake

```systemverilog
// Receiver is ready if output slot is empty or downstream is also ready
assign in_ready = out_ready || !out_valid;

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        out_valid <= 1'b0;
        out_data  <= '0;
    end else if (in_valid && in_ready) begin
        out_data  <= in_data;
        out_valid <= 1'b1;
    end else if (out_ready) begin
        out_valid <= 1'b0;
    end
end
```

---

## 8. Synchronous Reset Convention (Alternative)

Use asynchronous reset (shown in Section 1) for most pipeline registers.  
Use synchronous reset only when required by the target technology:

```systemverilog
always_ff @(posedge clk) begin
    if (rst)
        q <= '0;
    else
        q <= d;
end
```

Notes:
- Asynchronous active-low reset (`negedge rst_n`) is the preferred convention for RV32I cores
- Synchronous reset may be required for certain FPGA primitives

---

## 9. Program Counter Pattern

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        pc <= 32'h00000000;
    else if (!stall)
        pc <= pc_next;
end

// pc_next selection (combinational)
always_comb begin
    if (jump)
        pc_next = jump_target;
    else if (branch_taken)
        pc_next = branch_target;
    else
        pc_next = pc + 32'd4;
end
```

---

## 10. Anti-Patterns to Avoid

### Manual sensitivity list (BAD)

```systemverilog
// BAD - easy to miss signals
always @(a or b) begin
    y = a & b;
end
```

```systemverilog
// GOOD - automatic sensitivity
always_comb begin
    y = a & b;
end
```

---

### casex with don't-care bits (BAD)

```systemverilog
// BAD - x values can hide bugs
casex (opcode)
    7'b011xxxx: ctrl = 1'b1;
endcase
```

```systemverilog
// GOOD - explicit, safe matching
unique case (opcode)
    7'b0110011: ctrl = 1'b1;
    default:    ctrl = 1'b0;
endcase
```

---

### Blocking assignment in sequential logic (BAD)

```systemverilog
// BAD
always_ff @(posedge clk) begin
    q = d; // blocking in sequential - causes sim/synth mismatch
end
```

```systemverilog
// GOOD
always_ff @(posedge clk) begin
    q <= d;
end
```
