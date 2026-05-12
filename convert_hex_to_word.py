from pathlib import Path
import sys
import re

if len(sys.argv) != 3:
    print("Usage: python convert_hex_to_word.py input_byte.hex output_word.hex")
    sys.exit(1)

inp = Path(sys.argv[1])
out = Path(sys.argv[2])

tokens = []

for line in inp.read_text().splitlines():
    line = line.strip()
    if not line:
        continue
    if line.startswith("@"):
        continue

    parts = re.split(r"\s+", line)
    for p in parts:
        if p:
            tokens.append(int(p, 16))

words = []

for i in range(0, len(tokens), 4):
    if i + 3 >= len(tokens):
        break

    b0 = tokens[i]
    b1 = tokens[i + 1]
    b2 = tokens[i + 2]
    b3 = tokens[i + 3]

    # little-endian byte packing
    word = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24)
    words.append(f"{word:08x}")

out.write_text("\n".join(words) + "\n")
print(f"Wrote {len(words)} words to {out}")