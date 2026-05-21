"""Gradio UI for interactive RAG-RV32I RTL generation."""

import sys
import json
import logging
from pathlib import Path

import gradio as gr

from app.hybrid_retriever import hybrid_retriever
from app.generator import generator

logger = logging.getLogger(__name__)

# ─────────────────────────────────────────────
# Import TASKS and RETRIEVAL_QUERIES from generation script
# ─────────────────────────────────────────────
sys.path.insert(0, str(Path(__file__).parent.parent / "scripts"))
try:
    from generate_rtl_2 import TASKS, RETRIEVAL_QUERIES

    # Build EXAMPLE_MODULES from the actual generation config
    # Maps display names to their task + query
    _MODULE_DISPLAY_NAMES = {
        "alu": "ALU",
        "alu_control": "ALU Control",
        "regfile": "Register File",
        "if_stage": "IF Stage",
        "id_stage": "ID Stage",
        "ex_stage": "EX Stage",
        "mem_stage": "MEM Stage",
        "wb_stage": "WB Stage",
        "hazard_unit": "Hazard Unit",
        "forward_unit": "Forward Unit",
        "top": "Top Module",
    }

    EXAMPLE_MODULES = {}
    for key, display_name in _MODULE_DISPLAY_NAMES.items():
        if key in TASKS and key in RETRIEVAL_QUERIES:
            EXAMPLE_MODULES[display_name] = {
                "query": RETRIEVAL_QUERIES[key],
                "task": TASKS[key],
                "label": f"rv32i_{key}" if not key.startswith("rv32i_") else key,
            }

    logger.info(f"Loaded {len(EXAMPLE_MODULES)} example modules from generate_rtl_2.py")

except ImportError:
    logger.warning("Could not import TASKS/RETRIEVAL_QUERIES from generate_rtl_2.py — using empty examples")
    EXAMPLE_MODULES = {}



def retrieve_context(query: str, k_rtl: int):
    """Retrieve and display context documents."""
    if not hybrid_retriever.is_loaded:
        return "⚠️ Retriever not loaded. Run build_index.py first.", "{}"

    result = hybrid_retriever.retrieve(query, k_rtl=int(k_rtl))

    # Format documents for display
    doc_display = []
    for i, doc in enumerate(result["documents"], 1):
        source = doc.metadata.get("source", "unknown")
        chunk_id = doc.metadata.get("chunk_id", "")
        doc_type = doc.metadata.get("type", "")
        preview = doc.page_content[:300].replace("\n", " ")
        doc_display.append(
            f"**[{i}] {chunk_id}**\n"
            f"Source: `{source}` | Type: `{doc_type}`\n"
            f"> {preview}...\n"
        )

    docs_text = "\n---\n".join(doc_display) if doc_display else "No documents found."
    metadata_text = json.dumps(result["metadata"], indent=2)

    return docs_text, metadata_text


def generate_rtl(task: str, query: str, task_label: str, k_rtl: int):
    """Generate RTL and return verilog + metadata."""
    if not hybrid_retriever.is_loaded:
        return "⚠️ Retriever not loaded.", "{}", "{}"

    try:
        result = generator.generate(
            task=task,
            retrieval_query=query,
            task_label=task_label or "module",
            k_rtl=int(k_rtl),
        )

        validation_text = json.dumps(result["validation"], indent=2)
        gen_metadata = json.dumps(result["generation_metadata"], indent=2)

        return result["verilog"], validation_text, gen_metadata

    except Exception as e:
        logger.error(f"Generation failed: {e}")
        return f"❌ Error: {e}", "{}", "{}"


def load_example(module_name: str):
    """Load predefined module example into form fields."""
    if module_name in EXAMPLE_MODULES:
        ex = EXAMPLE_MODULES[module_name]
        return ex["task"], ex["query"], ex.get("label", module_name.lower().replace(" ", "_"))
    return "", "", ""


def build_ui() -> gr.Blocks:
    """Build the Gradio interface."""

    with gr.Blocks(
        title="RAG-RV32I RTL Generator",
    ) as demo:
        gr.Markdown(
            """
            # 🔧 RAG-RV32I RTL Generator
            **Generate synthesizable SystemVerilog for RV32I processor modules using RAG + LLM**

            Pipeline: Specification → FAISS Retrieval (BGE embeddings) → Context Assembly → Claude Haiku → SystemVerilog

            *40/42 rv32ui ISA tests passing (95.2%) on generated RTL*
            """
        )

        # ── Quick start examples ─────────────────────
        with gr.Row():
            example_dropdown = gr.Dropdown(
                choices=list(EXAMPLE_MODULES.keys()),
                label="Quick Start — Load Example Module",
                interactive=True,
            )
            load_btn = gr.Button("Load Example", variant="secondary")

        # ── Main input form ──────────────────────────
        with gr.Row():
            with gr.Column(scale=2):
                task_input = gr.Textbox(
                    label="Module Specification",
                    placeholder="Describe the SystemVerilog module to generate...",
                    lines=10,
                )
                query_input = gr.Textbox(
                    label="Retrieval Query",
                    placeholder="Keywords for context retrieval...",
                    lines=2,
                )
            with gr.Column(scale=1):
                label_input = gr.Textbox(
                    label="Task Label",
                    value="module",
                    lines=1,
                )
                k_rtl_slider = gr.Slider(
                    minimum=0,
                    maximum=6,
                    value=3,
                    step=1,
                    label="RTL Reference Chunks (k_rtl)",
                )

        # ── Action buttons ───────────────────────────
        with gr.Row():
            retrieve_btn = gr.Button("🔍 Retrieve Context", variant="secondary")
            generate_btn = gr.Button("⚡ Generate RTL", variant="primary")

        # ── Retrieval results ────────────────────────
        with gr.Accordion("Retrieved Context", open=False):
            retrieved_docs = gr.Markdown(label="Documents")
            retrieval_meta = gr.Code(label="Retrieval Metadata", language="json")

        # ── Generation results ───────────────────────
        with gr.Row():
            with gr.Column(scale=2):
                verilog_output = gr.Code(
                    label="Generated SystemVerilog",
                    language="python",
                    lines=25,
                )
            with gr.Column(scale=1):
                validation_output = gr.Code(
                    label="Validation",
                    language="json",
                )
                gen_meta_output = gr.Code(
                    label="Generation Metadata",
                    language="json",
                )

        # ── Wire up events ───────────────────────────
        load_btn.click(
            fn=load_example,
            inputs=[example_dropdown],
            outputs=[task_input, query_input, label_input],
        )

        retrieve_btn.click(
            fn=retrieve_context,
            inputs=[query_input, k_rtl_slider],
            outputs=[retrieved_docs, retrieval_meta],
        )

        generate_btn.click(
            fn=generate_rtl,
            inputs=[task_input, query_input, label_input, k_rtl_slider],
            outputs=[verilog_output, validation_output, gen_meta_output],
        )

    return demo


def launch_ui():
    """Launch the Gradio UI."""
    # Load retriever if not already loaded
    if not hybrid_retriever.is_loaded:
        try:
            hybrid_retriever.load(mode="hybrid", enable_reranker=True)
        except Exception as e:
            logger.warning(f"Could not load retriever: {e}")

    demo = build_ui()
    demo.launch(server_name="0.0.0.0", server_port=7860, share=False, theme=gr.themes.Soft())


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    launch_ui()