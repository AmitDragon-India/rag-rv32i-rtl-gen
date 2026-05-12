from pathlib import Path
from pydoc import text
from typing import List
import re

def enrich_markdown_chunk(text: str, source_stem: str) -> str:
    """
    Add semantic keywords based on content patterns.
    Applies to any corpus file — not ID-stage specific.
    """
    tags = []

    # Source file context — always add
    tags.append(f"Source: {source_stem}")

    # ── Structural patterns ──────────────────────────────
    # Tables (markdown pipe tables have weak embeddings)
    if text.count("|") > 4:
        tags.append("Keywords: table reference specification")

    # Code blocks
    if "```systemverilog" in text or "```verilog" in text:
        tags.append("Keywords: SystemVerilog RTL code example")

    # ── RV32I domain detection ───────────────────────────
    # Opcode map
    if any(x in text for x in ["R_TYPE", "I_TYPE", "LOAD", "STORE", "BRANCH", "JAL", "JALR", "LUI", "AUIPC"]):
        tags.append("Keywords: RV32I opcode localparam instruction type")

    # Control signals
    if any(x in text for x in ["reg_write", "mem_read", "mem_write", "alu_srcA", "alu_srcB", "wb_sel"]):
        tags.append("Keywords: RV32I control signals decode")

    # Immediate generation
    if any(x in text for x in ["imm_i", "imm_s", "imm_b", "imm_u", "imm_j"]):
        tags.append("Keywords: RV32I immediate generation sign extension")

    # ALU operations
    if any(x in text for x in ["ALU_ADD", "ALU_SUB", "ALU_SLT", "ALU_SLL", "ALU_SRA", "alu_op"]):
        tags.append("Keywords: RV32I ALU operation alu_op encoding")

    # Pipeline / hazard
    if any(x in text for x in ["stall", "flush", "pipeline", "hazard", "forward"]):
        tags.append("Keywords: RV32I pipeline stall flush hazard control")

    # Fix — require more specific regfile signals together
    if any(x in text for x in ["rs1_addr", "rs2_addr", "rd_addr", "hardwired"]) or \
       ("reg_write" in text and "rd_addr" in text):
        tags.append("Keywords: RV32I register file read write port x0")

    # Memory / load store
    if any(x in text for x in ["byte_en", "mem_rdata", "LB", "LH", "LW", "SB", "SH", "SW", 
                                "sext", "zext", "byte_off", "dmem_addr", "dmem_wdata",
                                "dmem_byte_en", "mem_rdata_extended", "sign-extend", 
                                "zero-extend", "FUNCT3_LB", "FUNCT3_LH", "FUNCT3_SW"]):
        tags.append("Keywords: RV32I memory load store byte enable sign extension byte_off sub-word dmem")

    # MEM stage specific
    if any(x in text for x in ["dmem_we", "dmem_re", "dmem_rdata", "mem_rdata_out", 
                                "byte_off", "dmem_byte_en", "mem_stage"]):
        tags.append("Keywords: rv32i_mem_stage memory access stage interface dmem byte_off load store")

    # EX stage specific  
    if any(x in text for x in ["opA_fwd", "opB_fwd", "opA_pre", "opB_pre", "alu_srcA", 
                                "redirect_en", "redirect_pc", "jalr_target", "branch_target",
                                "store_data_fwd", "ex_stage"]):
        tags.append("Keywords: rv32i_ex_stage execute stage forwarding redirect ALU branch jump")
    
    
    # Bug/fix patterns
    if any(x in text for x in ["always_ff", "non-blocking", "blocking", "latch", "default"]):
        tags.append("Keywords: RTL bug fix always_ff always_comb blocking non-blocking latch")

    # Branch/jump resolution
    if any(x in text for x in ["branch_taken", "redirect", "branch_target", "jump_target", "BEQ", "BNE", "BLT"]):
        tags.append("Keywords: RV32I branch jump redirect PC target comparison")
        
    # Forwarding unit
    if any(x in text for x in ["forward_a", "forward_b", "forward_s", "ex_mem_alu_result", 
                                "wb_data", "forward_unit", "forwarding", "store_data_fwd",
                                "opA_fwd", "opB_fwd"]):
        tags.append("Keywords: RV32I forwarding unit EX MEM WB operand forward select store")
        
    # Hazard unit
    if any(x in text for x in ["load_use_hazard", "stall_pc", "stall_if_id", "flush_if_id", "flush_id_ex", "hazard_unit"]):
        tags.append("Keywords: RV32I hazard unit load use stall flush control")

    # ── Assemble ─────────────────────────────────────────
    prefix = "\n".join(tags)
    return f"{prefix}\n\n{text}"


def read_text_file(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="ignore")


def is_rtl_file(path: Path) -> bool:
    return path.suffix.lower() in [".v", ".sv"]


def is_markdown_file(path: Path) -> bool:
    return path.suffix.lower() in [".md"]


