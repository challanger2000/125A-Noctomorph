# Noctomorph Production Preparation

Status: 2026-09-30
Branch: `dev/prototype-v0.1`

## Goal

Turn the completed research corpus into a controlled, measurable prototype without polluting the repository with uncontrolled third-party audio.

## Source flow

```
DISCOVERY
  -> RIGHTS VERIFIED
  -> ACQUIRED LOCALLY
  -> CHECKSUMMED
  -> MEASURED
  -> SEGMENTED
  -> DERIVED ASSET
  -> LISTENING / METRIC QA
  -> EMBEDDING CANDIDATE
```

Third-party originals are **not committed by default**. The repository stores provenance, checksums, measurements, derived-asset recipes and code. Audio may only enter a release asset bundle when its licence and redistribution path are explicitly approved.

## Production shortlist

See `docs/PRODUCTION-SHORTLIST.csv`.

The shortlist deliberately favors complementary physical behavior over source count. It covers:

- tonal/electrical foundation;
- long structural vibration;
- impossible modal bodies;
- glass/gong/pipe/tank resonance;
- subterranean and abandoned worlds;
- hydrophone motion;
- Gothic modal events;
- sparse animal/rail events;
- real cathedral/industrial/stone spaces.

## Acquisition

For each source create one row in `assets/provenance/source_manifest.csv` with:

- stable internal ID;
- canonical source URL;
- creator;
- licence;
- retrieval date;
- original filename;
- SHA-256;
- sample rate;
- bit depth/container;
- channel count;
- duration;
- production role;
- legal status;
- notes.

For CC BY material additionally record:

- required attribution text;
- licence URL;
- whether the asset was modified;
- exact modification summary.

## Measurement gate

Every acquired audio file must be measured before selection.

Minimum measurements:

- duration;
- sample rate/channels;
- sample peak;
- DC offset;
- RMS;
- crest factor;
- silence ratio;
- stereo correlation where applicable;
- spectral centroid;
- 10/50/90 % spectral-energy frequencies;
- low-band (<120 Hz) energy ratio;
- high-band (>8 kHz) energy ratio;
- transient/event density;
- estimated dominant/modal peaks;
- loopability heuristic for long beds;
- NaN/Inf scan.

Impulse responses additionally require:

- direct-arrival index;
- peak-normalized decay curve;
- EDT/T20-like approximate decay metrics where signal quality permits;
- early/late energy ratio;
- usable tail length;
- channel-format verification.

## Segmentation policy

Do not cut audio simply because it is long.

Segments should correspond to usable physical states:

- stable body;
- emergence;
- stress;
- event;
- decay;
- alternate perspective;
- clean/no-voice interval.

Keep source-relative start/end time, fades and transform recipe in a derived-asset manifest.

## First prototype architecture

The minimum engine is intentionally smaller than the eventual product:

1. **FOUNDATION**
   - synthetic dark tonal field;
   - optional real long-bed source;
   - bounded drift.

2. **WORLD / TEXTURE**
   - one or two real-source streams;
   - independent slow transport;
   - no obvious short looping.

3. **IMPOSSIBLE BODY**
   - fixed-allocation modal bank;
   - real/synthetic excitation;
   - controlled inharmonicity and damping.

4. **EVENTS**
   - sparse state-dependent events;
   - no constant random trigger rain;
   - event identity constrained by archetype.

5. **SPACE**
   - initial realtime-safe algorithmic tail;
   - later measured-IR / hybrid option after asset validation.

6. **EVOLVE**
   - deterministic non-periodic macro trajectory;
   - scene state changes, not arbitrary parameter randomization.

## Prototype QA gates

Before VST3 wrapper work:

- C++ core compiles warning-clean;
- deterministic same-seed render;
- different-seed render diverges while preserving macro statistics;
- 44.1/48/96/192 kHz;
- block sizes 1/16/64/257/1024;
- no allocation in process;
- no NaN/Inf;
- no denormal runaway;
- note-on starts; note-off releases;
- 30 s / 2 min / 5 min renders show no short-period repetition;
- 0 % controls obey neutral/off semantics where applicable;
- default scene is audibly dark without relying on weather/nature beds;
- output remains bounded under 100 % extreme settings.

## What is deliberately not frozen yet

- public GUI/control count;
- archetype final names;
- exact sample library size;
- convolution vs hybrid-space production choice;
- granular implementation details;
- final demo timing.

Those decisions should follow measured prototype evidence rather than precede it.
