import re
import time
from pathlib import Path
import os

from retrieve import retrieve_structured

import anthropic
client = anthropic.Anthropic(api_key=os.environ.get("ANTHROPIC_API_KEY"))

OUTPUT_DIR = Path("outputs/generated_rtl")
CTX_DIR = Path("outputs/retrieved_context")

# ─────────────────────────────────────────────
# Context priority order for prompt assembly
# ─────────────────────────────────────────────
def sort_context(docs):
    def priority(d):
        src = d.metadata.get("source", "")
        if "04_architecture.md"            in src: return 0
        if "02_Instruction.md"             in src: return 1
        if "01_formats.md"                 in src: return 2
        if "08_alu_spec.md"            in src: return 3
        if "07_regfile_spec.md"            in src: return 3
        if "03_pipeline_stages.md"         in src: return 3
        if "09_hazard_control.md"          in src: return 3
        if "10_top_integration.md"         in src: return 3
        if "05_verilog_design_patterns.md" in src: return 4
        if "06_common_rtl_bugs_and_fixes"  in src: return 4
        return 5
    return sorted(docs, key=priority)


# ─────────────────────────────────────────────
# Prompt builder
# ─────────────────────────────────────────────
def build_prompt(task: str, retrieved_docs):
    context_blocks = []
    for i, doc in enumerate(retrieved_docs, start=1):
        block = (
            f"[Context {i}]\n"
            f"Source: {doc.metadata.get('source')}\n"
            f"Chunk ID: {doc.metadata.get('chunk_id')}\n"
            f"{doc.page_content}\n"
        )
        context_blocks.append(block)

    context = "\n\n".join(context_blocks)

    prompt = f"""You are an expert RTL generation assistant specializing in RV32I processor design.

Task:
{task}

Retrieved context:
{context}

Instructions:
- Generate synthesizable SystemVerilog RTL only.
- Follow RV32I behavior strictly from the provided context.
- Do not invent unsupported instructions or signals.
- Use non-blocking assignments (<=) in always_ff sequential blocks.
- Use blocking assignments (=) in always_comb combinational blocks.
- Always assign safe default values to all outputs at the top of always_comb.
- Use localparam for all opcode and control signal constants.
- Keep module naming exactly as specified in the task.
- Return only one complete SystemVerilog module.
- Do not include any explanation outside the code.

Return format:
Return only SystemVerilog code starting with 'module' and ending with 'endmodule'.
"""
    return prompt.strip()


# ─────────────────────────────────────────────
# LLM call
# ─────────────────────────────────────────────
def call_llm(prompt: str) -> str:
    message = client.messages.create(
        model="claude-haiku-4-5-20251001",
        max_tokens=8192,
        temperature=0.2,
        messages=[
            {"role": "user", "content": prompt}
        ]
    )
    return message.content[0].text


# ─────────────────────────────────────────────
# Verilog extractor
# ─────────────────────────────────────────────
def extract_verilog(text: str) -> str:
    # Try fenced code block first
    fenced = re.search(r"```(?:systemverilog|verilog)?\s*(.*?)```", text, flags=re.DOTALL | re.IGNORECASE)
    if fenced:
        return fenced.group(1).strip()

    # Fall back to module...endmodule
    module_match = re.search(r"(module\b.*?endmodule)", text, flags=re.DOTALL)
    if module_match:
        return module_match.group(1).strip()

    return text.strip()


# ─────────────────────────────────────────────
# Basic validation
# ─────────────────────────────────────────────
def is_valid_verilog(verilog: str) -> bool:
    has_module    = bool(re.search(r'^\s*module\s+\w+', verilog, re.MULTILINE))
    has_endmodule = "endmodule" in verilog
    has_content   = len(verilog) > 100
    return has_module and has_endmodule and has_content


# ─────────────────────────────────────────────
# Save outputs
# ─────────────────────────────────────────────
def save_outputs(task_label, task_text, prompt, context_docs, raw_output, verilog):
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    CTX_DIR.mkdir(parents=True, exist_ok=True)

    with open(CTX_DIR / f"{task_label}_context.txt", "w", encoding="utf-8") as f:
        f.write(f"TASK LABEL:\n{task_label}\n\n")
        f.write(f"TASK:\n{task_text}\n\n")
        f.write(f"PROMPT:\n{prompt}\n\n")
        f.write("RETRIEVED DOCUMENTS:\n\n")
        for i, doc in enumerate(context_docs, start=1):
            f.write(f"[Context {i}] {doc.metadata}\n")
            f.write(doc.page_content)
            f.write("\n\n" + "=" * 80 + "\n\n")

    with open(OUTPUT_DIR / f"{task_label}.sv", "w", encoding="utf-8") as f:
        f.write(verilog)

    with open(OUTPUT_DIR / f"{task_label}_raw.txt", "w", encoding="utf-8") as f:
        f.write(raw_output)

    print(f"  Saved: outputs/generated_rtl/{task_label}.sv")


