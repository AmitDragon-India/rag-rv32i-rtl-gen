import re
import sys
from pathlib import Path

if len(sys.argv) != 2:
    print("Usage: python3 convert_hex_preserve_addr.py input-relocated.hex")
    sys.exit(1)

inp = Path(sys.argv[1])
outp = inp.with_name(inp.stem + "_word.hex")

lines = inp.read_text().splitlines()

byte_addr = 0
buf = []
out = []

def flush_buf():
    global byte_addr, buf, out
    while buf:
        while len(buf) < 4:
            buf.append(0)
        word = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24)
        out.append((byte_addr >> 2, word))
        byte_addr += 4
        buf = buf[4:]

for line in lines:
    line = line.strip()
    if not line:
        continue

    if line.startswith("@"):
        flush_buf()
        byte_addr = int(line[1:], 16)
        continue

    for b in re.split(r"\s+", line):
        if b:
            buf.append(int(b, 16))
            if len(buf) == 4:
                flush_buf()

flush_buf()

with outp.open("w") as f:
    current_addr = None
    for addr, word in out:
        if addr != current_addr:
            f.write(f"@{addr:08x}\n")
            current_addr = addr
        f.write(f"{word:08x}\n")
        current_addr += 1

print("Generated:", outp)