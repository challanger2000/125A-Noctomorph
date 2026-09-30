#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

ASSETS = [
    ("world_paper.wav", 0, True, 1.0),
    ("world_eccentric.wav", 1, True, 1.0),
    ("world_fence.wav", 2, True, 1.0),
    ("texture_packing.wav", 3, True, 1.0),
    ("texture_saw.wav", 4, True, 1.0),
    ("texture_brush.wav", 5, True, 1.0),
    ("world_wind.wav", 6, True, 1.0),
    ("world_ambient.wav", 7, True, 1.0),
    ("texture_choir.wav", 8, True, 1.0),
    ("body_glass.wav", 9, False, 1.0),
    ("body_gong.wav", 10, False, 0.09),
    ("event_metaldoor.wav", 11, False, 1.0),
    ("event_cabinet.wav", 12, False, 1.0),
    ("event_thud.wav", 13, False, 1.0),
    ("event_peters.wav", 14, False, 1.0),
]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("asset_dir", type=Path)
    ap.add_argument("out_dir", type=Path)
    args = ap.parse_args()
    args.out_dir.mkdir(parents=True, exist_ok=True)

    rows = []
    for index, (name, asset_id, loop, gain) in enumerate(ASSETS):
        path = (args.asset_dir / name).resolve()
        if not path.is_file():
            raise SystemExit(f"missing asset: {path}")
        rows.append((4000 + index, name, asset_id, loop, gain, path))

    header = args.out_dir / "noctomorph_assets_generated.h"
    rc = args.out_dir / "noctomorph_assets_generated.rc"

    with header.open("w", encoding="utf-8") as f:
        f.write("#pragma once\n#include <cstddef>\n\nnamespace Noctomorph {\n")
        f.write(
            "struct EmbeddedNoctomorphAssetMeta { "
            "int resourceId; const char* name; int assetId; bool loop; "
            "float excitationGain; };\n"
        )
        f.write(
            "inline constexpr EmbeddedNoctomorphAssetMeta "
            "kEmbeddedNoctomorphAssets[] = {\n"
        )
        for rid, name, asset_id, loop, gain, _ in rows:
            f.write(
                f'    {{{rid}, "{name}", {asset_id}, '
                f'{"true" if loop else "false"}, {gain:.6f}f}},\n'
            )
        f.write("};\n")
        f.write(
            "inline constexpr std::size_t kEmbeddedNoctomorphAssetCount = "
            "sizeof(kEmbeddedNoctomorphAssets) / "
            "sizeof(kEmbeddedNoctomorphAssets[0]);\n}\n"
        )

    with rc.open("w", encoding="utf-8") as f:
        for rid, _, _, _, _, path in rows:
            rp = str(path).replace("\\", "/")
            f.write(f'{rid} RCDATA "{rp}"\n')

    print(f"generated {len(rows)} embedded assets")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