# ─────────────────────────────────────────────
# Generate one module with retry
# ─────────────────────────────────────────────
def generate_module(task_label: str, task: str, retrieval_query: str, k_rtl: int = 3, max_retries: int = 3):
    print(f"\n{'='*60}")
    print(f"Generating: {task_label}")
    print(f"Retrieval query: {retrieval_query}")
    print(f"{'='*60}")

    docs = retrieve_structured(retrieval_query, k_rtl=k_rtl)
    docs = sort_context(docs)

    print(f"  Retrieved {len(docs)} context chunks:")
    for d in docs:
        print(f"    - {Path(d.metadata['source']).name} | {d.metadata['chunk_id']}")

    prompt = build_prompt(task, docs)

    for attempt in range(1, max_retries + 1):
        print(f"  Attempt {attempt}/{max_retries}...")
        raw_output = call_llm(prompt)
        verilog = extract_verilog(raw_output)

        if is_valid_verilog(verilog):
            print(f"  Valid RTL generated ({len(verilog)} chars)")
            # Save with attempt number for submission documentation
            save_outputs(f"{task_label}_attempt{attempt}", task, prompt, docs, raw_output, verilog)
            save_outputs(task_label, task, prompt, docs, raw_output, verilog)
            return verilog
        else:
            print(f"  Invalid RTL on attempt {attempt} — retrying...")
            save_outputs(f"{task_label}_attempt{attempt}_failed", task, prompt, docs, raw_output, verilog)
            time.sleep(2)

    print(f"  WARNING: All {max_retries} attempts failed for {task_label}")
    save_outputs(task_label, task, prompt, docs, raw_output, verilog)
    return verilog


