#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
import subprocess
import tempfile
from pathlib import Path

import imageio_ffmpeg
import numpy as np
import soundfile as sf
from scipy import signal

TARGET_SR = 48000


def load_audio(path: Path) -> np.ndarray:
    try:
        audio, sr = sf.read(str(path), always_2d=True, dtype="float64")
    except sf.LibsndfileError:
        # Some Wikimedia OGG files use codecs that the Windows libsndfile
        # build cannot decode. Fall back to a bundled FFmpeg executable.
        with tempfile.TemporaryDirectory() as td:
            decoded = Path(td) / "decoded.wav"
            cmd = [
                imageio_ffmpeg.get_ffmpeg_exe(),
                "-v", "error",
                "-y",
                "-i", str(path),
                "-ar", str(TARGET_SR),
                "-ac", "2",
                "-c:a", "pcm_s24le",
                str(decoded),
            ]
            subprocess.run(cmd, check=True)
            audio, sr = sf.read(str(decoded), always_2d=True, dtype="float64")

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
    ap.add_argument("--mode", choices=("all", "expansion31", "expansion34", "expansion36", "expansion39", "expansion42", "expansion45", "expansion48", "expansion51"), default="all")
    args = ap.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    d = args.input_dir

    if args.mode == "expansion51":
        # Verified 48-source bank is restored first; derive three new scene classes.
        write(args.out, "world_thunderrain.wav", d/"world_thunderrain.ogg",
              start=0, duration=28, peak=.20, speed=.50, hp=30, lp=2800)
        write(args.out, "body_chapterbell.wav", d/"body_chapterbell.wav",
              start=1, duration=10, channels=1, peak=.13, speed=.38, hp=35, lp=3000)
        write(args.out, "texture_grain.wav", d/"texture_grain.ogg",
              start=0, duration=16, peak=.13, speed=.52, hp=180, lp=5200)
        return 0

    if args.mode == "expansion48":
        # Verified 45-source bank is restored first; derive only three new source classes.
        write(args.out, "world_forestair.wav", d/"world_forestair.ogg",
              start=22, duration=44, peak=.20, speed=.46, reverse=True, hp=35, lp=2600)
        write(args.out, "texture_rollingrattle.wav", d/"texture_rollingrattle.ogg",
              start=1, duration=20, peak=.13, speed=.50, hp=180, lp=5200)
        write(args.out, "texture_interiorhum.wav", d/"texture_interiorhum.ogg",
              start=2, duration=24, peak=.14, speed=.56, hp=45, lp=2200)
        return 0

    if args.mode == "expansion45":
        # Verified 42-source bank is restored first; derive only three new source classes.
        write(args.out, "texture_feedback.wav", d/"texture_feedback.ogg",
              start=0, duration=15, peak=.13, speed=.48, hp=180, lp=4200)
        write(args.out, "texture_organicrattle.wav", d/"texture_organicrattle.ogg",
              start=1, duration=12, peak=.12, speed=.42, hp=220, lp=5600)
        write(args.out, "event_hollowclatter.wav", d/"event_hollowclatter.ogg",
              start=0, duration=4.0, channels=1, peak=.16, speed=.58, hp=120, lp=4200)
        return 0

    if args.mode == "expansion42":
        # Verified 39-source bank is restored first; derive only three mineral/resonant assets.
        write(args.out, "texture_stonegrind.wav", d/"texture_stonegrind.ogg",
              start=5, duration=30, peak=.16, speed=.50, hp=100, lp=4600)
        write(args.out, "event_gravel.wav", d/"event_gravel.ogg",
              start=0, duration=8.0, channels=1, peak=.17, speed=.58, hp=120, lp=4200)
        write(args.out, "body_glassring.wav", d/"body_glassring.ogg",
              start=2, duration=8.0, channels=1, peak=.14, speed=.42, hp=90, lp=4200)
        return 0

    if args.mode == "expansion39":
        # Verified 36-source bank is restored first; derive only three new body/material assets.
        write(args.out, "texture_woodcreak.wav", d/"texture_woodcreak.ogg",
              start=8, duration=28, peak=.16, speed=.46, hp=90, lp=3600)
        write(args.out, "texture_rubber.wav", d/"texture_rubber.ogg",
              start=0, duration=10, peak=.14, speed=.42, hp=180, lp=4200)
        write(args.out, "event_woodknock.wav", d/"event_woodknock.ogg",
              start=0, duration=6.0, channels=1, peak=.17, speed=.52, hp=90, lp=3200)
        return 0

    if args.mode == "expansion36":
        # Verified 34-source bank is restored first; derive only the two new assets.
        write(args.out, "texture_flowwater.wav", d/"texture_flowwater.ogg",
              start=2, duration=18, peak=.16, speed=.62, hp=80, lp=4200)
        write(args.out, "event_flint.wav", d/"event_flint.ogg",
              start=0, duration=10, channels=1, peak=.18, speed=.55, hp=180, lp=5200)
        return 0

    if args.mode == "expansion34":
        # The verified 31-source prepared bank is restored by CI first.
        # Only derive the three new non-industrial expansion assets here.
        write(args.out, "texture_rustle.wav", d/"texture_rustle.ogg",
              start=6, duration=32, peak=.16, speed=.58, hp=180, lp=5200)
        write(args.out, "world_waves.wav", d/"world_waves.ogg",
              start=40, duration=44, peak=.22, speed=.52, hp=35, lp=2600)
        write(args.out, "body_bloop.wav", d/"body_bloop.ogg",
              start=0, duration=9, channels=1, peak=.14, speed=.35, hp=25, lp=1800)
        return 0

    if args.mode == "expansion31":
        # The verified 25-source prepared bank is restored by CI first.
        # Only derive the six new 31-source expansion assets here.
        write(args.out, "world_stationtunnel.wav", d/"world_stationtunnel.ogg",
              start=18, duration=42, peak=.24, speed=.62, hp=45, lp=5200)
        write(args.out, "world_metro.wav", d/"world_metro.ogg",
              start=8, duration=34, peak=.24, speed=.66, hp=50, lp=5800)
        write(args.out, "world_trainplatform.wav", d/"world_trainplatform.ogg",
              start=28, duration=38, peak=.22, speed=.58, hp=45, lp=5200)
        write(args.out, "texture_waterpressure.wav", d/"texture_waterpressure.ogg",
              start=36, duration=30, peak=.18, speed=.52, hp=70, lp=4200)
        write(args.out, "texture_rain.wav", d/"texture_rain.ogg",
              start=5, duration=36, peak=.17, speed=.70, hp=120, lp=6500)
        write(args.out, "body_whirly.wav", d/"body_whirly.ogg",
              start=0, duration=7, channels=1, peak=.16, speed=.44, hp=90, lp=3600)
        return 0

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
    write(args.out, "world_steamrod.wav", d/"world_steamrod.ogg",
          start=12, duration=34, peak=.28, speed=.72, hp=40, lp=6500)
    write(args.out, "world_pneumatic.wav", d/"world_pneumatic.ogg",
          start=10, duration=32, peak=.27, speed=.64, hp=40, lp=7200)
    write(args.out, "world_waterpump.wav", d/"world_waterpump.ogg",
          start=5, duration=30, peak=.26, speed=.70, hp=35, lp=5400)
    write(args.out, "world_stationtunnel.wav", d/"world_stationtunnel.ogg",
          start=18, duration=42, peak=.24, speed=.62, hp=45, lp=5200)
    write(args.out, "world_metro.wav", d/"world_metro.ogg",
          start=8, duration=34, peak=.24, speed=.66, hp=50, lp=5800)
    write(args.out, "world_trainplatform.wav", d/"world_trainplatform.ogg",
          start=28, duration=38, peak=.22, speed=.58, hp=45, lp=5200)

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
    write(args.out, "texture_millbelt.wav", d/"texture_millbelt.ogg",
          start=8, duration=28, peak=.20, speed=.76, hp=110, lp=8500)
    write(args.out, "texture_steelcoiler.wav", d/"texture_steelcoiler.ogg",
          start=12, duration=28, peak=.22, speed=.68, hp=90, lp=8500)
    write(args.out, "texture_chisel.wav", d/"texture_chisel.ogg",
          start=2, duration=24, peak=.18, speed=.52, hp=140, lp=6000)
    write(args.out, "texture_planer.wav", d/"texture_planer.ogg",
          start=18, duration=34, peak=.20, speed=.72, hp=90, lp=7600)
    write(args.out, "texture_waterpressure.wav", d/"texture_waterpressure.ogg",
          start=36, duration=30, peak=.18, speed=.52, hp=70, lp=4200)
    write(args.out, "texture_rain.wav", d/"texture_rain.ogg",
          start=5, duration=36, peak=.17, speed=.70, hp=120, lp=6500)

    # BODY exciters are never mixed directly; they only excite the modal body.
    write(args.out, "body_glass.wav", d/"body_glass.ogg",
          start=0, duration=7, channels=1, peak=.17, speed=.62, hp=120, lp=6500)
    write(args.out, "body_gong.wav", d/"body_gong.ogg",
          start=0, duration=6, channels=1, peak=.20, speed=.48, lp=3200)
    write(args.out, "body_whirly.wav", d/"body_whirly.ogg",
          start=0, duration=7, channels=1, peak=.16, speed=.44, hp=90, lp=3600)

    # Sparse event pool. No event is forced at Note-On.
    write(args.out, "event_metaldoor.wav", d/"event_metaldoor.ogg",
          start=0, duration=5.5, peak=.22, speed=.68, lp=6000)
    write(args.out, "event_cabinet.wav", d/"event_cabinet.ogg",
          start=0, duration=12, peak=.20, speed=.58, lp=6500)
    write(args.out, "event_thud.wav", d/"event_thud.ogg",
          start=0, duration=.45, channels=1, peak=.24, speed=.50, lp=2500)
    write(args.out, "event_peters.wav", d/"event_peters.oga",
          start=4, duration=13, peak=.18, speed=.55, hp=35, lp=4200)
    write(args.out, "event_handlecreak.wav", d/"event_handlecreak.ogg",
          start=0, duration=5.0, peak=.18, speed=.55, hp=120, lp=5500)
    write(args.out, "event_hinge.wav", d/"event_hinge.ogg",
          start=0, duration=7.3, peak=.18, speed=.48, hp=120, lp=5200)
    write(args.out, "event_metalthump.wav", d/"event_metalthump.ogg",
          start=0, duration=1.7, peak=.22, speed=.55, lp=3200)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
