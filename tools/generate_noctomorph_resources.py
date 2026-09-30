#!/usr/bin/env python3
from __future__ import annotations

import argparse
from pathlib import Path

ASSETS = [
    ("world_paper.wav", 0, False, 1.0),
    ("world_eccentric.wav", 1, False, 1.0),
    ("world_fence.wav", 2, False, 1.0),
    ("texture_packing.wav", 3, False, 1.0),
    ("texture_saw.wav", 4, False, 1.0),
    ("texture_brush.wav", 5, False, 1.0),
    ("world_wind.wav", 6, False, 1.0),
    ("world_ambient.wav", 7, False, 1.0),
    ("texture_choir.wav", 8, False, 1.0),
    ("body_glass.wav", 9, False, 1.0),
    ("body_gong.wav", 10, False, 0.09),
    ("event_metaldoor.wav", 11, False, 1.0),
    ("event_cabinet.wav", 12, False, 1.0),
    ("event_thud.wav", 13, False, 1.0),
    ("event_peters.wav", 14, False, 1.0),
    ("world_steamrod.wav", 15, False, 1.0),
    ("world_pneumatic.wav", 16, False, 1.0),
    ("world_waterpump.wav", 17, False, 1.0),
    ("texture_millbelt.wav", 18, False, 1.0),
    ("texture_steelcoiler.wav", 19, False, 1.0),
    ("texture_chisel.wav", 20, False, 1.0),
    ("texture_planer.wav", 21, False, 1.0),
    ("event_handlecreak.wav", 22, False, 1.0),
    ("event_hinge.wav", 23, False, 1.0),
    ("event_metalthump.wav", 24, False, 1.0),
    ("world_stationtunnel.wav", 25, False, 1.0),
    ("world_metro.wav", 26, False, 1.0),
    ("world_trainplatform.wav", 27, False, 1.0),
    ("texture_waterpressure.wav", 28, False, 1.0),
    ("texture_rain.wav", 29, False, 1.0),
    ("body_whirly.wav", 30, False, 0.35),
    ("texture_rustle.wav", 31, False, 1.0),
    ("world_waves.wav", 32, False, 1.0),
    ("body_bloop.wav", 33, False, 0.22),
    ("texture_flowwater.wav", 34, False, 1.0),
    ("event_flint.wav", 35, False, 1.0),
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