# ─────────────────────────────────────────────
# All 7 module tasks
# ─────────────────────────────────────────────
TASKS = {

"id_stage": """
Generate a synthesizable SystemVerilog module for an RV32I Instruction Decode (ID) stage.

Module name: rv32i_id_stage

This module performs:
- Instruction field extraction
- Immediate generation (imm_i, imm_s, imm_b, imm_u, imm_j)
- High-level control signal generation
- Pass-through of register file operand data

Do NOT generate:
- Register file implementation
- Pipeline registers
- Hazard detection or forwarding logic
- Clock or reset ports

Interface:
input  logic [31:0] instr
input  logic [31:0] pc_in
input  logic [31:0] pc_plus4_in
input  logic [31:0] rs1_data_in
input  logic [31:0] rs2_data_in
input  logic [3:0]  alu_op_in
output logic [6:0]  opcode
output logic [2:0]  funct3
output logic [6:0]  funct7
output logic [4:0]  rs1
output logic [4:0]  rs2
output logic [4:0]  rd
output logic [31:0] imm
output logic [31:0] pc_out
output logic [31:0] pc_plus4_out
output logic [31:0] rs1_data_out
output logic [31:0] rs2_data_out
output logic [31:0] store_data
output logic        reg_write
output logic        mem_read
output logic        mem_write
output logic        branch
output logic        jump
output logic        alu_srcA
output logic [1:0]  alu_srcB
output logic [1:0]  wb_sel
output logic [3:0]  alu_op_out

Behavior:
- Use localparam logic [6:0] for all opcode definitions
- Use localparam logic [1:0] for wb_sel encoding: WB_ALU=00, WB_MEM=01, WB_PC4=10, WB_IMM_U=11
- Declare imm_i, imm_s, imm_b, imm_u, imm_j as logic [31:0] with assign statements before always_comb
- Use combinational logic only (always_comb) — no clock or reset ports
- At start of always_comb set safe defaults: all control signals=0, imm=32'b0
- Then extract fields: opcode=instr[6:0], rd=instr[11:7], funct3=instr[14:12], rs1=instr[19:15], rs2=instr[24:20], funct7=instr[31:25]
- Then pass through: rs1_data_out=rs1_data_in, rs2_data_out=rs2_data_in, pc_out=pc_in, pc_plus4_out=pc_plus4_in, alu_op_out=alu_op_in, store_data=rs2_data_in
- Use unique case(opcode) with one separate case branch per opcode
- Take control signal values from the Control Signal Table in the retrieved architecture spec
- Select correct immediate per opcode: R-type=32'd0, I-type=imm_i, Load=imm_i, Store=imm_s, Branch=imm_b, LUI=imm_u, AUIPC=imm_u, JAL=imm_j, JALR=imm_i
- NEVER group multiple opcodes in the same case branch
- NEVER use empty default — always assign all control signals in default case
- NEVER use assign for control signals — only always_comb
- Do not use funct3 or funct7 for immediate selection — only opcode determines immediate type
""",

"alu": """
Generate a synthesizable SystemVerilog module for an RV32I ALU.

Module name: rv32i_alu

Interface:
input  logic [31:0] operand_a
input  logic [31:0] operand_b
input  logic [3:0]  alu_op
output logic [31:0] alu_result

Use localparam for alu_op encoding.

Behavior:
- Pure combinational (always_comb only)
- Default alu_result = 32'b0
- Use unique case(alu_op)
- SLT: use $signed() comparison
- SRA: use $signed() cast before >>>
- Shifts: mask to operand_b[4:0]
""",

"alu_control": """
Generate a synthesizable SystemVerilog module for RV32I ALU control.

Module name: rv32i_alu_control

Interface:
input  logic [6:0] opcode
input  logic [2:0] funct3
input  logic [6:0] funct7
output logic [3:0] alu_op

Use localparam for opcodes and alu_op encoding.
Optionally use localparam for funct3 values to improve readability.

Behavior:
- Pure combinational (always_comb only)
- Default alu_op = ALU_ADD
- Use unique case(opcode) outer decode
- For R_TYPE: nested unique case(funct3), use funct7[5] to distinguish ADD/SUB and SRL/SRA
- For I_TYPE: nested unique case(funct3), use funct7[5] for SRAI vs SRLI
- LOAD, STORE, AUIPC, JAL, JALR, LUI -> ALU_ADD
- BRANCH -> ALU_SUB
- Always include default case
""",

"regfile": """
Generate a synthesizable SystemVerilog module for an RV32I register file.

Module name: rv32i_regfile

Interface:
input  logic        clk
input  logic        rst_n
input  logic [4:0]  rs1_addr
input  logic [4:0]  rs2_addr
output logic [31:0] rs1_data
output logic [31:0] rs2_data
input  logic [4:0]  rd_addr
input  logic [31:0] rd_data
input  logic        reg_write

Behavior:
- 32 x 32-bit registers
- x0 hardwired to zero — reads always return 0, writes suppressed
- Reads must use always_comb (NOT assign) to support write-before-read bypass
- Write-before-read bypass: if reg_write=1 and rd_addr matches rs1_addr or rs2_addr, the read output must return rd_data directly (not stale register value)
- Synchronous write on posedge clk with async active-low rst_n reset
- Write only when reg_write=1 AND rd_addr != 5'b0
- Reset initializes all registers to 0
""",

"if_stage": """
Generate a synthesizable SystemVerilog module for an RV32I Instruction Fetch (IF) stage.

Module name: rv32i_if_stage

Interface:
input  logic        clk
input  logic        rst_n
input  logic        stall
input  logic        flush
input  logic [31:0] pc_next
output logic [31:0] instr
output logic [31:0] pc_out
output logic [31:0] pc_plus4

Parameters:
parameter int    MEM_DEPTH = 8192
parameter string MEM_FILE  = ""

Behavior:
- PC register with async active-low reset, stall enable
- pc_plus4 = pc + 32'd4
- Instruction memory: array of MEM_DEPTH 32-bit words
- Word-addressed using exact index: imem[pc[$clog2(MEM_DEPTH)+1:2]]
- Memory loading in initial block:
    if MEM_FILE != "": $readmemh(MEM_FILE, imem)
    else: use $value$plusargs("load=%s", load_file) to load at runtime
- On flush: output NOP (32'h00000013)
- On stall: hold PC else update to pc_next

Coding constraints:
- Use always_ff for sequential PC update.
- Use continuous assignments for pc_out, pc_plus4, and instr.
- Return only one complete SystemVerilog module.
""",

"ex_stage": """
Generate a synthesizable SystemVerilog module for an RV32I Execute (EX) stage.

Module name: rv32i_ex_stage

Interface:
input  logic [31:0] rs1_data
input  logic [31:0] rs2_data
input  logic [31:0] store_data
input  logic [31:0] imm
input  logic [31:0] pc_in
input  logic [31:0] pc_plus4_in
input  logic        alu_srcA
input  logic [1:0]  alu_srcB
input  logic [3:0]  alu_op
input  logic [2:0]  funct3
input  logic        branch
input  logic        jump
input  logic [6:0]  opcode
input  logic [1:0]  forward_a
input  logic [1:0]  forward_b
input  logic [1:0]  forward_s
input  logic [31:0] ex_mem_alu_result
input  logic [31:0] wb_data
input  logic        mem_read
input  logic        mem_write
input  logic [1:0]  wb_sel
input  logic        reg_write
input  logic [4:0]  rd
output logic [31:0] alu_result
output logic [31:0] store_data_fwd
output logic [31:0] pc_plus4_out
output logic        branch_taken
output logic        redirect_en
output logic [31:0] redirect_pc
output logic        reg_write_out
output logic [4:0]  rd_out
output logic [1:0]  wb_sel_out
output logic        mem_read_out
output logic        mem_write_out
output logic [31:0] imm_out

Behavior:
- alu_srcA mux: 0 -> opA_pre=rs1_data, 1 -> opA_pre=pc_in
- alu_srcB mux: 2'b00 -> opB_pre=rs2_data, 2'b01 -> opB_pre=imm, 2'b10 -> opB_pre=32'd4
- forward_a mux: 2'b10 -> opA_fwd=ex_mem_alu_result, 2'b01 -> opA_fwd=wb_data, default -> opA_fwd=opA_pre
- forward_b mux: 2'b10 -> opB_fwd=ex_mem_alu_result, 2'b01 -> opB_fwd=wb_data, default -> opB_fwd=opB_pre
- forward_s mux: 2'b10 -> store_data_fwd=ex_mem_alu_result, 2'b01 -> store_data_fwd=wb_data, default -> store_data_fwd=store_data
- ALU instantiation:
    rv32i_alu alu_inst (.operand_a(opA_fwd), .operand_b(opB_fwd), .alu_op(alu_op), .alu_result(alu_result));
- Branch comparison on opA_fwd and opB_fwd using funct3: BEQ=000, BNE=001, BLT=100, BGE=101, BLTU=110, BGEU=111
- Only evaluate branch_condition when branch=1, otherwise branch_condition=0
- branch_taken = branch & branch_condition
- jal_target = pc_in + imm
- jalr_target = (opA_fwd + imm) & ~32'h1
- branch_target = pc_in + imm
- Redirect logic using if/else (NOT unique case):
    if (jump && opcode == 7'b1100111): redirect_en=1, redirect_pc=jalr_target
    else if (jump):                    redirect_en=1, redirect_pc=jal_target
    else if (branch_taken):            redirect_en=1, redirect_pc=branch_target
    else:                              redirect_en=0
- pc_plus4_out = pc_plus4_in
- imm_out = imm
- Pass through reg_write, rd, wb_sel, mem_read, mem_write unchanged

Coding constraints:
- Use unique case for alu_srcA, alu_srcB, forward_a, forward_b, forward_s, branch funct3
- Use if/else (NOT unique case) for redirect logic
- All logic in always_comb except ALU instantiation
""",

"mem_stage": """
Generate a synthesizable SystemVerilog module for an RV32I Memory Access (MEM) stage.

Module name: rv32i_mem_stage

Interface:
input  logic [31:0] alu_result, store_data, pc_plus4_in, imm_u_in
input  logic        mem_read, mem_write
input  logic [2:0]  funct3
input  logic [1:0]  wb_sel
input  logic        reg_write
input  logic [4:0]  rd_in
output logic [31:0] dmem_addr, dmem_wdata
output logic [3:0]  dmem_byte_en
output logic        dmem_we, dmem_re
input  logic [31:0] dmem_rdata
output logic [31:0] pc_plus4_out, imm_u_out, mem_rdata_out, alu_result_out
output logic        reg_write_out
output logic [4:0]  rd_out
output logic [1:0]  wb_sel_out

Behavior:
- dmem_addr = alu_result
- Use byte_off = alu_result[1:0] for sub-word byte addressing within a 32-bit aligned word
- On mem_write: generate dmem_byte_en and dmem_wdata based on funct3 and byte_off for SB/SH/SW
- On mem_read: sign/zero extend dmem_rdata based on funct3 and byte_off for LB/LBU/LH/LHU/LW
- Pass through reg_write, rd_in, pc_plus4_in, imm_u_in, alu_result, wb_sel unchanged to outputs

Coding constraints:
- Use always_comb only
- Use unique case for store funct3 and load funct3
- Use nested unique case for LB/LBU byte_off selection
- Use if/else for LH/LHU halfword selection
- Default dmem_byte_en=4'b0000, dmem_wdata=32'b0, mem_rdata_out=32'b0 at top of always_comb
- All outputs must have safe defaults
""",


"hazard_unit": """
Generate a synthesizable SystemVerilog module for an RV32I pipeline hazard unit.

Module name: rv32i_hazard_unit

Interface:
input  logic [31:0] if_id_instr
input  logic [4:0]  id_ex_rd
input  logic        id_ex_mem_read
input  logic        redirect_en
output logic        stall_pc
output logic        stall_if_id
output logic        flush_if_id
output logic        flush_id_ex

Behavior:
- Use assign statements to extract rs1=if_id_instr[19:15], rs2=if_id_instr[24:20]
- Use assign to compute load_use_hazard: id_ex_mem_read=1 AND id_ex_rd!=0 AND (id_ex_rd==rs1 OR id_ex_rd==rs2)
- All outputs default to 0 at top of always_comb
- On load_use_hazard: stall_pc=1, stall_if_id=1, flush_if_id=0, flush_id_ex=1
- On redirect_en (else if): stall_pc=0, stall_if_id=0, flush_if_id=1, flush_id_ex=1
- load_use_hazard takes priority over redirect_en (use if/else if)
- flush_id_ex=1 on BOTH load-use AND redirect — this is critical
- Pure combinational always_comb only
""",

"forward_unit": """
Generate a synthesizable SystemVerilog module for an RV32I pipeline forwarding unit.

Module name: rv32i_forward_unit

Interface:
input  logic [4:0]  id_ex_rs1
input  logic [4:0]  id_ex_rs2
input  logic [1:0]  id_ex_alu_srcB
input  logic        ex_mem_reg_write
input  logic [4:0]  ex_mem_rd
input  logic        mem_wb_reg_write
input  logic [4:0]  mem_wb_rd
output logic [1:0]  forward_a
output logic [1:0]  forward_b
output logic [1:0]  forward_s

Behavior:
- Use localparam for forward select encoding: FWD_NONE=2'b00, FWD_MEM_WB=2'b01, FWD_EX_MEM=2'b10
- All outputs default to FWD_NONE
- EX/MEM has priority over MEM/WB for all three outputs
- Never forward when rd==5'b0
- forward_a: checks id_ex_rs1 against ex_mem_rd then mem_wb_rd — no guard
- forward_b: checks id_ex_rs2 against ex_mem_rd then mem_wb_rd — ONLY when id_ex_alu_srcB==2'b00
- forward_s: checks id_ex_rs2 against ex_mem_rd then mem_wb_rd — no guard, always applies
- forward_b and forward_s check the same register (id_ex_rs2) but forward_b has alu_srcB guard
- Pure combinational always_comb only
""",

"wb_stage": """
Generate a synthesizable SystemVerilog module for an RV32I Writeback (WB) stage.

Module name: rv32i_wb_stage

Interface:
input  logic [31:0] alu_result
input  logic [31:0] mem_rdata
input  logic [31:0] pc_plus4_in
input  logic [31:0] imm_u_in
input  logic [1:0]  wb_sel
output logic [31:0] wb_data

Behavior:
- Implement wb_sel mux: 00-> alu_result, 01 -> mem_rdata, 10 -> pc_plus4_in, 11 -> imm_u_in
- Use unique case for wb_sel
- Pure combinational always_comb only
- Default wb_data = alu_result
""",


"top": """
Generate a synthesizable SystemVerilog module for an RV32I 5-stage pipelined processor.

Module name: rv32i_top

Parameters:
parameter int    MEM_DEPTH = 8192
parameter string MEM_FILE  = ""

Interface:
input  logic clk
input  logic rst_n

============================================================
STEP 1 — DECLARE ALL INTERNAL WIRES (copy exactly)
============================================================

// IF stage outputs
logic [31:0] if_instr, if_pc_out, if_pc_plus4;

// IF/ID pipeline register
logic [31:0] if_id_instr, if_id_pc, if_id_pc_plus4;

// alu_control output
logic [3:0]  alu_op_wire;

// regfile outputs
logic [31:0] rs1_data, rs2_data;

// ID stage outputs
logic [6:0]  id_opcode;
logic [2:0]  id_funct3;
logic [6:0]  id_funct7;
logic [4:0]  id_rs1, id_rs2, id_rd;
logic [31:0] id_imm;
logic [31:0] id_pc_out, id_pc_plus4_out;
logic [31:0] id_rs1_data_out, id_rs2_data_out;
logic [31:0] id_store_data;
logic        id_reg_write, id_mem_read, id_mem_write;
logic        id_branch, id_jump, id_alu_srcA;
logic [1:0]  id_alu_srcB, id_wb_sel;
logic [3:0]  id_alu_op_out;

// ID/EX pipeline register
logic        id_ex_reg_write, id_ex_mem_read, id_ex_mem_write;
logic        id_ex_branch, id_ex_jump, id_ex_alu_srcA;
logic [1:0]  id_ex_alu_srcB, id_ex_wb_sel;
logic [3:0]  id_ex_alu_op;
logic [31:0] id_ex_rs1_data, id_ex_rs2_data, id_ex_store_data;
logic [31:0] id_ex_imm, id_ex_pc, id_ex_pc_plus4;
logic [4:0]  id_ex_rd, id_ex_rs1, id_ex_rs2;
logic [2:0]  id_ex_funct3;
logic [6:0]  id_ex_opcode;

// forwarding unit outputs
logic [1:0]  forward_a, forward_b, forward_s;

// EX stage outputs
logic [31:0] ex_alu_result, ex_store_data_fwd, ex_pc_plus4_out;
logic        ex_branch_taken, ex_redirect_en;
logic [31:0] ex_redirect_pc;
logic        ex_reg_write_out, ex_mem_read_out, ex_mem_write_out;
logic [4:0]  ex_rd_out;
logic [1:0]  ex_wb_sel_out;
logic [31:0] ex_imm_out;

// hazard unit outputs
logic        stall_pc, stall_if_id, flush_if_id, flush_id_ex;

// pc_next wire
logic [31:0] pc_next;

// EX/MEM pipeline register
logic        ex_mem_reg_write, ex_mem_mem_read, ex_mem_mem_write;
logic [1:0]  ex_mem_wb_sel;
logic [31:0] ex_mem_alu_result, ex_mem_store_data;
logic [4:0]  ex_mem_rd;
logic [2:0]  ex_mem_funct3;
logic [31:0] ex_mem_pc_plus4, ex_mem_imm_u;

// EX/MEM corrected forwarding value
logic [31:0] ex_mem_fwd_data;

// data memory wires
logic [31:0] dmem_addr, dmem_wdata, dmem_rdata;
logic [3:0]  dmem_byte_en;
logic        dmem_we, dmem_re;

// MEM stage outputs
logic [31:0] mem_pc_plus4_out, mem_imm_u_out;
logic [31:0] mem_rdata_out, mem_alu_result_out;
logic        mem_reg_write_out;
logic [4:0]  mem_rd_out;
logic [1:0]  mem_wb_sel_out;

// MEM/WB pipeline register
logic        mem_wb_reg_write;
logic [1:0]  mem_wb_wb_sel;
logic [31:0] mem_wb_alu_result, mem_wb_mem_rdata;
logic [4:0]  mem_wb_rd;
logic [31:0] mem_wb_pc_plus4, mem_wb_imm_u;

// WB stage output
logic [31:0] wb_data;

============================================================
STEP 2 — INSTANTIATE SUBMODULES (copy exactly)
============================================================

rv32i_if_stage #(.MEM_DEPTH(MEM_DEPTH), .MEM_FILE(MEM_FILE)) if_stage (
    .clk(clk), .rst_n(rst_n),
    .stall(stall_pc), .flush(flush_if_id),
    .pc_next(pc_next),
    .instr(if_instr), .pc_out(if_pc_out), .pc_plus4(if_pc_plus4)
);

rv32i_alu_control alu_ctrl (
    .opcode(if_id_instr[6:0]),
    .funct3(if_id_instr[14:12]),
    .funct7(if_id_instr[31:25]),
    .alu_op(alu_op_wire)
);

rv32i_regfile regfile (
    .clk(clk), .rst_n(rst_n),
    .rs1_addr(if_id_instr[19:15]),
    .rs2_addr(if_id_instr[24:20]),
    .rs1_data(rs1_data), .rs2_data(rs2_data),
    .rd_addr(mem_wb_rd),
    .rd_data(wb_data),
    .reg_write(mem_wb_reg_write)
);

rv32i_id_stage id_stage (
    .instr(if_id_instr),
    .pc_in(if_id_pc), .pc_plus4_in(if_id_pc_plus4),
    .rs1_data_in(rs1_data), .rs2_data_in(rs2_data),
    .alu_op_in(alu_op_wire),
    .opcode(id_opcode), .funct3(id_funct3), .funct7(id_funct7),
    .rs1(id_rs1), .rs2(id_rs2), .rd(id_rd),
    .imm(id_imm),
    .pc_out(id_pc_out), .pc_plus4_out(id_pc_plus4_out),
    .rs1_data_out(id_rs1_data_out), .rs2_data_out(id_rs2_data_out),
    .store_data(id_store_data),
    .reg_write(id_reg_write), .mem_read(id_mem_read), .mem_write(id_mem_write),
    .branch(id_branch), .jump(id_jump),
    .alu_srcA(id_alu_srcA), .alu_srcB(id_alu_srcB),
    .wb_sel(id_wb_sel), .alu_op_out(id_alu_op_out)
);

rv32i_forward_unit fwd_unit (
    .id_ex_rs1(id_ex_rs1), .id_ex_rs2(id_ex_rs2),
    .id_ex_alu_srcB(id_ex_alu_srcB),
    .ex_mem_reg_write(ex_mem_reg_write), .ex_mem_rd(ex_mem_rd),
    .mem_wb_reg_write(mem_wb_reg_write), .mem_wb_rd(mem_wb_rd),
    .forward_a(forward_a), .forward_b(forward_b), .forward_s(forward_s)
);

// EX/MEM forwarding correction mux — must be before EX stage
always_comb begin
    case (ex_mem_wb_sel)
        2'b10:   ex_mem_fwd_data = ex_mem_pc_plus4;
        2'b11:   ex_mem_fwd_data = ex_mem_imm_u;
        default: ex_mem_fwd_data = ex_mem_alu_result;
    endcase
end

rv32i_ex_stage ex_stage (
    .rs1_data(id_ex_rs1_data), .rs2_data(id_ex_rs2_data),
    .store_data(id_ex_store_data),
    .imm(id_ex_imm), .pc_in(id_ex_pc), .pc_plus4_in(id_ex_pc_plus4),
    .alu_srcA(id_ex_alu_srcA), .alu_srcB(id_ex_alu_srcB),
    .alu_op(id_ex_alu_op), .funct3(id_ex_funct3),
    .branch(id_ex_branch), .jump(id_ex_jump), .opcode(id_ex_opcode),
    .forward_a(forward_a), .forward_b(forward_b), .forward_s(forward_s),
    .ex_mem_alu_result(ex_mem_fwd_data),
    .wb_data(wb_data),
    .mem_read(id_ex_mem_read), .mem_write(id_ex_mem_write),
    .wb_sel(id_ex_wb_sel), .reg_write(id_ex_reg_write), .rd(id_ex_rd),
    .alu_result(ex_alu_result), .store_data_fwd(ex_store_data_fwd),
    .pc_plus4_out(ex_pc_plus4_out),
    .branch_taken(ex_branch_taken), .redirect_en(ex_redirect_en),
    .redirect_pc(ex_redirect_pc),
    .reg_write_out(ex_reg_write_out), .rd_out(ex_rd_out),
    .wb_sel_out(ex_wb_sel_out),
    .mem_read_out(ex_mem_read_out), .mem_write_out(ex_mem_write_out),
    .imm_out(ex_imm_out)
);

rv32i_hazard_unit hazard_unit (
    .if_id_instr(if_id_instr),
    .id_ex_rd(id_ex_rd), .id_ex_mem_read(id_ex_mem_read),
    .redirect_en(ex_redirect_en),
    .stall_pc(stall_pc), .stall_if_id(stall_if_id),
    .flush_if_id(flush_if_id), .flush_id_ex(flush_id_ex)
);

rv32i_mem_stage mem_stage (
    .alu_result(ex_mem_alu_result),
    .store_data(ex_mem_store_data),
    .pc_plus4_in(ex_mem_pc_plus4), .imm_u_in(ex_mem_imm_u),
    .mem_read(ex_mem_mem_read), .mem_write(ex_mem_mem_write),
    .funct3(ex_mem_funct3), .wb_sel(ex_mem_wb_sel),
    .reg_write(ex_mem_reg_write), .rd_in(ex_mem_rd),
    .dmem_addr(dmem_addr), .dmem_wdata(dmem_wdata),
    .dmem_byte_en(dmem_byte_en), .dmem_we(dmem_we), .dmem_re(dmem_re),
    .dmem_rdata(dmem_rdata),
    .pc_plus4_out(mem_pc_plus4_out), .imm_u_out(mem_imm_u_out),
    .mem_rdata_out(mem_rdata_out), .alu_result_out(mem_alu_result_out),
    .reg_write_out(mem_reg_write_out), .rd_out(mem_rd_out),
    .wb_sel_out(mem_wb_sel_out)
);

rv32i_wb_stage wb_stage (
    .alu_result(mem_wb_alu_result), .mem_rdata(mem_wb_mem_rdata),
    .pc_plus4_in(mem_wb_pc_plus4), .imm_u_in(mem_wb_imm_u),
    .wb_sel(mem_wb_wb_sel), .wb_data(wb_data)
);

============================================================
STEP 3 — DATA MEMORY
============================================================

localparam int DMEM_ADDR_BITS = $clog2(MEM_DEPTH);

logic [31:0] dmem [0:MEM_DEPTH-1];

initial begin
    string load_file;
    if (MEM_FILE != "")
        $readmemh(MEM_FILE, dmem);
    else if ($value$plusargs("load=%s", load_file))
        $readmemh(load_file, dmem);
    dmem[32'h1000 >> 2] = 32'b0;
end

always_ff @(posedge clk) begin
    if (dmem_we) begin
        if (dmem_byte_en[0]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][7:0]   <= dmem_wdata[7:0];
        if (dmem_byte_en[1]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][15:8]  <= dmem_wdata[15:8];
        if (dmem_byte_en[2]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][23:16] <= dmem_wdata[23:16];
        if (dmem_byte_en[3]) dmem[dmem_addr[DMEM_ADDR_BITS+1:2]][31:24] <= dmem_wdata[31:24];
    end
end

assign dmem_rdata = dmem[dmem_addr[DMEM_ADDR_BITS+1:2]];

============================================================
STEP 4 — PC NEXT MUX
============================================================

always_comb begin
    if (ex_redirect_en) pc_next = ex_redirect_pc;
    else                pc_next = if_pc_plus4;
end

============================================================
STEP 5 — IF/ID PIPELINE REGISTER
============================================================

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n || flush_if_id) begin
        if_id_instr <= 32'h00000013; if_id_pc <= 32'b0; if_id_pc_plus4 <= 32'b0;
    end else if (!stall_if_id) begin
        if_id_instr <= if_instr; if_id_pc <= if_pc_out; if_id_pc_plus4 <= if_pc_plus4;
    end
end

============================================================
STEP 6 — ID/EX PIPELINE REGISTER
============================================================

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n || flush_id_ex) begin
        id_ex_reg_write <= 1'b0; id_ex_mem_read  <= 1'b0; id_ex_mem_write <= 1'b0;
        id_ex_branch    <= 1'b0; id_ex_jump       <= 1'b0; id_ex_alu_srcA  <= 1'b0;
        id_ex_alu_srcB  <= 2'b0; id_ex_wb_sel     <= 2'b0; id_ex_alu_op    <= 4'b0;
        id_ex_rs1_data  <= 32'b0; id_ex_rs2_data  <= 32'b0; id_ex_store_data <= 32'b0;
        id_ex_imm       <= 32'b0; id_ex_pc        <= 32'b0; id_ex_pc_plus4  <= 32'b0;
        id_ex_rd        <= 5'b0;  id_ex_rs1       <= 5'b0;  id_ex_rs2       <= 5'b0;
        id_ex_funct3    <= 3'b0;  id_ex_opcode    <= 7'b0;
    end else begin
        id_ex_reg_write  <= id_reg_write;   id_ex_mem_read   <= id_mem_read;
        id_ex_mem_write  <= id_mem_write;   id_ex_branch     <= id_branch;
        id_ex_jump       <= id_jump;        id_ex_alu_srcA   <= id_alu_srcA;
        id_ex_alu_srcB   <= id_alu_srcB;    id_ex_wb_sel     <= id_wb_sel;
        id_ex_alu_op     <= id_alu_op_out;  id_ex_rs1_data   <= id_rs1_data_out;
        id_ex_rs2_data   <= id_rs2_data_out; id_ex_store_data <= id_store_data;
        id_ex_imm        <= id_imm;         id_ex_pc         <= id_pc_out;
        id_ex_pc_plus4   <= id_pc_plus4_out; id_ex_rd        <= id_rd;
        id_ex_rs1        <= id_rs1;         id_ex_rs2        <= id_rs2;
        id_ex_funct3     <= id_funct3;      id_ex_opcode     <= id_opcode;
    end
end

============================================================
STEP 7 — EX/MEM PIPELINE REGISTER (reset only — never flush)
============================================================

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        ex_mem_reg_write <= 1'b0; ex_mem_mem_read  <= 1'b0; ex_mem_mem_write <= 1'b0;
        ex_mem_wb_sel    <= 2'b0; ex_mem_alu_result <= 32'b0; ex_mem_store_data <= 32'b0;
        ex_mem_rd        <= 5'b0; ex_mem_funct3    <= 3'b0;
        ex_mem_pc_plus4  <= 32'b0; ex_mem_imm_u    <= 32'b0;
    end else begin
        ex_mem_reg_write  <= ex_reg_write_out;
        ex_mem_mem_read   <= ex_mem_read_out;
        ex_mem_mem_write  <= ex_mem_write_out;
        ex_mem_wb_sel     <= ex_wb_sel_out;
        ex_mem_alu_result <= ex_alu_result;
        ex_mem_store_data <= ex_store_data_fwd;
        ex_mem_rd         <= ex_rd_out;
        ex_mem_funct3     <= id_ex_funct3;
        ex_mem_pc_plus4   <= ex_pc_plus4_out;
        ex_mem_imm_u      <= ex_imm_out;
    end
end

============================================================
STEP 8 — MEM/WB PIPELINE REGISTER (reset only — never flush)
============================================================

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        mem_wb_reg_write <= 1'b0; mem_wb_wb_sel    <= 2'b0;
        mem_wb_alu_result <= 32'b0; mem_wb_mem_rdata <= 32'b0;
        mem_wb_rd        <= 5'b0; mem_wb_pc_plus4  <= 32'b0;
        mem_wb_imm_u     <= 32'b0;
    end else begin
        mem_wb_reg_write  <= mem_reg_write_out;
        mem_wb_wb_sel     <= mem_wb_sel_out;
        mem_wb_alu_result <= mem_alu_result_out;
        mem_wb_mem_rdata  <= mem_rdata_out;
        mem_wb_rd         <= mem_rd_out;
        mem_wb_pc_plus4   <= mem_pc_plus4_out;
        mem_wb_imm_u      <= mem_imm_u_out;
    end
end

Coding constraints:
- Copy wire declarations and instantiations EXACTLY as given above
- Do NOT instantiate rv32i_alu — it is inside rv32i_ex_stage
- Do NOT add external ports — only clk and rst_n
- EX/MEM and MEM/WB reset on rst_n ONLY — never flush
- ex_mem_imm_u captures ex_imm_out (NOT id_ex_imm)
- ex_mem_fwd_data mux must use wb_sel encoding: 2'b10=JAL/JALR, 2'b11=LUI
- dmem uses DMEM_ADDR_BITS localparam for address indexing
""",

}


