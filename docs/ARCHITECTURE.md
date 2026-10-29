# V4JB – Architecture

JUCE plugin built from `V4JB.jucer` with Projucer and MSBuild (`just build-debug`). The separate `tests/CMakeLists.txt` builds audit executables, not a replacement product. Every new product file needs a `<FILE>` entry in the jucer, then `just resave`.

## Source

- `src/PluginProcessor.*` – the audio processor. Owns the APVTS, the engine, the morph pad, the effects (slot 12) and the wave data. Per block: read parameters, advance the morph, split the block at each MIDI event and render the engine, run the effects, ramp Volume. Real-time: no allocations, locks or system calls. Also handles state save/load, presets, the randomizer, and wave loading on a worker thread (`juce::ThreadPool`), message thread only.
- `src/PluginEditor.*` – the shell: top bar (About, presets, Randomize, Economy, Voices, Octave, Pitch, Glide, Volume) and four tabs. Standalone adds a piano strip (C2–C6).
- `src/ui/` – header-only pages: `Theme.h` (palette, look and feel, Knob/HSlider/Combo/Toggle, spectrum bars), `EditPage.h` (12-slot chain), `ModPage.h` (envelopes, LFOs), `MorphPage.h` (XY pad, path editor, macros), `WavePage.h`. `VersionCompare.h` compares tags for the update checker.
- `src/dsp/` – the engine, header-only, no JUCE, tested by `just check`: `Common.h`, `Partials.h` (sine bank), `Spectral.h` (slot 1), `Modules.h` (module pool), `Mod.h` (envelopes, LFOs), `Morph.h`, `Wave.h`, `Engine.h` (voices, macro mapping, spectrum taps). See `docs/DSP_NOTES.md`.
- `src/WaveAnalyzer.h` – audio to `WaveData` (FFT, worker thread). A wave source is an absolute path or `factory:<name>.wav`, which reads a bundled `presets/waves/*.wav` from BinaryData. `tools/make_waves.cpp` (`just waves`) synthesises those files.
- `src/Presets.h` – factory presets (BinaryData from `presets/*.xml`) and user presets (`Documents/V4JB/Presets`).
- `src/parameters/` – `ParameterIDs.h` (ids are saved in DAW projects and presets, do not rename) and `ParameterLayout.h` (about 225 parameters, generated in loops).

## Threads and sharing

| Data | Writer | Reader | How |
|------|--------|--------|-----|
| Parameters | host / UI | audio | APVTS atomics, cached pointers |
| Standalone key clicks | UI | audio | `AbstractFifo` |
| Morph corners, path nodes | UI | audio | relaxed atomics in `v4jb::Morph` |
| Puck, live macros, spectrum taps | audio | UI | relaxed atomics, UI polls at 30 Hz |
| Wave data | worker -> message thread / host recall | audio | `AudioReadSlot` single-reader hazard; non-audio publishers/collectors serialize under `waveMutex` |

## State

The APVTS tree (`<PARAMS>`) plus a `<MORPH c0..c3 nodes>` child, `waveA`/`waveB` sources (file path or `factory:` name) and `presetName`. Factory presets may set `waveA`/`waveB`; presets without them keep the loaded waves. User presets are the same XML with `presetName` and `category`. Factory presets list only what differs from the defaults. Morph points and paths can also be saved on their own (`.v4jbmorph`).

Audit candidate: `waveDataVersion=1` adds `WAVE_DATA` children containing bounded,
little-endian spectral-analysis floats, so new sessions/presets do not require
the source file. Legacy sources are decoded before committing recall; a missing
source rejects recall and reports an error instead of discarding the old sound.
Resource validation precedes parameter mutation. Host/control-thread recalls
must remain off audio: XML/APVTS and legacy file decoding are not realtime safe.
Old plugin versions cannot restore the new embedded data without the old files.

## Update check

About → "Check for update" fetches `https://api.github.com/repos/iamtew/V4JB/tags` on a background thread, keeps tags shaped like `vN.N.N`, and compares them with the jucer `version=`.

## Release

`just build-dist` builds Release, zips VST3 and Standalone, and builds the WiX MSI from `installer/`.

## Continuous editor resizing: reusable JUCE reference

Source: `src/PluginEditor.cpp` (constructor's resize setup and `resized()`) and `src/PluginEditor.h` (`Content` component); session persistence: `src/PluginProcessor.cpp` (`uiScale`). The logical layout remains at **1220 x 800** pixels (Standalone: plus 72-pixel keyboard strip). The editor is `setResizable(true, true)`, with `setResizeLimits` for **40–150%**, a fixed aspect ratio, and a proportional `content.setTransform(juce::AffineTransform::scale(s))` for **all real interactive child components**. It is neither a raster screenshot zoom nor a set of discrete zoom buttons. The initial scale is restored from the session's `uiScale` before setting resize limits; `resized()` writes updates back to that state. Factory preset loading does not change the editor size.

**For future 125A chats:** the central, authoritative copy of this reusable design pattern is [`125A-Engineering/STANDARDS/GUI-GUIDELINES.md`](https://github.com/challanger2000/125A-Engineering/blob/main/STANDARDS/GUI-GUIDELINES.md), linked from `125A-Engineering/START-HERE.md`. Refer to this implementation when asked for *stufenlose Vergrößerung / nahtloses GUI-Resize / Ziehen der Fensterecke ohne Zoom-Buttons*. JUCE code must be adapted for plugins using VSTGUI or other frameworks. Actual Studio One drag-to-resize, mouse interactions and high-DPI rendering are **NOT RUN**, even though the Windows VST3 CI and Steinberg validator passed. Current owner acceptance is for **private use**, not a publication claim.
