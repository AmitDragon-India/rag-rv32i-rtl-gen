"""Auto-calibrate evaluation ground truth from actual retrieval results.

Instead of manually specifying expected chunks (tedious and error-prone),
this script:

1. Runs retrieval for each module using the ACTUAL generation queries
2. Saves the retrieved chunks as the ground truth baseline
3. Future evaluations compare against this baseline

The logic: if your pipeline generated RTL that passed 40/42 ISA tests,
then the chunks it retrieved ARE the correct ground truth.

Usage:
    # Step 1: Generate baseline with your current (working) retriever
    python -m app.calibrate --mode baseline

    # Step 2: After making changes, compare against baseline
    python -m app.calibrate --mode compare

    # Step 3: View current baseline
    python -m app.calibrate --mode view
"""

import json
import logging
import time
from pathlib import Path
from typing import Dict, List, Any
from dataclasses import dataclass, asdict

from app.config import settings

logger = logging.getLogger(__name__)

BASELINE_FILE = Path("outputs/eval_baseline.json")

# ─────────────────────────────────────────────
# Module generation queries
# ─────────────────────────────────────────────
# UPDATE THESE with your actual queries from generate_rtl.py
# Run: grep -A5 "query" scripts/generate_rtl.py
# and paste the queries here

MODULE_QUERIES = {
    "rv32i_alu": {
        "query": "RV32I ALU operations ADD SUB AND OR XOR SLT SLTU SLL SRL SRA alu_op encoding always_comb unique case operand shift signed",
        "k_rtl": 2,
    },
    "rv32i_alu_control": {
        "query": "RV32I ALU control funct3 funct7 opcode alu_op decode R_TYPE I_TYPE BRANCH LOAD STORE ALU_ADD ALU_SUB ALU_SRA ALU_SRL funct7[5]",
        "k_rtl": 2,
    },
    "rv32i_regfile": {
        "query": "RV32I register file x0 hardwired read write port rd_addr reg_write write-before-read bypass rs1_addr rs2_addr always_comb",
        "k_rtl": 2,
    },
    "rv32i_if_stage": {
        "query": "RV32I instruction fetch PC program counter imem readmemh $readmemh $value$plusargs stall flush NOP pipeline register MEM_DEPTH word address byte address clog2",
        "k_rtl": 2,
    },
    "rv32i_id_stage": {
        "query": "RV32I instruction decode RV32I opcode localparam control signal table immediate generation imm_i imm_s imm_b imm_u imm_j unique case always_comb reg_write mem_read mem_write alu_srcA alu_srcB wb_sel store_data",
        "k_rtl": 2,
    },
    "rv32i_ex_stage": {
        "query": "RV32I execute stage ALU operand mux forward_a forward_b forward_s opA_fwd opB_fwd opA_pre opB_pre store_data_fwd branch taken jump redirect jalr_target branch_target redirect_en redirect_pc ex_mem_alu_result wb_data",
        "k_rtl": 2,
    },
    "rv32i_mem_stage": {
        "query": "RV32I memory stage load store byte enable sign extension zero extension LB LBU LH LHU LW SB SH SW byte_off dmem_byte_en dmem_wdata dmem_addr mem_rdata_out dmem_we dmem_re sub-word addressing funct3",
        "k_rtl": 3,
    },
    "rv32i_wb_stage": {
        "query": "RV32I writeback stage wb_sel mux alu_result mem_rdata pc_plus4 imm_u WB_ALU WB_MEM WB_PC4 WB_IMM_U always_comb unique case",
        "k_rtl": 2,
    },
    "rv32i_hazard_unit": {
        "query": "RV32I hazard unit load use hazard stall_pc stall_if_id flush_if_id flush_id_ex id_ex_mem_read id_ex_rd if_id_instr redirect_en branch jump bubble NOP pipeline stall flush priority",
        "k_rtl": 2,
    },
    "rv32i_forward_unit": {
        "query": "RV32I forwarding unit forward_a forward_b forward_s id_ex_rs1 id_ex_rs2 id_ex_alu_srcB ex_mem_rd mem_wb_rd ex_mem_reg_write mem_wb_reg_write EX MEM WB priority store_data_fwd alu_srcB guard",
        "k_rtl": 2,
    },
    "rv32i_top": {
        "query": "RV32I top level pipeline integration instantiation IF/ID ID/EX EX/MEM MEM/WB pipeline registers pc_next wb_data forwarding hazard data memory dmem ex_mem_fwd_data LUI JAL forwarding correction MEM_DEPTH 8192 DMEM_ADDR_BITS flush stall",
        "k_rtl": 2,
    },
}