RETRIEVAL_QUERIES = {
    "id_stage":    "RV32I instruction decode opcode control signal table immediate generation imm_i imm_s imm_b imm_u imm_j unique case always_comb reg_write mem_read mem_write alu_srcA alu_srcB wb_sel store_data",
    "alu": "RV32I ALU operations ADD SUB AND OR XOR SLT SLTU SLL SRL SRA alu_op encoding always_comb unique case operand shift signed",
    "alu_control": "RV32I ALU control funct3 funct7 opcode alu_op decode R_TYPE I_TYPE BRANCH LOAD STORE ALU_ADD ALU_SUB ALU_SRA ALU_SRL funct7[5]",
    "regfile":     "RV32I register file x0 hardwired read write port rd_addr reg_write write-before-read bypass rs1_addr rs2_addr always_comb",
    "if_stage": "RV32I instruction fetch PC program counter imem readmemh $readmemh $value$plusargs stall flush NOP pipeline register MEM_DEPTH word address byte address clog2",
    "ex_stage": "RV32I execute stage ALU operand mux forward_a forward_b forward_s opA_fwd opB_fwd opA_pre opB_pre store_data_fwd branch taken jump redirect jalr_target branch_target redirect_en redirect_pc ex_mem_alu_result wb_data",
    "mem_stage":   "RV32I memory stage load store byte enable sign extension zero extension LB LBU LH LHU LW SB SH SW byte_off dmem_byte_en dmem_wdata dmem_addr mem_rdata_out dmem_we dmem_re sub-word addressing funct3",
    "hazard_unit": "RV32I hazard unit load use hazard stall_pc stall_if_id flush_if_id flush_id_ex id_ex_mem_read id_ex_rd if_id_instr redirect_en branch jump bubble NOP pipeline stall flush priority",
    "forward_unit": "RV32I forwarding unit forward_a forward_b forward_s id_ex_rs1 id_ex_rs2 id_ex_alu_srcB ex_mem_rd mem_wb_rd ex_mem_reg_write mem_wb_reg_write EX MEM WB priority store_data_fwd alu_srcB guard",
    "wb_stage": "RV32I writeback stage wb_sel mux alu_result mem_rdata pc_plus4 imm_u WB_ALU WB_MEM WB_PC4 WB_IMM_U always_comb unique case",
    "top": "RV32I top level pipeline integration instantiation IF/ID ID/EX EX/MEM MEM/WB pipeline registers pc_next wb_data forwarding hazard data memory dmem ex_mem_fwd_data LUI JAL forwarding correction MEM_DEPTH 8192 DMEM_ADDR_BITS flush stall",
}

# ─────────────────────────────────────────────
# Main — generate all modules
# ─────────────────────────────────────────────
def main():
    # Generate a single module (change key to generate others)
    # Options: "id_stage", "alu", "alu_control", "regfile",
    #          "if_stage", "ex_stage", "mem_stage", "hazard_unit", "forward_unit", "wb_stage", "top"

    target = "regfile"   # ← change this to generate other modules
    k_rtl  = 2
    label = f"{target}_claude_haiku_v1"

    verilog = generate_module(
        task_label  = label,
        task        = TASKS[target],
        retrieval_query = RETRIEVAL_QUERIES[target],
        k_rtl       = k_rtl,
        max_retries = 3,
    )

    print("\n" + "="*60)
    print("GENERATED RTL:")
    print("="*60)
    print(verilog)


if __name__ == "__main__":
    main()