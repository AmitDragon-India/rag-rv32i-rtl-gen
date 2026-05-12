import json
from pathlib import Path
from utils import (
    read_text_file,
    is_rtl_file,
    is_markdown_file,
    split_markdown_by_headers,
    split_rtl_semantically,
    chunk_large_text,
    enrich_markdown_chunk,
)

CORPUS_DIR = Path("corpus")
OUTPUT_FILE = Path("outputs/chunks.json")

# Cap RTL chunks per file to prevent large reference cores
# (e.g. PicoRV32) from dominating the index
MAX_RTL_CHUNKS_PER_FILE = 60


def chunk_file(path: Path):
    text = read_text_file(path)
    chunks = []

    if is_rtl_file(path):
        rtl_blocks = split_rtl_semantically(text)
        for block_id, block in enumerate(rtl_blocks):
            subchunks = chunk_large_text(block, max_chars=2200, overlap=250)
            for i, chunk in enumerate(subchunks):
                chunks.append({
                    "source": str(path),
                    "type": "rtl",
                    "chunk_id": f"{path.stem}_rtl_{block_id}_{i}",
                    "text": chunk
                })

        # Cap to prevent large reference RTL files dominating the index
        if len(chunks) > MAX_RTL_CHUNKS_PER_FILE:
            print(f"  NOTE: {path.name} produced {len(chunks)} chunks — capping at {MAX_RTL_CHUNKS_PER_FILE}")
            chunks = chunks[:MAX_RTL_CHUNKS_PER_FILE]

    elif is_markdown_file(path):
        sections = split_markdown_by_headers(text)
        if not sections:
            sections = [text]

        for section_id, section in enumerate(sections):
            enriched = enrich_markdown_chunk(section, path.stem)  # ← add this
            chunks.append({
                "source": str(path),
                "type": "markdown",
                "chunk_id": f"{path.stem}_sec_{section_id}",
                "text": enriched   # ← use enriched not section
            })

        # Warn if a markdown file produces suspiciously many chunks
        if len(chunks) > 60:
            print(f"  WARNING: {path.name} produced {len(chunks)} chunks — consider splitting the file")

    return chunks


def main():
    all_chunks = []
    files_processed = 0
    files_skipped = 0

    # Scan only supported extensions (.txt removed — not used in corpus)
    supported = {".md", ".v", ".sv"}

    for path in sorted(CORPUS_DIR.rglob("*")):
        if not path.is_file():
            continue
        if path.suffix.lower() not in supported:
            continue

        try:
            file_chunks = chunk_file(path)
            all_chunks.extend(file_chunks)
            print(f"  {path.name}: {len(file_chunks)} chunks")
            files_processed += 1
        except Exception as e:
            print(f"  WARNING: skipped {path.name} — {e}")
            files_skipped += 1

    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        json.dump(all_chunks, f, indent=2)

    print(f"\nDone: {files_processed} files processed, {files_skipped} skipped")
    print(f"Total chunks: {len(all_chunks)}")
    print(f"Saved to: {OUTPUT_FILE}")

    # Warn if any single source dominates the index
    source_counts = {}
    for chunk in all_chunks:
        src = Path(chunk["source"]).name
        source_counts[src] = source_counts.get(src, 0) + 1

    print("\nChunks per source file:")
    for src, count in sorted(source_counts.items(), key=lambda x: -x[1]):
        bar = "#" * min(count, 40)
        print(f"  {src:40s} {count:4d}  {bar}")


if __name__ == "__main__":
    main()
