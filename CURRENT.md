# CURRENT — Noctomorph

Status date: 2026-09-30

## Authoritative working branches

- `main`: minimal public project root; do not use for active development yet.
- `research/sound-database-v0.1`: completed broad research corpus and source database.
- `dev/prototype-v0.1`: current active prototype / production-preparation branch.

## Product direction

Noctomorph is an evolving **dark-cinematic scene instrument**.

Locked identity:
- dark is always present;
- cinematic scale;
- Gothic influence without becoming a Gothic cliché soundboard;
- industrial / architectural / physical reality as raw material;
- unreal / impossible worlds are a core goal;
- real-source complexity plus synthesis where control or impossibility requires it;
- not a weather/nature generator;
- not a generic horror SFX player;
- not merely a drone generator.

## Research status

Broad research sweep: **COMPLETE ENOUGH TO BUILD**

- 152 documented database entries at research freeze.
- No major source-family blind spot remained in the coverage matrix.
- New sources are now added only when they beat an existing candidate or fill a concrete implementation need.

## Prototype core status

Branch: `dev/prototype-v0.1`

Implemented:
- deterministic dark tonal FOUNDATION;
- WORLD / TEXTURE clip hooks;
- IMPOSSIBLE BODY modal resonator bank;
- deterministic Lorenz-like EVOLVE state;
- sparse EVENTS;
- realtime-safe fixed-allocation algorithmic SPACE;
- full-wet SPACE semantics at 100 %;
- note-on / note-off lifecycle;
- -1 dBFS output safety ceiling;
- standalone smoke renderer;
- source/IR measurement tooling.

## Verified QA

Targeted Windows x64 Core QA: **PASS**

Verified:
- MSVC Release build;
- deterministic same-seed rendering;
- different-seed divergence;
- zero-source-layer semantics;
- block-size invariance: 1 / 16 / 64 / 257 / 1024;
- 100 % SPACE has no immediate dry leak;
- finite/extreme behavior at 44.1 / 48 / 96 / 192 kHz;
- note-off release decay;
- WORLD stereo playback and source-rate conversion hook;
- EVENT clip playback hook;
- 30 s smoke render;
- 5 min evolution probe.

5-minute probe:
- finite: yes
- peak: 0.89125 (-1 dBFS ceiling)
- RMS: 0.0603289
- events: 30
- one-second windows: 300
- exact duplicate one-second hashes: 0
- cumulative events by minute: 6 / 11 / 17 / 24 / 30

## Real-source pipeline status

Implemented:
- provenance manifests;
- derived-asset manifest;
- audio measurement utility;
- per-channel impulse-response metrics;
- folder/batch IR analyzer;
- OpenAIR CI source probe;
- Aachen Cathedral full-dataset CI probe.

Measured production evidence:
- DATA-IR-001 — Aachen Cathedral St. Nicholas Chapel: **46/46 measured**
- DATA-IR-004 — OpenAIR R1 Nuclear Reactor Hall
- DATA-IR-005 — OpenAIR Terry's Factory Warehouse

Aachen DATA-IR-001:
- all 46 files are 48 kHz / 4-channel B-format;
- measured -60 dB comparison landmark spans 4.483–9.965 s;
- mean measured -60 dB landmark: 7.546 s;
- full per-file hashes and metrics: `measurements/aachen/DATA-IR-001-summary.csv`.

## Repository audio policy

Third-party original audio must not be committed casually.

The repository keeps:
- source provenance;
- canonical URLs;
- licences;
- SHA-256;
- measurements;
- segment/transform recipes;
- derived-asset metadata.

Release embedding is a separate deliberate step after legal and audio QA.

## Real-source integration milestone

First real-source INDUSTRIAL scene: **PASS as engineering prototype**.

Measured current 5-minute output:
- peak -1.51 dBFS;
- spectral centroid 441 Hz;
- energy below 120 Hz 63.7 %;
- stereo correlation 0.599;
- 11 sparse engine events in 5 min.

Source-only control proves the real material itself is broad (centroid 1.46 kHz, stereo correlation 0.201). This led to permanent WORLD/TEXTURE gain calibration and sparse-event timing changes in the core.

See `docs/REAL-SOURCE-RENDER-QA.md`.

## Next gates

