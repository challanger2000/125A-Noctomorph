#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

ASSETS = [
    ("world.wav", 0, True, 1.0),
    ("texture.wav", 1, True, 1.0),
    ("body_bright.wav", 2, False, 1.0),
    ("body_deep.wav", 3, False, 0.075),
    ("event.wav", 4, False, 1.0),
]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("asset_dir", type=Path)
    ap.add_argument("out_dir", type=Path)
    args = ap.parse_args()
    args.out_dir.mkdir(parents=True, exist_ok=True)

    rows = []
    for index, (name, role, loop, gain) in enumerate(ASSETS):
        path = (args.asset_dir / name).resolve()
        if not path.is_file():
            raise SystemExit(f"missing asset: {path}")
        rows.append((4000 + index, name, role, loop, gain, path))

    header = args.out_dir / "noctomorph_assets_generated.h"
    rc = args.out_dir / "noctomorph_assets_generated.rc"

    with header.open("w", encoding="utf-8") as f:
        f.write("#pragma once\n")
        f.write("#include <cstddef>\n\n")
        f.write("namespace Noctomorph {\n")
        f.write(
            "struct EmbeddedNoctomorphAssetMeta { "
            "int resourceId; const char* name; int role; bool loop; "
            "float excitationGain; };\n"
        )
        f.write(
            "inline constexpr EmbeddedNoctomorphAssetMeta "
            "kEmbeddedNoctomorphAssets[] = {\n"
        )
        for rid, name, role, loop, gain, _ in rows:
            f.write(
                f'    {{{rid}, "{name}", {role}, '
                f'{"true" if loop else "false"}, {gain:.6f}f}},\n'
            )
        f.write("};\n")
        f.write(
            "inline constexpr std::size_t kEmbeddedNoctomorphAssetCount = "
            "sizeof(kEmbeddedNoctomorphAssets) / "
            "sizeof(kEmbeddedNoctomorphAssets[0]);\n"
        )
        f.write("}\n")

    with rc.open("w", encoding="utf-8") as f:
        for rid, _, _, _, _, path in rows:
            rp = str(path).replace("\\", "/")
            f.write(f'{rid} RCDATA "{rp}"\n')

    print(f"generated {len(rows)} embedded assets")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
