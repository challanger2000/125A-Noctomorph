# Archetype QA Baseline

Status: 2026-09-30

This is the first production-preparation baseline where all six Noctomorph
archetypes are rendered from controlled real-source material while BODY uses
source-specific exciter calibration.

## Shared controlled source set

- WORLD: Work With Sounds Paper Machine PM4
- TEXTURE: Work With Sounds Automatic Packing Station
- EVENT: Cologne Cathedral Petersglocke
- BODY:
  - broadband glass exciter for INDUSTRIAL / RUINS / NOCTURNE / WASTELAND
  - deep gong exciter, strongly attenuated, for ABYSS / VOID
- measured spaces:
  - INDUSTRIAL: OpenAIR R1 Nuclear Reactor Hall
  - RUINS: OpenAIR Maes Howe Tomb, B-format stereo decode
  - NOCTURNE: OpenAIR Hamilton Mausoleum, B-format stereo decode
  - ABYSS: OpenAIR Hamilton Mausoleum, B-format stereo decode
  - WASTELAND: OpenAIR Terry's Factory Warehouse
  - VOID: OpenAIR Hamilton Mausoleum, B-format stereo decode

## Measurements

| Archetype | Peak dBFS | RMS dBFS | Spectral centroid | Energy <120 Hz | Stereo correlation |
|---|---:|---:|---:|---:|---:|
| INDUSTRIAL | -14.24 | -37.34 | 256.39 Hz | 45.31 % | 0.588 |
| RUINS | -5.34 | -33.55 | 582.34 Hz | 18.11 % | 0.159 |
| NOCTURNE | -14.61 | -35.29 | 360.95 Hz | 17.85 % | 0.330 |
| ABYSS | -4.81 | -31.83 | 137.64 Hz | 68.22 % | 0.599 |
| WASTELAND | -17.63 | -36.00 | 364.79 Hz | 15.36 % | 0.401 |
| VOID | -7.73 | -34.65 | 117.06 Hz | 73.71 % | 0.500 |

## Interpretation

### PASS — archetype separation now exists

The six states no longer collapse into one low drone with different labels.

- **RUINS** is the brightest/widest measured state and strongly spatial.
- **ABYSS** and **VOID** intentionally retain the deepest low-frequency identity.
- **INDUSTRIAL** sits between physical machinery and tonal body.
- **WASTELAND** is relatively lean in the low end and more exposed/dry.
- **NOCTURNE** is darker than RUINS/WASTELAND and spatially wider than INDUSTRIAL.

### Remaining overlap

NOCTURNE and WASTELAND have similar spectral centroids (~361–365 Hz), although
their low-energy ratios and spatial signatures differ.

Next design step:
- give NOCTURNE an internal **abstract presence / formant-resonance layer**;
- do not use literal choir playback;
- keep it sparse, dark, slowly drifting and subordinate to the world.

## BODY conclusion

Source-specific BODY excitation is now the accepted architecture.

Rejected:
- one global BODY output trim;
- one deep gong source for every archetype;
- continuous low-frequency machine-bed excitation at unity gain.

Accepted:
- BODY source carries an explicit excitation calibration;
- broadband transient material for normal body excitation;
- deep tonal material only where an archetype intentionally needs monumental low modes.
