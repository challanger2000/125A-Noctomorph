# Archetype 5-Minute Longform QA

Status: 2026-09-30
Workflow run: `36695110370`
Artifact digest: `sha256:3be2a2746a88884e3bef56d5703fd76e4daa4c4583ebcd912ece1f1db8e23ec4`

## Scope

Six controlled real-source archetypes were rendered for 300 seconds each, then
passed through their measured-space listening paths.

The first 300 seconds of each final render were checked for:
- finite samples;
- output bound;
- one-second exact repetition;
- second-to-second RMS variation;
- minute-by-minute RMS continuity;
- spectrum;
- low-frequency energy;
- midrange presence/formant energy;
- spectral flatness;
- stereo correlation.

## Result

**PASS — all six archetypes complete the 5-minute gate.**

Exact repeated one-second windows:
- INDUSTRIAL: 0 / 300
- RUINS: 0 / 300
- NOCTURNE: 0 / 300
- ABYSS: 0 / 300
- WASTELAND: 0 / 300
- VOID: 0 / 300

No archetype becomes silent, non-finite, or obviously periodic during the
measured interval.

## Long-form behavior

### INDUSTRIAL
- centroid: 252 Hz
- <120 Hz: 45.4 %
- stereo correlation: 0.591
- second-RMS std: 1.44 dB
- minute RMS remains tightly clustered around -37 dBFS

Interpretation: stable physical-machine world with moderate tonal mass.

### RUINS
- centroid: 561 Hz
- <120 Hz: 21.7 %
- 350–3000 Hz energy: 39.2 %
- stereo correlation: 0.157
- second-RMS std: 2.05 dB

Interpretation: brightest and widest current state. The -1 dBFS peak is from the
offline IR-probe safety normalization, not evidence of core clipping.

### NOCTURNE
- centroid: 350 Hz
- <120 Hz: 23.5 %
- 350–3000 Hz energy: 19.7 %
- stereo correlation: 0.330
- second-RMS std: 1.61 dB

Interpretation: stable, dark, spacious and materially less low-dominant than
ABYSS/VOID. The accepted abstract Presence layer remains subordinate.

### ABYSS
- centroid: 164 Hz
- <120 Hz: 53.5 %
- stereo correlation: 0.530
- second-RMS std: 4.16 dB

Interpretation: deliberately deep and more dynamically unstable. Minute RMS
moves downward over the five-minute window but remains comfortably active.

### WASTELAND
- centroid: 361 Hz
- <120 Hz: 17.7 %
- stereo correlation: 0.420
- second-RMS std: 1.12 dB

Interpretation: leanest/stablest exposed world. Its coarse centroid remains near
NOCTURNE, but low-frequency and spatial behavior differ.

### VOID
- centroid: 112 Hz
- <120 Hz: 76.6 %
- stereo correlation: 0.517
- second-RMS std: 4.37 dB

Interpretation: intentionally the deepest state, with strong long-term level
movement and no exact repetition.

## Identity conclusion

The six states are technically separated enough for continued development.

Remaining listening question:
- NOCTURNE and WASTELAND remain close in a single coarse metric (centroid), so
  final voicing must be judged with the complete listening pack, not centroid
  alone.

This is **not a release blocker**. Their low-frequency balance, spatial
signature, formant/presence behavior and intended scene semantics already differ.

## Next technical gate

Move from prototype scene architecture to the VST3 integration layer while
preserving:
- deterministic recall;
- no realtime allocation;
- source-specific BODY excitation;
- sparse event behavior;
- full-wet SPACE semantics;
- current archetype profiles;
- measured long-form non-repetition.
