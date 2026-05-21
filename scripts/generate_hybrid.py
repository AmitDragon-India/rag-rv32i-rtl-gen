"""Generate all RV32I modules using HYBRID retrieval (FAISS + BM25 + Reranker).

This is identical to generate_rtl_2.py EXCEPT:
- Uses hybrid_retriever instead of retrieve_structured (FAISS-only)
- Saves to outputs/generated_rtl_hybrid/ for comparison
- Generates ALL modules in one run

Usage:
    python scripts/generate_hybrid.py           # generate all modules
    python scripts/generate_hybrid.py --module alu  # generate one module
"""

import re
import sys
import time
import argparse
from pathlib import Path
import os

# Add project root to path so we can import app modules
sys.path.insert(0, str(Path(__file__).parent.parent))

from app.hybrid_retriever import hybrid_retriever

import anthropic
client = anthropic.Anthropic(api_key=os.environ.get("ANTHROPIC_API_KEY"))

OUTPUT_DIR = Path("outputs/generated_rtl_hybrid")
CTX_DIR = Path("outputs/retrieved_context_hybrid")


# ─────────────────────────────────────────────
# Prompt builder (identical to original)
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
# LLM call (identical to original)
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


def extract_verilog(text: str) -> str:
    fenced = re.search(r"```(?:systemverilog|verilog)?\s*(.*?)```", text, flags=re.DOTALL | re.IGNORECASE)
    if fenced:
        return fenced.group(1).strip()
    module_match = re.search(r"(module\b.*?endmodule)", text, flags=re.DOTALL)
    if module_match:
        return module_match.group(1).strip()
    return text.strip()


def is_valid_verilog(verilog: str) -> bool:
    has_module = bool(re.search(r'^\s*module\s+\w+', verilog, re.MULTILINE))
    has_endmodule = "endmodule" in verilog
    has_content = len(verilog) > 100
    return has_module and has_endmodule and has_content


def save_outputs(task_label, task_text, prompt, context_docs, raw_output, verilog):
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    CTX_DIR.mkdir(parents=True, exist_ok=True)

    with open(CTX_DIR / f"{task_label}_context.txt", "w", encoding="utf-8") as f:
        f.write(f"TASK LABEL:\n{task_label}\n\n")
        f.write(f"TASK:\n{task_text}\n\n")
        f.write(f"RETRIEVAL MODE: hybrid (FAISS + BM25 + Reranker)\n\n")
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

    print(f"  Saved: {OUTPUT_DIR}/{task_label}.sv")


# ─────────────────────────────────────────────
# Generate one module with HYBRID retrieval
# ─────────────────────────────────────────────
def generate_module(task_label: str, task: str, retrieval_query: str, k_rtl: int = 3, max_retries: int = 3):
    print(f"\n{'='*60}")
    print(f"Generating: {task_label}")
    print(f"Retrieval: HYBRID (FAISS + BM25 + Reranker)")
    print(f"{'='*60}")

    # ── HYBRID RETRIEVAL (the only change from original) ──
    result = hybrid_retriever.retrieve(
        query=retrieval_query,
        k_rtl=k_rtl,
        rerank=True,
    )
    docs = result["documents"]
    metadata = result["metadata"]

    print(f"  Retrieved {len(docs)} context chunks ({metadata['total_retrieval_time_ms']:.0f}ms):")
    print(f"    FAISS: {metadata.get('faiss_results', 0)}, BM25: {metadata.get('bm25_results', 0)}, Reranked: {metadata.get('reranked', False)}")
    for d in docs:
        print(f"    - {Path(d.metadata['source']).name} | {d.metadata['chunk_id']}")

    prompt = build_prompt(task, docs)

    for attempt in range(1, max_retries + 1):
        print(f"  Attempt {attempt}/{max_retries}...")
        raw_output = call_llm(prompt)
        verilog = extract_verilog(raw_output)

        if is_valid_verilog(verilog):
            print(f"  ✅ Valid RTL generated ({len(verilog)} chars)")
            save_outputs(task_label, task, prompt, docs, raw_output, verilog)
            return verilog
        else:
            print(f"  ❌ Invalid RTL on attempt {attempt} — retrying...")
            save_outputs(f"{task_label}_attempt{attempt}_failed", task, prompt, docs, raw_output, verilog)
            time.sleep(2)

    print(f"  ⚠️ All {max_retries} attempts failed for {task_label}")
    save_outputs(task_label, task, prompt, docs, raw_output, verilog)
    return verilog


