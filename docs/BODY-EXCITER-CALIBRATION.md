# BODY Exciter Calibration

Status: 2026-09-30

## Purpose

The IMPOSSIBLE BODY engine is a resonant physical layer. External recordings are
**exciters**, not parallel audible sample layers.

The same peak-normalized source can inject radically different energy into a
high-Q modal bank. Therefore BODY assets must not be calibrated by sample peak
alone.

## Measured evidence

### GONG-SHORT — deep modal exciter

- duration: 5.600 s
- SHA-256: `f57283d08a0ecb052425af1ac1457f827fa27f423d49158963b3370c89fc942d`
- spectral centroid: 193.94 Hz
- energy <120 Hz: 27.79 %
- energy f10/f50/f90: 96.90 / 193.80 / 325.69 Hz
- event density: 1.25 / s
- role: **DEEP / MONUMENTAL BODY EXCITER**
- current prototype calibration: strongly attenuated for ABYSS / VOID

A deep gong is useful, but using it at unity exciter gain double-emphasizes the
same low modes already provided by FOUNDATION and the modal bank.

### MEDITATION-GONG — tonal alternate

- duration: 15.832 s
- SHA-256: `790c6f1eba33512a1a4846e6d14dec22d649f85dee8b8556e43f7ae66359a070`
- spectral centroid: 222.28 Hz
- energy <120 Hz: 0.044 %
- stereo correlation: 0.99999
- role: **TONAL BODY EXCITER / ALTERNATE**
- note: source stereo is effectively mono; spatial identity must come from the body/space engine.

### TICKING-GLASS — broadband transient exciter

- duration: 19.083 s
- SHA-256: `cdd6ea5641c2c25c8656b40de7ae94eb34705475d8fab92b6431fe1bd3ede7cf`
- spectral centroid: 7.722 kHz
- energy <120 Hz: 0.005 %
- energy >8 kHz: 39.30 %
- event density: 7.81 / s
- role: **BROADBAND / BRITTLE / PARTICLE BODY EXCITER**
- useful for INDUSTRIAL / RUINS / NOCTURNE / WASTELAND tests because it excites
  the body's own modal spectrum instead of imposing a low tonal fundamental.

## Calibration rule

Each production BODY asset needs an explicit `excitationGain`.

Do not infer this value from:
- file peak;
- file RMS alone;
- nominal bit depth;
- subjective loudness alone.

Determine it from:
1. normalized source playback;
2. actual modal-bank response RMS/peak;
3. low-frequency energy ratio;
4. crest factor / transient behavior;
5. comparison against WORLD/TEXTURE energy;
6. 30 s and long-form scene behavior.

## Target behavior

BODY should:
- add material identity and resonant depth;
- remain clearly subordinate to the complete scene unless intentionally pushed;
- not duplicate FOUNDATION's sub role;
- not force the output safety ceiling continuously;
- not erase the natural stereo field of WORLD/TEXTURE;
- remain physically plausible at low/mid values and become unreal only as the
  user deliberately increases BODY/TENSION/EVOLVE.

## Rejected approach

A global fixed BODY output trim was tested and rejected.

It solved an overdriven deep-gong case but made broadband glass excitation far
too quiet. Calibration therefore belongs to the **exciter source**, not to a
single global post-body trim.
