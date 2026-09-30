#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path

from analyze_audio import analyze


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("root", type=Path)
    ap.add_argument("--json", type=Path, required=True)
    ap.add_argument("--csv", type=Path, required=True)
    args = ap.parse_args()

    files = sorted(p for p in args.root.rglob("*.wav") if p.is_file())
    if not files:
        raise SystemExit("No WAV files found")

    rows = []
    full = []
    for path in files:
        result = analyze(path, True)
        full.append(result)
        ir = result.get("impulse_response", {})
        channels = result.get("impulse_response_channels", [])
        ch60 = [
            ch.get("decay_to_minus60_seconds")
            for ch in channels
            if ch.get("decay_to_minus60_seconds") is not None
        ]
        rows.append({
            "file": str(path.relative_to(args.root)).replace("\\", "/"),
            "sha256": result["sha256"],
            "sample_rate": result["sample_rate"],
            "channels": result["channels"],
            "duration_seconds": result["duration_seconds"],
            "direct_arrival_seconds_mono": ir.get("direct_arrival_seconds"),
            "decay_minus20_seconds_mono": ir.get("decay_to_minus20_seconds"),
            "decay_minus30_seconds_mono": ir.get("decay_to_minus30_seconds"),
            "decay_minus60_seconds_mono": ir.get("decay_to_minus60_seconds"),
            "min_channel_minus60_seconds": min(ch60) if ch60 else None,
            "max_channel_minus60_seconds": max(ch60) if ch60 else None,
            "spectral_centroid_hz": result["spectral_centroid_hz"],
            "peak_dbfs": result["peak_dbfs"],
        })

    args.json.parent.mkdir(parents=True, exist_ok=True)
    args.csv.parent.mkdir(parents=True, exist_ok=True)

    args.json.write_text(json.dumps(full, indent=2, sort_keys=True) + "\n", encoding="utf-8")

    with args.csv.open("w", newline="", encoding="utf-8") as fh:
        writer = csv.DictWriter(fh, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)

    print(f"Measured {len(files)} WAV files")
    if rows:
        vals = [r["decay_minus60_seconds_mono"] for r in rows if r["decay_minus60_seconds_mono"] is not None]
        if vals:
            print(f"mono_decay_minus60_min={min(vals):.6f}")
            print(f"mono_decay_minus60_max={max(vals):.6f}")
            print(f"mono_decay_minus60_mean={sum(vals)/len(vals):.6f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
