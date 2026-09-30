#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
from pathlib import Path

import numpy as np
import soundfile as sf
from scipy import signal

TARGET_SR = 48000


def load_audio(path: Path) -> np.ndarray:
    audio, sr = sf.read(str(path), always_2d=True, dtype="float64")
    if audio.size == 0:
        raise RuntimeError(f"empty source: {path}")
    if sr != TARGET_SR:
        g = math.gcd(int(sr), TARGET_SR)
        audio = signal.resample_poly(audio, TARGET_SR // g, int(sr) // g, axis=0)
    return audio


def prepare(
    path: Path,
    *,
    start: float,
    duration: float,
    channels: int = 2,
    peak: float = 0.30,
    speed: float = 1.0,
    reverse: bool = False,
    hp: float | None = None,
    lp: float | None = None,
) -> np.ndarray:
    audio = load_audio(path)

    begin = max(0, int(round(start * TARGET_SR)))
    count = max(64, int(round(duration * TARGET_SR)))
    if begin >= len(audio):
        begin = 0
    audio = audio[begin:min(len(audio), begin + count)]

    if channels == 1:
        audio = np.mean(audio, axis=1, keepdims=True)
    elif audio.shape[1] == 1:
        audio = np.repeat(audio, 2, axis=1)
    else:
        audio = audio[:, :2]

    if reverse:
        audio = audio[::-1].copy()

    speed = float(np.clip(speed, 0.25, 2.0))
    if abs(speed - 1.0) > 1e-6:
        target_len = max(64, int(round(len(audio) / speed)))
        audio = signal.resample(audio, target_len, axis=0)

    nyq = TARGET_SR * 0.5
    if hp is not None and lp is not None:
        sos = signal.butter(3, [hp / nyq, lp / nyq], btype="bandpass", output="sos")
        audio = signal.sosfilt(sos, audio, axis=0)
    elif hp is not None:
        sos = signal.butter(3, hp / nyq, btype="highpass", output="sos")
        audio = signal.sosfilt(sos, audio, axis=0)
    elif lp is not None:
        sos = signal.butter(3, lp / nyq, btype="lowpass", output="sos")
        audio = signal.sosfilt(sos, audio, axis=0)

    fade = min(len(audio) // 5, int(round(0.150 * TARGET_SR)))
    if fade > 1:
        ramp = np.linspace(0.0, 1.0, fade, endpoint=False)[:, None]
        audio[:fade] *= ramp
        audio[-fade:] *= ramp[::-1]

    p = float(np.max(np.abs(audio)))
    if p > 1e-12:
        audio *= float(np.clip(peak, 0.01, 0.80)) / p

    return np.ascontiguousarray(audio, dtype=np.float32)


def write(out_dir: Path, name: str, source: Path, **kwargs) -> None:
    audio = prepare(source, **kwargs)
    path = out_dir / name
    sf.write(str(path), audio, TARGET_SR, subtype="PCM_16")
    print(
        f"{source.name} -> {name} "
        f"seconds={len(audio)/TARGET_SR:.2f} ch={audio.shape[1]} "
        f"peak={np.max(np.abs(audio)):.3f}"
    )


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--input-dir", type=Path, required=True)
    ap.add_argument("--out", type=Path, required=True)
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    d = args.input_dir

    # Long WORLD beds: deliberately different physical identities.
    write(args.out, "world_paper.wav", d/"world_paper.ogg",
          start=8, duration=36, peak=.34, speed=.92, lp=9000)
    write(args.out, "world_eccentric.wav", d/"world_eccentric.ogg",
          start=6, duration=32, peak=.30, speed=.78, lp=7000)
    write(args.out, "world_fence.wav", d/"world_fence.ogg",
          start=5, duration=34, peak=.30, speed=.88, hp=45, lp=9000)
    write(args.out, "world_wind.wav", d/"world_wind.ogg",
          start=10, duration=40, peak=.28, speed=.68, hp=40, lp=5200)
    write(args.out, "world_ambient.wav", d/"world_ambient.ogg",
          start=1, duration=32, peak=.26, speed=.58, lp=3200)

    # TEXTURE beds: more detail, lower level, often strongly transformed.
    write(args.out, "texture_packing.wav", d/"texture_packing.ogg",
          start=10, duration=28, peak=.24, speed=1.05, hp=100, lp=10000)
    write(args.out, "texture_saw.wav", d/"texture_saw.ogg",
          start=12, duration=30, peak=.22, speed=.72, hp=180, lp=9000)
    write(args.out, "texture_brush.wav", d/"texture_brush.ogg",
          start=1, duration=26, peak=.20, speed=.60, hp=250, lp=7000)
    write(args.out, "texture_choir.wav", d/"texture_choir.ogg",
          start=32, duration=22, peak=.15, speed=.36, reverse=True,
          hp=120, lp=1800)

    # BODY exciters are never mixed directly; they only excite the modal body.
    write(args.out, "body_glass.wav", d/"body_glass.ogg",
          start=0, duration=7, channels=1, peak=.17, speed=.62, hp=120, lp=6500)
    write(args.out, "body_gong.wav", d/"body_gong.ogg",
          start=0, duration=6, channels=1, peak=.20, speed=.48, lp=3200)

    # Sparse event pool. No event is forced at Note-On.
    write(args.out, "event_metaldoor.wav", d/"event_metaldoor.ogg",
          start=0, duration=5.5, peak=.22, speed=.68, lp=6000)
    write(args.out, "event_cabinet.wav", d/"event_cabinet.ogg",
          start=0, duration=12, peak=.20, speed=.58, lp=6500)
    write(args.out, "event_thud.wav", d/"event_thud.ogg",
          start=0, duration=.45, channels=1, peak=.24, speed=.50, lp=2500)
    write(args.out, "event_peters.wav", d/"event_peters.oga",
          start=4, duration=13, peak=.18, speed=.55, hp=35, lp=4200)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
