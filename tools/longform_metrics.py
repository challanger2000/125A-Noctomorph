#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import json
import math
from pathlib import Path

import numpy as np
import soundfile as sf


def db20(x: float) -> float:
    return -300.0 if x <= 0.0 or not math.isfinite(x) else 20.0 * math.log10(x)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("input", type=Path)
    ap.add_argument("--json", type=Path, required=True)
    ap.add_argument("--analysis-seconds", type=float, default=300.0)
    args = ap.parse_args()

    data, sr = sf.read(str(args.input), always_2d=True, dtype="float64")
    n = min(len(data), int(round(args.analysis_seconds * sr)))
    data = data[:n]
    if n <= 0:
        raise SystemExit("empty input")

    finite = bool(np.all(np.isfinite(data)))
    data = np.nan_to_num(data, nan=0.0, posinf=0.0, neginf=0.0)

    second = sr
    hashes = []
    sec_rms_db = []
    for start in range(0, n - second + 1, second):
        block = data[start:start + second]
        q = np.clip(np.rint(block * 32767.0), -32768, 32767).astype("<i2", copy=False)
        hashes.append(hashlib.sha256(q.tobytes()).hexdigest())
        rms = math.sqrt(float(np.mean(block * block)) + 1e-30)
        sec_rms_db.append(db20(rms))

    unique = len(set(hashes))
    duplicates = len(hashes) - unique

    minute_rms_db = []
    minute = 60 * sr
    for start in range(0, n, minute):
        block = data[start:min(n, start + minute)]
        if not len(block):
            continue
        rms = math.sqrt(float(np.mean(block * block)) + 1e-30)
        minute_rms_db.append(db20(rms))

    peak = float(np.max(np.abs(data)))
    rms = math.sqrt(float(np.mean(data * data)) + 1e-30)

    sec_arr = np.asarray(sec_rms_db, dtype=np.float64)
    result = {
        "path": str(args.input),
        "sample_rate": int(sr),
        "analysis_seconds": n / float(sr),
        "finite": finite,
        "peak_linear": peak,
        "peak_dbfs": db20(peak),
        "rms_dbfs": db20(rms),
        "one_second_windows": len(hashes),
        "unique_one_second_hashes": unique,
        "duplicate_one_second_hashes": duplicates,
        "second_rms_db_min": float(np.min(sec_arr)) if sec_arr.size else None,
        "second_rms_db_max": float(np.max(sec_arr)) if sec_arr.size else None,
        "second_rms_db_std": float(np.std(sec_arr)) if sec_arr.size else None,
        "minute_rms_db": minute_rms_db,
    }

    args.json.parent.mkdir(parents=True, exist_ok=True)
    args.json.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(json.dumps(result, indent=2, sort_keys=True))

    if not finite:
        return 2
    if peak > 0.892:
        return 3
    if rms < 1e-6:
        return 4
    if duplicates != 0:
        return 5
    if len(minute_rms_db) >= 5 and any(x < -75.0 for x in minute_rms_db[:5]):
        return 6
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
