#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
from pathlib import Path

import numpy as np
import soundfile as sf
from scipy import signal

TARGET_SR = 48000


def load_and_prepare(
    source: Path,
    start_s: float,
    duration_s: float,
    channels: int,
    peak_target: float,
) -> tuple[np.ndarray, int]:
    audio, sr = sf.read(str(source), always_2d=True, dtype="float64")
    if audio.size == 0:
        raise RuntimeError(f"empty source: {source}")

    if sr != TARGET_SR:
        g = math.gcd(int(sr), TARGET_SR)
        audio = signal.resample_poly(
            audio, TARGET_SR // g, int(sr) // g, axis=0)
        sr = TARGET_SR

    if channels == 1:
        audio = np.mean(audio, axis=1, keepdims=True)
    elif channels == 2:
        if audio.shape[1] == 1:
            audio = np.repeat(audio, 2, axis=1)
        else:
            audio = audio[:, :2]
    else:
        raise ValueError("channels must be 1 or 2")

    start = max(0, int(round(start_s * sr)))
    count = max(1, int(round(duration_s * sr)))
    if start >= len(audio):
        start = 0
    audio = audio[start:min(len(audio), start + count)]
    if len(audio) < 64:
        raise RuntimeError(f"segment too short: {source}")

    fade = min(len(audio) // 4, int(round(0.030 * sr)))
    if fade > 1:
        ramp = np.linspace(0.0, 1.0, fade, endpoint=False)[:, None]
        audio[:fade] *= ramp
        audio[-fade:] *= ramp[::-1]

    peak = float(np.max(np.abs(audio)))
    target = float(np.clip(peak_target, 0.01, 0.95))
    if peak > 1e-12:
        audio *= target / peak

    return np.ascontiguousarray(audio, dtype=np.float32), sr


def write_asset(source: Path, output: Path, **kwargs) -> None:
    audio, sr = load_and_prepare(source, **kwargs)
    output.parent.mkdir(parents=True, exist_ok=True)
    sf.write(str(output), audio, sr, subtype="PCM_16")
    print(
        f"{source.name} -> {output.name} "
        f"seconds={len(audio)/sr:.3f} channels={audio.shape[1]} "
        f"peak={float(np.max(np.abs(audio))):.4f}"
    )


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--world", type=Path, required=True)
    ap.add_argument("--texture", type=Path, required=True)
    ap.add_argument("--body-bright", type=Path, required=True)
    ap.add_argument("--body-deep", type=Path, required=True)
    ap.add_argument("--event", type=Path, required=True)
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()

    write_asset(
        args.world, args.out / "world.wav",
        start_s=5.0, duration_s=30.0, channels=2, peak_target=0.40)
    write_asset(
        args.texture, args.out / "texture.wav",
        start_s=10.0, duration_s=20.0, channels=2, peak_target=0.30)
    write_asset(
        args.body_bright, args.out / "body_bright.wav",
        start_s=0.0, duration_s=6.0, channels=1, peak_target=0.18)
    write_asset(
        args.body_deep, args.out / "body_deep.wav",
        start_s=0.0, duration_s=5.5, channels=1, peak_target=0.24)
    write_asset(
        args.event, args.out / "event.wav",
        start_s=0.0, duration_s=16.0, channels=2, peak_target=0.32)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
