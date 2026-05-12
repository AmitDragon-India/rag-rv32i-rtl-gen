# RV32I Register File Specification

---

## Overview

The register file contains 32 general-purpose 32-bit registers (x0–x31).
- x0 is hardwired to zero — reads always return 0, writes are silently ignored
- Two independent read ports (rs1, rs2)
- One write port (rd)

---

## Interface

```systemverilog
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
```

---

## Read Port Behavior

- Reads must be **combinational (asynchronous)** — output is valid in the same cycle the address is presented
- Reading x0 (addr = 5'b0) must always return 32'b0
- Must implement **write-before-read bypass**: if `reg_write=1` and `rd_addr` matches `rs1_addr` or `rs2_addr`, the read output must return `rd_data` (the new value being written) rather than the stale value from the register array

---

## Write Port Behavior

- Writes are **synchronous** — occur on posedge clk
- Write is gated: only occurs when `reg_write=1` AND `rd_addr != 5'b0`
- Writes to x0 must be suppressed — the guard `rd_addr != 5'b0` is mandatory
- On active-low reset (`rst_n=0`), all registers must be cleared to 0

---

## Write-Before-Read Bypass Requirement

In a 5-stage pipeline, the WB stage writes a register on the rising clock edge while the ID stage reads combinationally in the same cycle. Without bypass, ID would read the stale pre-write value. The bypass must forward `rd_data` directly to the read output whenever `reg_write=1` and the write address matches the read address.

---

## Common Bugs to Avoid

- Making reads synchronous (registered) — causes one-cycle read delay breaking pipeline timing
- Missing write-before-read bypass — causes stale register reads when WB and ID access the same register simultaneously
- Forgetting `rd_addr != 5'b0` guard on write — allows corrupting the hardwired-zero register
- Using `assign` for reads when bypass logic is needed — use `always_comb` instead

---

## ABI Register Names (Reference)

| Register | ABI Name | Role                   |
|----------|----------|------------------------|
| x0       | zero     | Hardwired zero         |
| x1       | ra       | Return address         |
| x2       | sp       | Stack pointer          |
| x3       | gp       | Global pointer         |
| x4       | tp       | Thread pointer         |
| x5–x7    | t0–t2    | Temporaries            |
| x8       | s0/fp    | Saved / frame pointer  |
| x9       | s1       | Saved register         |
| x10–x11  | a0–a1    | Fn args / return vals  |
| x12–x17  | a2–a7    | Function arguments     |
| x18–x27  | s2–s11   | Saved registers        |
| x28–x31  | t3–t6    | Temporaries            |
