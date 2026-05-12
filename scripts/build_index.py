import json
from pathlib import Path
from langchain_community.vectorstores import FAISS
from langchain_core.documents import Document
from langchain_community.embeddings import HuggingFaceEmbeddings

CHUNKS_FILE = Path("outputs/chunks.json")
VECTORSTORE_DIR = Path("vectorstore/faiss_index")


def main():
    with open(CHUNKS_FILE, "r", encoding="utf-8") as f:
        raw_chunks = json.load(f)

    print(f"Loaded {len(raw_chunks)} chunks from {CHUNKS_FILE}")

    docs = []
    for item in raw_chunks:
        docs.append(
            Document(
                page_content=item["text"],
                metadata={
                    "source": item["source"],
                    "chunk_id": item["chunk_id"],
                    "type": item["type"],
                },
            )
        )

    # BAAI/bge-base-en-v1.5 gives significantly better technical/code retrieval
    # than all-MiniLM-L6-v2 which is a general English sentence model
    embeddings = HuggingFaceEmbeddings(
        model_name="BAAI/bge-base-en-v1.5"
    )

    print("Building FAISS index...")
    db = FAISS.from_documents(docs, embeddings)

    VECTORSTORE_DIR.mkdir(parents=True, exist_ok=True)
    db.save_local(str(VECTORSTORE_DIR))

    print(f"Indexed {len(docs)} chunks into {VECTORSTORE_DIR}")

    # Show breakdown by source type
    type_counts = {}
    source_counts = {}
    for item in raw_chunks:
        t = item["type"]
        src = Path(item["source"]).name
        type_counts[t] = type_counts.get(t, 0) + 1
        source_counts[src] = source_counts.get(src, 0) + 1

    print("\nChunks by type:")
    for t, count in sorted(type_counts.items()):
        print(f"  {t:12s}: {count}")

    print("\nChunks by source:")
    for src, count in sorted(source_counts.items(), key=lambda x: -x[1]):
        print(f"  {src:45s}: {count}")


if __name__ == "__main__":
    main()
