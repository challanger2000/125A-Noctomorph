#!/usr/bin/env python3
"""
Noctomorph audio measurement tool.

Usage:
    python tools/analyze_audio.py input.wav [--json out.json]

The tool is intentionally analysis-only: it does not modify source audio.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
from pathlib import Path

import numpy as np
import soundfile as sf
from scipy import signal


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def db20(x: float, floor_db: float = -300.0) -> float:
    if not math.isfinite(x) or x <= 0.0:
        return floor_db
    return 20.0 * math.log10(x)


def spectral_metrics(mono: np.ndarray, sr: int) -> dict:
    if mono.size < 8:
        return {
            "spectral_centroid_hz": 0.0,
            "energy_f10_hz": 0.0,
            "energy_f50_hz": 0.0,
            "energy_f90_hz": 0.0,
            "low_energy_ratio_lt120hz": 0.0,
            "high_energy_ratio_gt8khz": 0.0,
            "dominant_peaks_hz": [],
        }

    nperseg = min(65536, max(2048, 2 ** int(math.floor(math.log2(min(mono.size, 65536))))))
    freqs, psd = signal.welch(
        mono.astype(np.float64, copy=False),
        fs=sr,
        window="hann",
        nperseg=nperseg,
        noverlap=nperseg // 2,
        scaling="spectrum",
    )
    power = np.maximum(psd, 0.0)
    total = float(np.sum(power))
    if total <= 1e-30:
        return {
            "spectral_centroid_hz": 0.0,
            "energy_f10_hz": 0.0,
            "energy_f50_hz": 0.0,
            "energy_f90_hz": 0.0,
            "low_energy_ratio_lt120hz": 0.0,
            "high_energy_ratio_gt8khz": 0.0,
            "dominant_peaks_hz": [],
        }

    centroid = float(np.sum(freqs * power) / total)
    cdf = np.cumsum(power) / total

    def qfreq(q: float) -> float:
        i = int(np.searchsorted(cdf, q, side="left"))
        i = min(max(i, 0), len(freqs) - 1)
        return float(freqs[i])

    low_ratio = float(np.sum(power[freqs < 120.0]) / total)
    high_ratio = float(np.sum(power[freqs > 8000.0]) / total)

    min_distance_bins = max(1, int(12.0 / max(freqs[1] - freqs[0], 1e-9)))
    peaks, props = signal.find_peaks(
        10.0 * np.log10(power + 1e-30),
        prominence=6.0,
        distance=min_distance_bins,
    )
    if peaks.size:
        order = peaks[np.argsort(power[peaks])[::-1]]
        top = sorted(float(freqs[i]) for i in order[:12] if 15.0 <= freqs[i] <= min(20000.0, sr / 2.0))
    else:
        top = []

    return {
        "spectral_centroid_hz": centroid,
        "energy_f10_hz": qfreq(0.10),
        "energy_f50_hz": qfreq(0.50),
        "energy_f90_hz": qfreq(0.90),
        "low_energy_ratio_lt120hz": low_ratio,
        "high_energy_ratio_gt8khz": high_ratio,
        "dominant_peaks_hz": top,
    }


def event_density(mono: np.ndarray, sr: int) -> float:
    if mono.size < sr // 4:
        return 0.0
    frame = max(32, int(round(sr * 0.010)))
    hop = max(16, int(round(sr * 0.005)))
    if mono.size < frame:
        return 0.0

    x = mono.astype(np.float64, copy=False)
    env = np.sqrt(
        signal.convolve(x * x, np.ones(frame) / frame, mode="valid")[::hop] + 1e-20
    )
    if env.size < 8:
        return 0.0

    med = float(np.median(env))
    mad = float(np.median(np.abs(env - med))) + 1e-12
    threshold = med + 6.0 * mad
    active = env > threshold
    rising = np.count_nonzero(active[1:] & ~active[:-1])
    duration = mono.size / float(sr)
    return float(rising / max(duration, 1e-9))


def loopability(mono: np.ndarray, sr: int) -> dict:
    if mono.size < sr:
        return {"score": 0.0, "edge_rms_delta_db": 0.0, "edge_correlation": 0.0}

    win = min(mono.size // 4, int(sr * 0.250))
    if win < 64:
        return {"score": 0.0, "edge_rms_delta_db": 0.0, "edge_correlation": 0.0}

    a = mono[:win].astype(np.float64, copy=False)
    b = mono[-win:].astype(np.float64, copy=False)
    rms_a = math.sqrt(float(np.mean(a * a)) + 1e-30)
    rms_b = math.sqrt(float(np.mean(b * b)) + 1e-30)
    rms_delta_db = abs(db20(rms_a) - db20(rms_b))

    a0 = a - np.mean(a)
    b0 = b - np.mean(b)
    denom = math.sqrt(float(np.sum(a0 * a0) * np.sum(b0 * b0))) + 1e-30
    corr = float(np.sum(a0 * b0) / denom)
    corr = max(-1.0, min(1.0, corr))

    rms_score = math.exp(-rms_delta_db / 6.0)
    corr_score = max(0.0, corr)
    score = float(0.55 * rms_score + 0.45 * corr_score)
    return {
        "score": score,
        "edge_rms_delta_db": rms_delta_db,
        "edge_correlation": corr,
    }


def ir_metrics(mono: np.ndarray, sr: int) -> dict:
    if mono.size < 32:
        return {}

    absx = np.abs(mono.astype(np.float64, copy=False))
    peak_idx = int(np.argmax(absx))
    peak = float(absx[peak_idx])
    if peak <= 1e-12:
        return {}

    tail = mono[peak_idx:].astype(np.float64, copy=False)
    energy = tail * tail
    schroeder = np.cumsum(energy[::-1])[::-1]
    schroeder /= max(float(schroeder[0]), 1e-30)
    decay_db = 10.0 * np.log10(np.maximum(schroeder, 1e-30))

    def crossing(level_db: float):
        idx = np.flatnonzero(decay_db <= level_db)
        return None if idx.size == 0 else int(idx[0])

    t10 = crossing(-10.0)
    t20 = crossing(-20.0)
    t30 = crossing(-30.0)
    t60 = crossing(-60.0)

    def sec(idx):
        return None if idx is None else float(idx / sr)

    early_n = min(len(energy), max(1, int(round(sr * 0.080))))
    early = float(np.sum(energy[:early_n]))
    late = float(np.sum(energy[early_n:]))
    elr_db = 10.0 * math.log10((early + 1e-30) / (late + 1e-30))

    return {
        "direct_arrival_sample": peak_idx,
        "direct_arrival_seconds": peak_idx / float(sr),
        "decay_to_minus10_seconds": sec(t10),
        "decay_to_minus20_seconds": sec(t20),
        "decay_to_minus30_seconds": sec(t30),
        "decay_to_minus60_seconds": sec(t60),
        "early_0_80ms_to_late_energy_db": elr_db,
    }


def analyze(path: Path, treat_as_ir: bool) -> dict:
    data, sr = sf.read(str(path), always_2d=True, dtype="float64")
    frames, channels = data.shape
    finite = np.isfinite(data)
    finite_ok = bool(np.all(finite))
    if not finite_ok:
        data = np.nan_to_num(data, nan=0.0, posinf=0.0, neginf=0.0)

    mono = np.mean(data, axis=1)
    peak = float(np.max(np.abs(data))) if data.size else 0.0
    rms = math.sqrt(float(np.mean(data * data)) + 1e-30) if data.size else 0.0
    dc = float(np.mean(mono)) if mono.size else 0.0
    crest = peak / max(rms, 1e-30)

    silence_threshold = 10.0 ** (-90.0 / 20.0)
    silence_ratio = float(np.mean(np.abs(mono) <= silence_threshold)) if mono.size else 1.0

    stereo_corr = None
    if channels >= 2:
        l = data[:, 0] - np.mean(data[:, 0])
        r = data[:, 1] - np.mean(data[:, 1])
        denom = math.sqrt(float(np.sum(l * l) * np.sum(r * r))) + 1e-30
        stereo_corr = float(np.sum(l * r) / denom)
        stereo_corr = max(-1.0, min(1.0, stereo_corr))

    info = sf.info(str(path))
    subtype = info.subtype or ""
    result = {
        "path": str(path),
        "sha256": sha256_file(path),
        "container_format": info.format,
        "subtype": subtype,
        "sample_rate": int(sr),
        "channels": int(channels),
        "frames": int(frames),
        "duration_seconds": frames / float(sr),
        "finite_samples_only": finite_ok,
        "peak_linear": peak,
        "peak_dbfs": db20(peak),
        "rms_linear": rms,
        "rms_dbfs": db20(rms),
        "crest_factor": crest,
        "dc_offset": dc,
        "silence_ratio_below_minus90dbfs": silence_ratio,
        "stereo_correlation": stereo_corr,
        "event_density_per_second": event_density(mono, sr),
        "loopability": loopability(mono, sr),
    }
    result.update(spectral_metrics(mono, sr))
    if treat_as_ir:
        result["impulse_response"] = ir_metrics(mono, sr)
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("--json", type=Path, default=None)
    parser.add_argument("--ir", action="store_true", help="also compute impulse-response decay metrics")
    args = parser.parse_args()

    result = analyze(args.input, args.ir)
    payload = json.dumps(result, indent=2, sort_keys=True)
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(payload + "\n", encoding="utf-8")
    print(payload)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
