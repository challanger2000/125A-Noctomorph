#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
from pathlib import Path

import numpy as np
import soundfile as sf
from scipy import signal


def read_audio(path: Path):
    data, sr = sf.read(str(path), always_2d=True, dtype="float64")
    return data, int(sr)


def resample_if_needed(data: np.ndarray, src_sr: int, dst_sr: int) -> np.ndarray:
    if src_sr == dst_sr:
        return data
    g = math.gcd(src_sr, dst_sr)
    up = dst_sr // g
    down = src_sr // g
    return signal.resample_poly(data, up, down, axis=0)


def decode_ir(ir: np.ndarray, channel: int) -> np.ndarray:
    if ir.shape[1] == 1:
        return ir[:, 0]
    channel = max(0, min(channel, ir.shape[1] - 1))
    return ir[:, channel]


def decode_bformat_stereo(ir: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    """Simple engineering stereo decode for FuMa-style W/X/Y/Z B-format.

    This is intentionally a listening/measurement probe, not the final
    production decoder. It preserves directional differences instead of
    collapsing the measured field to W-only mono.
    """
    if ir.shape[1] < 3:
        mono = decode_ir(ir, 0)
        return mono, mono

    w = ir[:, 0]
    x = ir[:, 1]
    y = ir[:, 2]

    left = w + 0.55 * x + 0.35 * y
    right = w - 0.55 * x + 0.35 * y

    peak = max(float(np.max(np.abs(left))), float(np.max(np.abs(right))), 1e-12)
    left = left / peak
    right = right / peak
    return left, right


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("input", type=Path)
    ap.add_argument("ir", type=Path)
    ap.add_argument("output", type=Path)
    ap.add_argument("--wet", type=float, default=1.0)
    ap.add_argument("--ir-channel", type=int, default=0,
                    help="B-format/default channel to use as mono convolution kernel")
    ap.add_argument("--tail-seconds", type=float, default=0.0)
    ap.add_argument("--bformat-stereo", action="store_true",
                    help="decode W/X/Y B-format to distinct left/right kernels")
    ap.add_argument("--ceiling-dbfs", type=float, default=-1.0)
    args = ap.parse_args()

    dry, sr = read_audio(args.input)
    ir, ir_sr = read_audio(args.ir)
    ir = resample_if_needed(ir, ir_sr, sr)
    if args.bformat_stereo:
        kernel_l, kernel_r = decode_bformat_stereo(ir)
    else:
        kernel = decode_ir(ir, args.ir_channel)
        peak_ir = float(np.max(np.abs(kernel))) if kernel.size else 0.0
        if peak_ir <= 1e-12:
            raise SystemExit("IR is silent")
        kernel = kernel / peak_ir
        kernel_l = kernel
        kernel_r = kernel

    tail = max(0, int(round(args.tail_seconds * sr)))
    out_len = dry.shape[0] + max(kernel_l.shape[0], kernel_r.shape[0]) - 1
    if tail > 0:
        out_len = min(out_len, dry.shape[0] + tail)

    wet = np.zeros((out_len, 2), dtype=np.float64)
    dry_l = dry[:, 0]
    dry_r = dry[:, 1] if dry.shape[1] > 1 else dry[:, 0]
    wet[:, 0] = signal.fftconvolve(dry_l, kernel_l, mode="full")[:out_len]
    wet[:, 1] = signal.fftconvolve(dry_r, kernel_r, mode="full")[:out_len]

    dry_pad = np.zeros_like(wet)
    dry_pad[:dry.shape[0], 0] = dry[:, 0]
    dry_pad[:dry.shape[0], 1] = dry[:, 1] if dry.shape[1] > 1 else dry[:, 0]

    wet_amount = float(np.clip(args.wet, 0.0, 1.0))
    mixed = (1.0 - wet_amount) * dry_pad + wet_amount * wet

    # Energy-normalize conservatively relative to the dry render before the
    # final safety ceiling. This is a listening/measurement probe, not the
    # final realtime convolution architecture.
    dry_rms = math.sqrt(float(np.mean(dry_pad * dry_pad)) + 1e-30)
    wet_rms = math.sqrt(float(np.mean(mixed * mixed)) + 1e-30)
    if wet_rms > 0 and dry_rms > 0:
        target = dry_rms * (0.85 + 0.25 * wet_amount)
        mixed *= min(1.0, target / wet_rms)

    ceiling = 10.0 ** (args.ceiling_dbfs / 20.0)
    peak = float(np.max(np.abs(mixed))) if mixed.size else 0.0
    if peak > ceiling and peak > 0:
        mixed *= ceiling / peak

    args.output.parent.mkdir(parents=True, exist_ok=True)
    sf.write(str(args.output), mixed.astype(np.float32), sr, subtype="PCM_24")

    print(f"input_sr={sr}")
    print(f"ir_sr={ir_sr}")
    print(f"ir_channels={ir.shape[1]}")
    print(f"kernel_channel={args.ir_channel}")
    print(f"bformat_stereo={args.bformat_stereo}")
    print(f"wet={wet_amount}")
    print(f"output_seconds={mixed.shape[0]/sr:.6f}")
    print(f"output_peak={float(np.max(np.abs(mixed))):.9f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