def split_markdown_by_headers(text: str) -> List[str]:
    """
    Split markdown by headers (h1-h3).
    Any section larger than 2000 chars is sub-chunked to avoid
    oversized retrieval chunks that waste context window space.
    """
    parts = re.split(r'(?=^#{1,3}\s)', text, flags=re.MULTILINE)
    parts = [p.strip() for p in parts if p.strip()]

    result = []
    for part in parts:
        # CHANGE TO:
        if len(part) > 4000:
            result.extend(chunk_large_text(part, max_chars=3500, overlap=200))
        else:
            result.append(part)
    return result


def chunk_large_text(text: str, max_chars: int = 1800, overlap: int = 200) -> List[str]:
    chunks = []
    start = 0
    n = len(text)

    while start < n:
        end = min(start + max_chars, n)

        # Don't split inside a code fence
        snippet = text[start:end]
        if snippet.count("```") % 2 != 0:
            last_fence = snippet.rfind("```")
            if last_fence > 0:
                new_end = start + last_fence
                # Only back up if it actually moves end backward
                # and we're making progress (avoid infinite loop)
                if new_end > start:
                    end = new_end
                else:
                    # Code block larger than max_chars — find closing fence
                    # and include the whole block as one chunk
                    closing = text.find("```", start + end)
                    if closing != -1:
                        end = min(closing + 3, n)
                    # else just proceed with current end

        chunk = text[start:end].strip()
        if chunk:
            chunks.append(chunk)
        if end == n:
            break

        # Guard against infinite loop — always advance
        next_start = max(0, end - overlap)
        if next_start <= start:
            next_start = start + 1
        start = next_start

    return chunks


def clean_filename(name: str) -> str:
    return re.sub(r'[^a-zA-Z0-9_\-\.]', '_', name)


def split_rtl_by_module(text: str) -> List[str]:
    """
    Split an RTL file into individual module texts by splitting on
    endmodule boundaries. Each returned string includes its endmodule.
    """
    parts = re.split(r'\bendmodule\b', text, flags=re.IGNORECASE)
    modules = []
    for part in parts:
        part = part.strip()
        if re.search(r'^\s*module\s+\w+', part, flags=re.MULTILINE):
            modules.append(part + "\nendmodule")
    return modules


def extract_module_name(module_text: str) -> str:
    m = re.search(r'^\s*module\s+(\w+)', module_text, flags=re.MULTILINE)
    return m.group(1) if m else "unknown_module"


def split_rtl_module_semantically(module_text: str) -> List[str]:
    """
    Split one RTL module into semantically meaningful chunks:
      - module header / port declarations
      - always_ff / always_comb / always @(*) / initial blocks
      - assign statements grouped from the module tail
    Falls back to full module if no behavioral blocks are found.
    """
    module_name = extract_module_name(module_text)

    # Find starts of major behavioral blocks
    block_pattern = re.compile(
        r'^\s*(always_ff\b|always_comb\b|always\s*@\s*\([^)]*\)|initial\b)',
        flags=re.MULTILINE
    )

    matches = list(block_pattern.finditer(module_text))
    chunks = []

    # If no major blocks, keep whole module as one chunk
    if not matches:
        return [f"// MODULE: {module_name}\n{module_text.strip()}"]

    # Header / port declarations before first behavioral block
    first_start = matches[0].start()
    header = module_text[:first_start].strip()
    if header:
        chunks.append(f"// MODULE: {module_name}\n// BLOCK_TYPE: header\n{header}")

    # Behavioral blocks — each always/initial block as its own chunk
    for i, match in enumerate(matches):
        start = match.start()
        end = matches[i + 1].start() if i + 1 < len(matches) else len(module_text)
        block_text = module_text[start:end].strip()

        block_type_match = re.match(
            r'^\s*(always_ff\b|always_comb\b|always\s*@\s*\([^)]*\)|initial\b)',
            block_text
        )
        block_type = block_type_match.group(1) if block_type_match else "behavioral"

        chunks.append(
            f"// MODULE: {module_name}\n// BLOCK_TYPE: {block_type}\n{block_text}"
        )

    # Collect assign statements that appear AFTER the last behavioral block
    # (avoids duplicating assigns already present in the header chunk)
    last_block_start = matches[-1].start()
    tail_text = module_text[last_block_start:]
    assign_lines = re.findall(r'^\s*assign\s+.*?;\s*$', tail_text, flags=re.MULTILINE)
    if assign_lines:
        assign_chunk = "\n".join(assign_lines).strip()
        chunks.append(
            f"// MODULE: {module_name}\n// BLOCK_TYPE: assign_group\n{assign_chunk}"
        )

    # Deduplicate exact repeats
    deduped = []
    seen = set()
    for c in chunks:
        key = c.strip()
        if key not in seen:
            deduped.append(c)
            seen.add(key)

    return deduped


def split_rtl_semantically(text: str) -> List[str]:
    """
    Split an RTL file first by module boundary, then split each
    module into semantic sub-chunks (header, behavioral blocks, assigns).
    """
    modules = split_rtl_by_module(text)
    all_chunks = []
    for module_text in modules:
        all_chunks.extend(split_rtl_module_semantically(module_text))
    return all_chunks
