"""Production RTL generator with LLM integration, validation, and metrics."""

import re
import os
import time
import logging
from pathlib import Path
from typing import Dict, Any, Optional, List

import anthropic
from langchain_core.documents import Document

from app.config import settings
from app.hybrid_retriever import hybrid_retriever

logger = logging.getLogger(__name__)


class RTLGenerator:
    """Generates synthesizable SystemVerilog from specs using RAG + LLM."""

    def __init__(self):
        api_key = os.environ.get("ANTHROPIC_API_KEY")
        if not api_key:
            logger.warning("ANTHROPIC_API_KEY not set — generation will fail")
            self._client = None
        else:
            self._client = anthropic.Anthropic(api_key=api_key)

    def _build_prompt(self, task: str, docs: List[Document]) -> str:
        """Assemble generation prompt from task spec and retrieved context."""
        context_blocks = []
        for i, doc in enumerate(docs, start=1):
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
Return only SystemVerilog code starting with 'module' and ending with 'endmodule'."""

        return prompt.strip()

    def _call_llm(self, prompt: str) -> str:
        """Call Claude API for RTL generation."""
        if not self._client:
            raise RuntimeError("Anthropic client not initialized — set ANTHROPIC_API_KEY")

        cfg = settings.generation
        start = time.time()

        message = self._client.messages.create(
            model=cfg.model,
            max_tokens=cfg.max_tokens,
            temperature=cfg.temperature,
            messages=[{"role": "user", "content": prompt}],
        )

        elapsed = time.time() - start
        response_text = message.content[0].text

        logger.info(
            f"LLM call completed in {elapsed:.2f}s "
            f"(model={cfg.model}, tokens_in={message.usage.input_tokens}, "
            f"tokens_out={message.usage.output_tokens})"
        )

        return response_text

    @staticmethod
    def extract_verilog(text: str) -> str:
        """Extract SystemVerilog code from LLM response."""
        # Try fenced code block first
        fenced = re.search(
            r"```(?:systemverilog|verilog)?\s*(.*?)```",
            text,
            flags=re.DOTALL | re.IGNORECASE,
        )
        if fenced:
            return fenced.group(1).strip()

        # Fall back to module...endmodule
        module_match = re.search(r"(module\b.*?endmodule)", text, flags=re.DOTALL)
        if module_match:
            return module_match.group(1).strip()

        return text.strip()

    @staticmethod
    def validate_verilog(verilog: str) -> Dict[str, Any]:
        """Validate extracted SystemVerilog and return diagnostics."""
        has_module = bool(re.search(r"^\s*module\s+\w+", verilog, re.MULTILINE))
        has_endmodule = "endmodule" in verilog
        has_content = len(verilog) > 100

        # Extract module name
        name_match = re.search(r"^\s*module\s+(\w+)", verilog, re.MULTILINE)
        module_name = name_match.group(1) if name_match else None

        # Count always blocks
        always_ff_count = len(re.findall(r"\balways_ff\b", verilog))
        always_comb_count = len(re.findall(r"\balways_comb\b", verilog))

        is_valid = has_module and has_endmodule and has_content

        return {
            "is_valid": is_valid,
            "module_name": module_name,
            "char_count": len(verilog),
            "line_count": verilog.count("\n") + 1,
            "always_ff_count": always_ff_count,
            "always_comb_count": always_comb_count,
            "has_module": has_module,
            "has_endmodule": has_endmodule,
        }

    def generate(
        self,
        task: str,
        retrieval_query: str,
        task_label: str = "module",
        k_rtl: int = None,
    ) -> Dict[str, Any]:
        """
        Full pipeline: retrieve context → build prompt → generate RTL → validate.

        Returns dict with 'verilog', 'validation', 'retrieval_metadata', 'generation_metadata'.
        """
        pipeline_start = time.time()
        cfg = settings.generation

        # ── Step 1: Retrieve ─────────────────────────────
        retrieval_result = hybrid_retriever.retrieve(retrieval_query, k_rtl=k_rtl)
        docs = retrieval_result["documents"]

        logger.info(f"[{task_label}] Retrieved {len(docs)} context documents")

        # ── Step 2: Build prompt ─────────────────────────
        prompt = self._build_prompt(task, docs)
        prompt_tokens_est = len(prompt.split())

        # ── Step 3: Generate with retries ────────────────
        verilog = ""
        validation = {"is_valid": False}
        attempts = []

        for attempt in range(1, cfg.max_retries + 1):
            logger.info(f"[{task_label}] Generation attempt {attempt}/{cfg.max_retries}")

            try:
                raw_output = self._call_llm(prompt)
                verilog = self.extract_verilog(raw_output)
                validation = self.validate_verilog(verilog)

                attempts.append({
                    "attempt": attempt,
                    "valid": validation["is_valid"],
                    "char_count": validation["char_count"],
                })

                if validation["is_valid"]:
                    logger.info(
                        f"[{task_label}] Valid RTL generated on attempt {attempt} "
                        f"({validation['char_count']} chars, {validation['line_count']} lines)"
                    )
                    break
                else:
                    logger.warning(f"[{task_label}] Invalid RTL on attempt {attempt}")

            except Exception as e:
                logger.error(f"[{task_label}] LLM call failed on attempt {attempt}: {e}")
                attempts.append({"attempt": attempt, "valid": False, "error": str(e)})

            if attempt < cfg.max_retries:
                time.sleep(cfg.retry_delay_seconds)

        pipeline_elapsed = time.time() - pipeline_start

        result = {
            "verilog": verilog,
            "validation": validation,
            "retrieval_metadata": retrieval_result["metadata"],
            "generation_metadata": {
                "task_label": task_label,
                "model": cfg.model,
                "attempts": attempts,
                "total_attempts": len(attempts),
                "prompt_words_est": prompt_tokens_est,
                "pipeline_time_ms": round(pipeline_elapsed * 1000, 2),
            },
        }

        logger.info(
            f"[{task_label}] Pipeline completed in {pipeline_elapsed:.2f}s "
            f"(valid={validation['is_valid']}, attempts={len(attempts)})"
        )

        return result

    def save_output(self, task_label: str, verilog: str) -> Path:
        """Save generated RTL to output directory."""
        output_dir = Path(settings.output.generated_rtl_dir)
        output_dir.mkdir(parents=True, exist_ok=True)

        filepath = output_dir / f"{task_label}.sv"
        filepath.write_text(verilog, encoding="utf-8")

        logger.info(f"Saved RTL to {filepath}")
        return filepath


# Singleton instance
generator = RTLGenerator()