# ─────────────────────────────────────────────
# Retrieval queries (from your generate_rtl_2.py)
# ─────────────────────────────────────────────
RETRIEVAL_QUERIES = {
    "id_stage":      "RV32I instruction decode RV32I opcode localparam control signal table immediate generation imm_i imm_s imm_b imm_u imm_j unique case always_comb reg_write mem_read mem_write alu_srcA alu_srcB wb_sel store_data",
    "alu":           "RV32I ALU operations ADD SUB AND OR XOR SLT SLTU SLL SRL SRA alu_op encoding always_comb unique case operand shift signed",
    "alu_control":   "RV32I ALU control funct3 funct7 opcode alu_op decode R_TYPE I_TYPE BRANCH LOAD STORE ALU_ADD ALU_SUB ALU_SRA ALU_SRL funct7[5]",
    "regfile":       "RV32I register file x0 hardwired read write port rd_addr reg_write write-before-read bypass rs1_addr rs2_addr always_comb",
    "if_stage":      "RV32I instruction fetch PC program counter imem readmemh $readmemh $value$plusargs stall flush NOP pipeline register MEM_DEPTH word address byte address clog2",
    "ex_stage":      "RV32I execute stage ALU operand mux forward_a forward_b forward_s opA_fwd opB_fwd opA_pre opB_pre store_data_fwd branch taken jump redirect jalr_target branch_target redirect_en redirect_pc ex_mem_alu_result wb_data",
    "mem_stage":     "RV32I memory stage load store byte enable sign extension zero extension LB LBU LH LHU LW SB SH SW byte_off dmem_byte_en dmem_wdata dmem_addr mem_rdata_out dmem_we dmem_re sub-word addressing funct3",
    "hazard_unit":   "RV32I hazard unit load use hazard stall_pc stall_if_id flush_if_id flush_id_ex id_ex_mem_read id_ex_rd if_id_instr redirect_en branch jump bubble NOP pipeline stall flush priority",
    "forward_unit":  "RV32I forwarding unit forward_a forward_b forward_s id_ex_rs1 id_ex_rs2 id_ex_alu_srcB ex_mem_rd mem_wb_rd ex_mem_reg_write mem_wb_reg_write EX MEM WB priority store_data_fwd alu_srcB guard",
    "wb_stage":      "RV32I writeback stage wb_sel mux alu_result mem_rdata pc_plus4 imm_u WB_ALU WB_MEM WB_PC4 WB_IMM_U always_comb unique case",
    "top":           "RV32I top level pipeline integration instantiation IF/ID ID/EX EX/MEM MEM/WB pipeline registers pc_next wb_data forwarding hazard data memory dmem ex_mem_fwd_data LUI JAL forwarding correction MEM_DEPTH 8192 DMEM_ADDR_BITS flush stall",
}

# Module generation order (dependencies first)
GENERATION_ORDER = [
    "alu",
    "alu_control",
    "regfile",
    "if_stage",
    "id_stage",
    "ex_stage",
    "mem_stage",
    "wb_stage",
    "hazard_unit",
    "forward_unit",
    "top",
]


def main():
    parser = argparse.ArgumentParser(description="Generate RV32I modules with hybrid retrieval")
    parser.add_argument(
        "--module",
        choices=list(RETRIEVAL_QUERIES.keys()),
        default=None,
        help="Generate a single module (default: all modules)",
    )
    parser.add_argument("--k-rtl", type=int, default=2, help="RTL reference chunks")
    args = parser.parse_args()

    # Import TASKS from original generate_rtl_2.py
    # We need to add the scripts directory to path
    sys.path.insert(0, str(Path(__file__).parent))
    from generate_rtl_2 import TASKS

    # Load hybrid retriever
    print("Loading hybrid retriever (FAISS + BM25 + Reranker)...")
    hybrid_retriever.load(mode="hybrid", enable_reranker=True)
    print("Retriever loaded.\n")

    if args.module:
        # Generate single module
        modules = [args.module]
    else:
        # Generate all modules
        modules = GENERATION_ORDER

    results = {}
    for module in modules:
        if module not in TASKS:
            print(f"⚠️ No task spec for '{module}' — skipping")
            continue

        label = f"rv32i_{module}" if not module.startswith("rv32i_") else module
        verilog = generate_module(
            task_label=label,
            task=TASKS[module],
            retrieval_query=RETRIEVAL_QUERIES[module],
            k_rtl=args.k_rtl,
            max_retries=3,
        )
        results[module] = {
            "valid": is_valid_verilog(verilog),
            "chars": len(verilog),
        }

    # Summary
    print(f"\n{'='*60}")
    print("GENERATION SUMMARY (Hybrid Retrieval)")
    print(f"{'='*60}")
    for module, info in results.items():
        status = "✅" if info["valid"] else "❌"
        print(f"  {status} {module:20s} | {info['chars']:5d} chars")

    valid = sum(1 for r in results.values() if r["valid"])
    print(f"\n  {valid}/{len(results)} modules generated successfully")
    print(f"  Output: {OUTPUT_DIR}/")
    print(f"\n  Next: compile with Verilator and run ISA tests")
    print(f"  Compare against outputs/generated_rtl_1/ (dense-only baseline)")


if __name__ == "__main__":
    main()
