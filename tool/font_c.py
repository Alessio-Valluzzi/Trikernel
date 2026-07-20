import gzip
import sys

filename = sys.argv[1]

with gzip.open(filename, "rb") as f:
    data = f.read()

# PSF1
if data[0] != 0x36 or data[1] != 0x04:
    print("Non è un font PSF1")
    exit(1)

charsize = data[3]

font = data[4:]

print("unsigned char font[] = {")

for b in font:
    print(f"0x{b:02x},", end=" ")

print("\n};")
print(f"\n#define FONT_HEIGHT {charsize}")
