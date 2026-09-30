# Noctomorph VST3 Prototype QA

Status: 2026-09-30
Branch: `dev/prototype-v0.1`
Validated workflow run: `36697084055`

## Scope

This is the first host-facing VST3 prototype. It intentionally contains:
- processor/controller split;
- stereo instrument output;
- MIDI/event input;
- 10 public parameters;
- deterministic monophonic scene retrigger;
- versioned state;
- sample-accurate parameter handling;
- no GUI;
- no embedded third-party production audio.

The purpose is host-contract validation before GUI and asset packaging.

## Steinberg Validator

**PASS — 47 tests passed, 0 failed**

Detected class:
- name: 125A Noctomorph
- category: Audio Module Class
- subcategory: Instrument|Synth
- version: 0.1.0
- SDK: VST 3.8.1
- Processor CID: `125A03014E4F43544F4D4F5250483001`
- Controller CID: `125A03024E4F43544F4D4F5250483002`

Bus topology:
- audio input: 0
- audio output: 1 stereo
- event input: 1
- event output: 0

Parameters:
1. Archetype
2. Foundation
3. World
4. Texture
5. Body
6. Tension
7. Evolve
8. Events
9. Space
10. Output

Validator confirms both block and sample automation accuracy tests.

## State / Recall probe

**PASS**

Verified:
- exact default state;
- processor getState/setState roundtrip;
- controller setComponentState synchronization;
- state magic/version contract;
- rejection of NaN state;
- rejection of out-of-range state;
- rejection of bad magic;
- rejection of unknown state version;
- rejected state does not mutate last valid state.

Current format:
- magic: `NOC1`
- version: 1
- 10 normalized float values.

## Process contract probe

**PASS**

Matrix:
- realtime and offline modes;
- 44.1 / 48 / 96 / 192 kHz;
- block sizes 1 / 16 / 64 / 257 / 1024.

Verified:
- 32-bit processing supported;
- 64-bit processing not falsely advertised;
- instrument bus topology;
- stereo arrangement;
- MIDI Note On / matching Note Off;
- sample-position parameter automation;
- NaN automation sanitization;
- legal zero-sample parameter flush;
- repeated setActive / setProcessing lifecycle;
- finite output throughout.

## Intentional prototype limitations

- No editor/GUI yet, therefore editor lifecycle/zoom QA is not applicable yet.
- No embedded real production-source bank yet. The wrapper currently exercises the synthetic/fallback engine architecture only.
- 64-bit audio processing is not advertised.
- The scene instrument is monophonic at this stage.
- Final VST3 version number is not frozen.

## Gate result

**VST3 wrapper architecture: PASS for continued development.**

Next host-facing work should be:
1. controlled production asset bank integration;
2. state-safe asset/archetype selection;
3. GUI/editor implementation;
4. editor lifecycle / DPI / 100–150% zoom QA;
5. final realtime/offline/audio torture with the asset-enabled build.


## Asset-enabled host verification

Validated workflow run: `36701035412`

The VST3 bundle now contains the controlled prototype asset bank and the
third-party attribution file.

The native process probe explicitly isolates the three direct sample roles.
With FOUNDATION, BODY, SPACE and the other sample roles disabled:

- WORLD-only RMS: **0.024314**
- TEXTURE-only RMS: **0.004179**
- EVENT-only RMS: **0.000168**

All three are non-zero.

This is a hard host-side proof that the loaded VST3 is using the embedded asset
bank rather than silently falling back to the synthetic-only engine.

After the archetype behavior-family update:
- Core QA: PASS
- Real Source Render QA: PASS
- Archetype Listening QA: PASS
- State/Recall: PASS
- Process Contract: PASS
- Steinberg Validator: **47/47 PASS**

The attribution file is also checked for presence inside
`Noctomorph.vst3/Contents/Resources` during CI.
