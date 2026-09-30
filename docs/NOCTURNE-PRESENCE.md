# NOCTURNE Presence Calibration

Status: 2026-09-30

## Decision

The abstract NOCTURNE presence layer is **accepted for the prototype architecture**.

It is synthesized from deterministic noise exciting four slowly drifting formant resonators. No choir recording, spoken material, lyric, or recognizable vocal sample is used.

## Isolated 30-second measurement

Measured after the calibrated presence-gain update:

- peak: **-29.91 dBFS**
- RMS: **-44.09 dBFS**
- spectral centroid: **584.7 Hz**
- energy below 120 Hz: **0.72 %**
- stereo correlation: **0.893**

Interpretation:
- intentionally subordinate in level;
- does not duplicate FOUNDATION's sub role;
- adds midrange/formant identity rather than literal voice playback;
- high correlation is acceptable for the isolated layer because spatial width comes from the complete world/space path, not from fake chorus widening.

## Full NOCTURNE archetype measurement

Current measured final scene:

- peak: **-14.67 dBFS**
- RMS: **-35.11 dBFS**
- spectral centroid: **365.0 Hz**
- energy below 120 Hz: **17.27 %**
- stereo correlation: **0.333**

This separates NOCTURNE from:
- WASTELAND: similar broad centroid but different low-frequency and spatial behavior;
- RUINS: brighter and substantially wider;
- ABYSS / VOID: strongly low-dominant.

## Realtime cost

The complete core including Presence passed the current realtime performance gate:

- 48 kHz: ~45–47x realtime
- 96 kHz: ~23–24x realtime
- 192 kHz: ~11.3–11.8x realtime

The enforced gate remains at least **4x realtime headroom** in the dedicated performance probe.

## Rule

Presence remains archetype behavior, not a literal choir control.

If exposed later, it should be through a musically meaningful macro/state interaction rather than a named "CHOIR" parameter.