1. Expand measured production pools beyond the first INDUSTRIAL scene.
2. Build equivalent real-source scenes for RUINS / NOCTURNE / ABYSS / WASTELAND / VOID.
3. Add measured real exciters to IMPOSSIBLE BODY rather than only stream playback.
4. Test 30 s / 2 min / 5 min identity and repetition per archetype.
5. Only then freeze public control architecture and start the VST3 wrapper / GUI.


## 2026-09-30 calibrated real-source baseline

The first useful six-archetype baseline is now measured and documented in
`docs/ARCHETYPE-QA-BASELINE.md`.

Key result:
- archetypes are measurably separated in spectral balance and spatial behavior;
- source-specific BODY excitation is accepted;
- global BODY output trim is rejected;
- NOCTURNE's next identity step is an abstract presence/formant layer.


## 2026-09-30 Nocturne presence / BODY lifecycle checkpoint

- NOCTURNE abstract formant Presence: **accepted**, documented in `docs/NOCTURNE-PRESENCE.md`.
- Presence-only measurement: -44.09 dBFS RMS, 584.7 Hz centroid, 0.72 % energy below 120 Hz.
- Complete core with Presence passes the realtime performance gate with >11x realtime at 192 kHz in the current CI environment.
- BODY-exciter lifecycle corrected: a source now excites BODY at Note-On and defined events only; it is no longer silently auto-restarted as a continuous long stream.
- A dedicated regression test prevents BODY auto-retrigger from returning.


## 2026-09-30 six-archetype longform checkpoint

Five-minute real-source QA: **PASS for all six archetypes**.

- 300 one-second windows checked per archetype.
- Exact duplicate one-second windows: **0 for all six**.
- No non-finite/silent long-form failure.
- Detailed measurements: `docs/ARCHETYPE-LONGFORM-QA.md` and `measurements/longform/summary.csv`.
- Prototype scene architecture is now sufficiently stable to begin VST3 integration without freezing the final GUI.


## 2026-09-30 VST3 prototype checkpoint

First Noctomorph VST3 instrument wrapper: **PASS**.

- Steinberg Validator: **47/47 PASS**
- State/Recall native probe: **PASS**
- Process-contract native probe: **PASS**
- Matrix: realtime/offline, 44.1/48/96/192 kHz, block 1/16/64/257/1024.
- MIDI, sample-position automation, NaN sanitation, zero-sample flush and activate/deactivate lifecycle verified.
- Instrument topology: 0 audio inputs, stereo output, MIDI/event input.
- No GUI or embedded third-party audio yet by design.

Detailed evidence: `docs/VST3-PROTOTYPE-QA.md`.


## 2026-09-30 asset-enabled archetype checkpoint

Asset-enabled VST3 and archetype behavior families: **PASS**.

- Embedded WORLD/TEXTURE/EVENT roles are proven active from the loaded VST3.
- WORLD-only RMS: 0.024314
- TEXTURE-only RMS: 0.004179
- EVENT-only RMS: 0.000168
- All six archetypes now have explicit internal behavior traits rather than only
  modal-ratio differences.
- Dedicated regression prevents the six archetypes from collapsing to identical
  output under identical user settings.
- Latest Core QA: PASS.
- Latest Real-Source QA: PASS.
- Latest Archetype Listening QA: PASS.
- Latest asset-enabled VST3 Host QA: PASS.
- Steinberg Validator remains **47/47 PASS**.

Detailed behavior definition: `docs/ARCHETYPE-BEHAVIOR-FAMILIES.md`.


## 2026-09-30 control-architecture freeze

Public control architecture is now **frozen for static GUI prototyping**.

11 parameters:
ARCHETYPE / FOUNDATION / WORLD / TEXTURE / BODY / TENSION / MOTION /
EVOLVE / EVENTS / SPACE / OUTPUT.

MOTION was added after the product-concept audit because it is functionally
distinct from EVOLVE.

- State version bumped from v1 to v2.
- Existing first 10 state values and ParamIDs remain unchanged.
- Legacy v1 state migration: **PASS**, MOTION defaults to 35 %.
- Validator: **47/47 PASS** with 11 exported parameters.
- Core / real-source / archetype listening / VST3 host QA: all PASS.

Detailed control contract: `docs/CONTROL-ARCHITECTURE.md`.
