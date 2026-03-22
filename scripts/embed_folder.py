import os
import sys

def embed_folder(src_dir, out_cpp):
    if not os.path.exists(src_dir):
        print(f"Warning: {src_dir} not found. Generating empty loader.")
        with open(out_cpp, "w") as f:
            f.write('#include "PakInterface.h"\n')
            f.write('void LoadEmbeddedProperties(PakInterface* p) {}\n')
        return

    files = []
    for root, _, filenames in os.walk(src_dir):
        for filename in filenames:
            rel_path = os.relpath(os.path.join(root, filename), os.path.dirname(src_dir))
            # Normalize to forward slashes for cross-platform matching
            rel_path = rel_path.replace("\\", "/")
            files.append((rel_path, os.path.join(root, filename)))

    with open(out_cpp, "w") as f:
        f.write('#include <cstdint>\n')
        f.write('#include "PakInterface.h"\n\n')
        
        # Write data arrays
        for i, (rel_path, full_path) in enumerate(files):
            var_name = f"data_{i}"
            f.write(f"static const uint8_t {var_name}[] = {{\n")
            with open(full_path, "rb") as bf:
                data = bf.read()
                f.write(", ".join(map(hex, data)))
            f.write("\n};\n\n")

        # Write loader function
        f.write('void LoadEmbeddedProperties(PakInterface* p) {\n')
        for i, (rel_path, _) in enumerate(files):
            var_name = f"data_{i}"
            # Load with size from sizeof()
            f.write(f'    p->AddFileMemory("{rel_path}", (void*){var_name}, sizeof({var_name}));\n')
        f.write('}\n')

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python embed_folder.py <src_dir> <out_cpp>")
        sys.exit(1)
    embed_folder(sys.argv[1], sys.argv[2])
