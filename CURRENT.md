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

Key documents:
- `docs/SOUND-SOURCE-DATABASE.md`
- `docs/RESEARCH-COVERAGE.md`
- `docs/PRODUCTION-SHORTLIST.csv`
- `docs/PRODUCTION-PREP.md`

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

Measured production candidates already committed as evidence:
- DATA-IR-004 — OpenAIR R1 Nuclear Reactor Hall
- DATA-IR-005 — OpenAIR Terry's Factory Warehouse

Aachen Cathedral DATA-IR-001:
- 46 measured B-format IRs expected;
- full-dataset CI measurement currently in progress at this status checkpoint.

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

## Next gates

1. Complete Aachen Cathedral full-dataset measurement.
2. Continue acquisition/measurement of the highest-priority production shortlist.
3. Add measured real sources to WORLD / TEXTURE / EVENT / BODY prototype paths.
4. Produce 30 s / 2 min / 5 min real-source listening renders.
5. Compare archetype identity and repetition metrics.
6. Only then freeze public control architecture and start the VST3 wrapper / GUI.