def generate_baseline(mode: str = "dense_only", enable_reranker: bool = False) -> Dict:
    """
    Run retrieval for all modules and save results as ground truth.

    Uses dense_only mode by default since that's what produced the
    40/42 passing RTL originally.
    """
    from app.hybrid_retriever import HybridRetriever

    retriever = HybridRetriever()
    retriever.load(mode=mode, enable_reranker=enable_reranker)

    baseline = {
        "created_at": time.strftime("%Y-%m-%d %H:%M:%S"),
        "mode": mode,
        "reranker": enable_reranker,
        "modules": {},
    }

    for module_name, config in MODULE_QUERIES.items():
        logger.info(f"Calibrating: {module_name}")

        result = retriever.retrieve(
            query=config["query"],
            k_rtl=config["k_rtl"],
            rerank=enable_reranker,
        )

        docs = result["documents"]

        # Save chunk IDs and sources as ground truth
        chunk_ids = [d.metadata.get("chunk_id", "") for d in docs]
        sources = list(set(Path(d.metadata.get("source", "")).name for d in docs))
        source_types = {d.metadata.get("chunk_id", ""): d.metadata.get("type", "") for d in docs}

        baseline["modules"][module_name] = {
            "query": config["query"],
            "k_rtl": config["k_rtl"],
            "chunk_ids": chunk_ids,
            "sources": sources,
            "source_types": source_types,
            "total_chunks": len(chunk_ids),
            "retrieval_time_ms": result["metadata"].get("total_retrieval_time_ms", 0),
        }

        logger.info(
            f"  {module_name}: {len(chunk_ids)} chunks from {len(sources)} sources "
            f"({result['metadata'].get('total_retrieval_time_ms', 0):.0f}ms)"
        )

    # Save baseline
    BASELINE_FILE.parent.mkdir(parents=True, exist_ok=True)
    with open(BASELINE_FILE, "w") as f:
        json.dump(baseline, f, indent=2)

    logger.info(f"Baseline saved to {BASELINE_FILE}")
    return baseline


def load_baseline() -> Dict:
    """Load existing baseline."""
    if not BASELINE_FILE.exists():
        raise FileNotFoundError(
            f"No baseline found at {BASELINE_FILE}. "
            "Run: python -m app.calibrate --mode baseline"
        )
    with open(BASELINE_FILE) as f:
        return json.load(f)


def compare_against_baseline(
    mode: str = "hybrid",
    enable_reranker: bool = True,
) -> Dict:
    """
    Compare a new retrieval strategy against the saved baseline.

    Metrics:
    - Overlap: how many baseline chunks are still retrieved?
    - New chunks: what chunks are retrieved that weren't in baseline?
    - Missing chunks: what baseline chunks are no longer retrieved?
    - Source consistency: are the same source files represented?
    """
    from app.hybrid_retriever import HybridRetriever

    baseline = load_baseline()

    retriever = HybridRetriever()
    retriever.load(mode=mode, enable_reranker=enable_reranker)

    comparison = {
        "baseline_mode": baseline["mode"],
        "new_mode": mode,
        "new_reranker": enable_reranker,
        "modules": {},
        "summary": {},
    }

    total_overlap = 0
    total_baseline_chunks = 0
    total_new_chunks = 0
    total_missing = 0

    for module_name, bl in baseline["modules"].items():
        config = MODULE_QUERIES.get(module_name)
        if not config:
            continue

        result = retriever.retrieve(
            query=config["query"],
            k_rtl=config["k_rtl"],
            rerank=enable_reranker,
        )

        new_chunks = [d.metadata.get("chunk_id", "") for d in result["documents"]]
        bl_chunks = set(bl["chunk_ids"])

        overlap = [c for c in new_chunks if c in bl_chunks]
        missing = [c for c in bl_chunks if c not in new_chunks]
        added = [c for c in new_chunks if c not in bl_chunks]

        overlap_pct = len(overlap) / len(bl_chunks) if bl_chunks else 0

        comparison["modules"][module_name] = {
            "baseline_chunks": len(bl_chunks),
            "new_chunks": len(new_chunks),
            "overlap": len(overlap),
            "overlap_pct": round(overlap_pct, 4),
            "missing": missing,
            "added": added,
            "retrieval_time_ms": result["metadata"].get("total_retrieval_time_ms", 0),
        }

        total_overlap += len(overlap)
        total_baseline_chunks += len(bl_chunks)
        total_new_chunks += len(added)
        total_missing += len(missing)

        status = "✅" if overlap_pct >= 0.7 else "⚠️" if overlap_pct >= 0.4 else "❌"
        logger.info(
            f"  {status} {module_name}: overlap={overlap_pct:.0%} "
            f"({len(overlap)}/{len(bl_chunks)}), "
            f"+{len(added)} new, -{len(missing)} missing"
        )

    # Summary
    avg_overlap = total_overlap / total_baseline_chunks if total_baseline_chunks else 0
    comparison["summary"] = {
        "avg_overlap_pct": round(avg_overlap, 4),
        "total_baseline_chunks": total_baseline_chunks,
        "total_overlap": total_overlap,
        "total_new_added": total_new_chunks,
        "total_missing": total_missing,
    }

    return comparison


