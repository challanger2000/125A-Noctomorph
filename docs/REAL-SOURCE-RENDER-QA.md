# Real-Source Render QA

Status date: 2026-09-30
Branch: `dev/prototype-v0.1`

## First production-source scene

Current QA scene uses:

- WORLD: Work With Sounds — Paper Machine PM4
- TEXTURE: Work With Sounds — Automatic Packing Station
- EVENT: Wikimedia CC0 — Petersglocke / Cologne Cathedral
- synthetic FOUNDATION + IMPOSSIBLE BODY + SPACE + EVOLVE remain active

These sources are used for engineering validation, not as a final preset definition.

## Source-only control

A 30-second control render disables FOUNDATION, BODY and EVENTS while keeping the same real WORLD/TEXTURE material.

Measured:

- spectral centroid: **1460.9 Hz**
- energy below 120 Hz: **9.56 %**
- stereo correlation: **0.201**
- RMS: **-39.12 dBFS**

Conclusion:

The selected real source material is naturally broad and spatial. The earlier low-heavy output was not caused by the source recordings.

## Calibration sequence

### Initial full scene

The first full real-source scene was dominated by synthetic low-frequency layers:

- 300 s spectral centroid: ~69 Hz
- energy below 120 Hz: ~87 %
- stereo correlation: ~0.85
- EVENTS: 35 events / 5 min

This was rejected as too close to a sub-heavy drone.

### Event and source calibration

Corrections applied:

1. EVENTS low-range timing rewritten so low settings become genuinely sparse.
2. FOUNDATION changed so the root is the main tonal carrier and the sub-octave is support.
3. WORLD/TEXTURE real-source gains recalibrated against synthetic FOUNDATION/BODY.

### Current full scene

Latest 300-second render:

- peak: **-1.51 dBFS**
- RMS: **-34.30 dBFS**
- spectral centroid: **441.2 Hz**
- 10/50/90 % energy frequencies: **29.3 / 68.8 / 567.6 Hz**
- energy below 120 Hz: **63.72 %**
- energy above 8 kHz: **1.55 %**
- stereo correlation: **0.599**
- sparse engine events: **11 / 5 min**
- finite samples only: PASS

30-second render:

- spectral centroid: **685.2 Hz**
- energy below 120 Hz: **44.34 %**
- stereo correlation: **0.593**

Interpretation:

- dark/low-centered identity remains;
- real industrial material now materially shapes the output;
- stereo field is substantially less mono-dominated;
- sparse bell/event behavior is now plausible;
- no clipping or numerical instability;
- the scene is no longer merely a synthetic sub-drone with field recordings underneath.

## Current verdict

**Engineering direction: PASS**

This is not yet a final sound-quality verdict. The next work should diversify the real-source pools and test multiple archetypes rather than over-tune this single INDUSTRIAL scene.
