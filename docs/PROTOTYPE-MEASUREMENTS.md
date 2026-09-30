# Noctomorph Prototype Measurements

Status: 2026-09-30
Branch: `dev/prototype-v0.1`

## Core QA

Verified by targeted Windows x64 CI:

- MSVC Release build: PASS
- deterministic same-seed render: PASS
- different-seed divergence: PASS
- 0 % source-layer semantics: PASS
- finite output under extreme settings: PASS
- sample rates 44.1 / 48 / 96 / 192 kHz: PASS
- note-off release decay: PASS
- output safety ceiling: PASS
- 30-second smoke render: PASS

## 30-second smoke render measurement

Configuration:

- sample rate: 48 kHz
- stereo
- archetype: NOCTURNE
- MIDI note: 36
- FOUNDATION 58 %
- TEXTURE 30 %
- BODY 55 %
- TENSION 42 %
- EVOLVE 72 %
- EVENTS 38 %
- SPACE 48 %
- OUTPUT 50 %
- no external WORLD/TEXTURE/EVENT samples loaded yet

Measured:

- duration: 30.0 s
- peak: **-1.48 dBFS**
- RMS: **-27.94 dBFS**
- crest factor: **26.47 dB**
- DC offset: **1.68e-5**
- stereo correlation: **0.806**
- spectral centroid: **124.8 Hz**
- 10 % energy frequency: **32.2 Hz**
- 50 % energy frequency: **86.4 Hz**
- 90 % energy frequency: **264.4 Hz**
- strongest 0.25-10 s envelope autocorrelation: **0.519 at 9.80 s**
- samples touching the -1 dBFS safety ceiling: **0**

## Interpretation

### PASS

- Output is numerically stable.
- No hard clipping remains in the smoke render.
- The default prototype is intentionally very dark and low-centered.
- Crest factor is large enough that sparse events can coexist with a low sustained bed.
- The core already produces significant slow-level variation rather than a flat static drone.

### Not yet a sound-quality verdict

The current smoke renderer contains **no selected real-world source assets**. Therefore:

- stereo correlation is not yet representative of the production instrument;
- spectral balance is intentionally provisional;
- Gothic/industrial identity cannot be judged from the synthetic skeleton alone;
- the render is an engineering proof, not a preset.

## Next QA gate

A five-minute evolution probe now checks:

- finite output for the full render;
- -1 dBFS safety bound;
- non-silent RMS;
- no exact repeated one-second stereo windows;
- plausible sparse event count;
- events continue to occur in every minute.

After that passes, the next meaningful step is to insert measured real-source candidates into WORLD/TEXTURE/EVENT/IMPOSSIBLE-BODY paths.