def print_comparison(comp: Dict):
    """Pretty-print comparison results."""
    print(f"\n{'=' * 65}")
    print(f"RETRIEVAL COMPARISON: {comp['baseline_mode']} → {comp['new_mode']}")
    print(f"{'=' * 65}")

    for name, data in comp["modules"].items():
        overlap_pct = data["overlap_pct"]
        status = "✅" if overlap_pct >= 0.7 else "⚠️" if overlap_pct >= 0.4 else "❌"
        print(
            f"  {status} {name:25s} | overlap: {overlap_pct:5.0%} "
            f"({data['overlap']}/{data['baseline_chunks']}) "
            f"| +{len(data['added'])} new | -{len(data['missing'])} lost "
            f"| {data['retrieval_time_ms']:.0f}ms"
        )

    s = comp["summary"]
    print(f"\n{'─' * 65}")
    print(f"  OVERALL: {s['avg_overlap_pct']:.0%} baseline overlap")
    print(f"  {s['total_overlap']}/{s['total_baseline_chunks']} baseline chunks preserved")
    print(f"  +{s['total_new_added']} new chunks added")
    print(f"  -{s['total_missing']} baseline chunks lost")
    print(f"{'=' * 65}")


def view_baseline():
    """Print current baseline contents."""
    baseline = load_baseline()
    print(f"\nBaseline created: {baseline['created_at']}")
    print(f"Mode: {baseline['mode']}, Reranker: {baseline.get('reranker', False)}")
    print(f"\n{'─' * 65}")

    for name, data in baseline["modules"].items():
        print(f"\n  {name} ({data['total_chunks']} chunks, {data['retrieval_time_ms']:.0f}ms)")
        print(f"    Sources: {', '.join(data['sources'])}")
        print(f"    Chunks:  {', '.join(data['chunk_ids'][:5])}")
        if len(data["chunk_ids"]) > 5:
            print(f"             ... +{len(data['chunk_ids']) - 5} more")


if __name__ == "__main__":
    import argparse

    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s | %(levelname)-8s | %(name)s | %(message)s",
    )

    parser = argparse.ArgumentParser(description="Calibrate evaluation baseline")
    parser.add_argument(
        "--mode",
        choices=["baseline", "compare", "view"],
        default="baseline",
        help=(
            "baseline: generate ground truth from current retriever | "
            "compare: compare new retriever against baseline | "
            "view: show current baseline"
        ),
    )
    args = parser.parse_args()

    if args.mode == "baseline":
        print("Generating baseline using dense-only retrieval...")
        print("(This captures the chunks that produced your 40/42 passing RTL)")
        baseline = generate_baseline(mode="dense_only", enable_reranker=False)
        print(f"\n✅ Baseline saved with {len(baseline['modules'])} modules")
        view_baseline()

    elif args.mode == "compare":
        print("Comparing hybrid retrieval against baseline...")
        comp = compare_against_baseline(mode="hybrid", enable_reranker=True)
        print_comparison(comp)

        # Save comparison
        out_path = "outputs/eval_comparison.json"
        with open(out_path, "w") as f:
            json.dump(comp, f, indent=2)
        print(f"\nSaved to {out_path}")

    elif args.mode == "view":
        view_baseline()
