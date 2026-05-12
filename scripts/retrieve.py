from pathlib import Path
from langchain_community.vectorstores import FAISS
from langchain_community.embeddings import HuggingFaceEmbeddings

VECTORSTORE_DIR = Path("vectorstore/faiss_index")

# Cache the DB so it is not reloaded from disk on every call
_db_cache = None


def load_retriever():
    global _db_cache
    if _db_cache is not None:
        return _db_cache

    # Must match the model used in build_index.py exactly
    embeddings = HuggingFaceEmbeddings(
        model_name="BAAI/bge-base-en-v1.5"
    )
    _db_cache = FAISS.load_local(
        str(VECTORSTORE_DIR),
        embeddings,
        allow_dangerous_deserialization=True
    )
    return _db_cache


def deduplicate_docs(docs):
    seen = set()
    unique_docs = []
    for d in docs:
        key = (d.metadata.get("source", ""), d.metadata.get("chunk_id", ""))
        if key not in seen:
            seen.add(key)
            unique_docs.append(d)
    return unique_docs


def retrieve_structured(query: str, k_rtl: int = 3):
    db = load_retriever()

    # 1. SPEC docs — search with actual query to get relevant spec chunks
    spec_candidates = db.similarity_search(query, k=30)
    
    spec_docs = [
        d for d in spec_candidates
        if any(x in d.metadata.get("source", "") for x in [
            "04_architecture.md",
            "02_Instruction.md",
            "01_formats.md",
            "08_alu_spec.md",
            "07_regfile_spec.md",
            "03_pipeline_stages.md",
            "09_hazard_control.md",
            "10_top_integration.md",
        ])
        and len(d.page_content.strip()) > 200  # filter empty title chunks
    ][:10]
    
    # DEBUG — remove after testing
    print(f"\nDEBUG spec_candidates top 10:")
    for d in spec_candidates[:10]:
        print(f"  {Path(d.metadata['source']).name} | {d.metadata['chunk_id']}")
    print(f"DEBUG spec_docs after filter: {len(spec_docs)}")

    # 2. HARDWARE PATTERN docs — design patterns and bug fixes
    # Fix:
    if any(x in query.lower() for x in ["decode", "alu", "control", "immediate"]):
        pattern_query = "always_comb unique case default combinational latch prevention"
    else:
        pattern_query = "always_ff pipeline register stall flush reset non-blocking"
    
    pattern_candidates = db.similarity_search(pattern_query, k=10)
    pattern_docs = [
        d for d in pattern_candidates
        if any(x in d.metadata.get("source", "") for x in [
            "05_verilog_design_patterns.md",
            "06_common_rtl_bugs_and_fixes.md",
        ])
        and len(d.page_content.strip()) > 200
    ][:2]

    # 3. RTL reference docs — reduce for decode queries, ibex decoder is noisy
    # Fix — also for alu_control:
    if any(x in query.lower() for x in ["decode", "instruction decode", "alu control"]):
        k_rtl = min(k_rtl, 2)

    rtl_docs = []
    if k_rtl > 0:
        rtl_candidates = db.similarity_search(query, k=k_rtl * 3)
        rtl_docs = [
            d for d in rtl_candidates
            if d.metadata.get("type") == "rtl"
        ][:k_rtl]

    docs = spec_docs + pattern_docs + rtl_docs
    docs = deduplicate_docs(docs)

    return docs


if __name__ == "__main__":
    query = "RV32I register file x0 hardwired read write port rd_addr reg_write write-before-read bypass rs1_addr rs2_addr always_comb"
    docs = retrieve_structured(query, k_rtl=3)

    for i, doc in enumerate(docs, start=1):
        print("=" * 80)
        print(f"Result {i}")
        print("Source:", doc.metadata["source"])
        print("Chunk ID:", doc.metadata["chunk_id"])
        print(doc.page_content[:1200])
        print()
