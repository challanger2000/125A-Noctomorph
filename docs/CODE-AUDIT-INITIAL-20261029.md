# 125A Noctomorph / V4JB 0.0.4 — Initial source-code and test-adequacy audit

**Date:** 2026-10-29. **Audited development HEAD:** `bf295a91b2aa0ae29d2693d643fa2169c8177e72`.
**Scope:** Static inspection of processor, JUCE editor, advanced UI, morph controls, WAV analyzer, DSP and existing tests; independent JUCE-free DSP test run under AddressSanitizer + UndefinedBehaviorSanitizer.
**Status:** **PARTIAL SOURCE AUDIT — NOT RELEASE READY**. Findings are source-level defects/risks; no reproduction in a loaded VST3 host is yet claimed.

## Verified evidence
- The entire `src/PluginProcessor.cpp` blob is **byte-identical** to upstream V4JB tag 0.0.4 (SHA `a98ba5644faa952d55a13c18fa68d1a2306cbed2`). Processor findings predate the GUI redesign.
- `tests/engine_check.cpp` compiled and ran using Linux `g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -Isrc` from supplied V4JB source, with `ASAN_OPTIONS=detect_leaks=1:halt_on_error=1` and `UBSAN_OPTIONS=halt_on_error=1`: **PASS**; reported `rms chord 0.171, peak 0.587, tail 0.00000; engine_check OK`. This test does **not** link JUCE or exercise the processor/UI/host.
- The last screenshot-oriented Windows CI run `#8` passed its JUCE compilation and DSP self-test, and captured ten actual standalone-editor PNGs. No Steinberg validator, DAW project recall, automation or real-time race stress was executed. That CI branch does not include the later one-line text-only change in the development HEAD.
- `src/PluginProcessor.cpp` still implements the original V4JB DSP; no audio-function fix is made by this audit document.

## Findings, ranked

### F-01 — HIGH / lifetime race: wave data may be retired without observing audio-thread quiescence
- `src/PluginProcessor.cpp:214-216, 486-504`; `src/PluginProcessor.h:118-128`.
- Audio callback loads raw `WaveData*` into Engine. On replacement, message thread keeps prior `shared_ptr` in `retired`, but timer frees after **10 seconds regardless of `blockCounter` advancement**.
- If audio callback stalls while holding the prior pointer, elapsed wall time is not proof it has stopped using that memory; potential use-after-free in an adversarial suspended/slow callback. Standard 2-completed-block reclamation is safer than time-only fallback, but lifecycle/stop/restart must also be handled without leaking indefinitely.
- **Proof level:** source-level unsafe lifetime condition; no ASan JUCE-thread reproduction yet.
- **Fix/test plan:** use a provable audio-read epoch or ownership handoff; only reclaim when no callback can retain the pointer. Stress repeated A/B loading/clear while audio callback is deliberately held, then resume; ASan/TSan where supported; stop/start and editor close tests.

### F-02 — HIGH / incorrect 100% wet endpoints under 125A control rules
- `src/PluginProcessor.cpp:326-340, 343-358`.
- Delay uses `l[i] += mix * dl` and `r[i] += mix * dr`: original dry signal remains at **100%** even when MIX = 100%, with echo added.
- Reverb sets `rp.dryLevel = 1.f - 0.5f * mix`; at MIX = 100%, **50% dry** remains.
- Under 125A's explicit wet/dry contract, `100% wet` must have no direct sound. The existing code does not satisfy that contract. Treat as a **DSP specification/design change**, not as a silent equivalence-preserving GUI fix to upstream V4JB.
- **Fix/test plan:** define linear/equal-power mix law, then independent impulse test at 0%, 50%, 100%, with level/tail and preset regression; explicit owner approval for intentional change in original sound.

### F-03 — MEDIUM/HIGH / mono output silently truncated on an oversized host callback
- `src/PluginProcessor.cpp:97-100, 218-257`.
- Mono scratch is allocated at `max(samplesPerBlock,8192)` during prepare. If an actual mono buffer exceeds that allocation, `m=min(n,scratch.size())` and all remaining output frames are forced to zero.
- **Proof level:** deterministic branch in code for `n > monoScratch.size()`, not yet exercised in the VST3 host.
- **Fix/test plan:** preallocate to supported maximum or process mono in bounded chunks without RT allocation; probe block sizes 1, 16, 128, 512, 8192, 16384 with intentionally lower reported prepare size; ensure no silent tail.

### F-04 — MEDIUM/HIGH / dropped note-offs when Standalone key FIFO fills
- `src/PluginProcessor.cpp:115-138`; `src/PluginProcessor.h:49-51, 95-100`.
- `pushKey` silently drops any key event when the 31-entry AbstractFifo is full. A dropped **note-off** after an accepted note-on can leave a voice held until a later all-notes-off/voice steal.
- **Proof level:** straightforward queue-full path; no Standalone input stress test yet.
- **Fix/test plan:** ensure note-off/all-notes-off cannot be lost; stress press/release storms while audio is paused, then resume. Check no stuck voices.

### F-05 — MEDIUM / missing invalid host-tempo guard
- `src/PluginProcessor.cpp:186-201, 284-293`.
- Host BPM is used without checking whether it is finite and positive. Synced delay computes `60.f / (float)bpm`; zero/negative/nonfinite values can produce invalid or unintended delay lengths. Engine transport also receives BPM directly.
- **Fix/test plan:** guard to a documented fallback tempo and test 0, negative, NaN, infinity, valid BPM, play/stop and missing PPQ.

### F-06 — MEDIUM / session portability of WAV-based patches
- `src/PluginProcessor.cpp:522-564`.
- State stores `waveA` and `waveB` as source paths; external WAV contents are not embedded. Missing paths on project reopen cause `clearWave`, potentially changing the patch silently.
- **Fix/test plan:** decide embed/cache/relocation strategy, or detect missing paths and report reproducibly without silently changing previously accepted state. Test new location, missing file and offline render.

### F-07 — MEDIUM / missing feedback on unsuccessful wave import
- `src/PluginProcessor.cpp:464-478`; `src/ui/WavePage.h:27-61`.
- If the reader fails and `WaveAnalyzer::analyse` returns null, completion callback does nothing: the previous wave remains, with no visible error. The user may falsely believe a new sample loaded.
- **Fix/test plan:** explicit error/status on the message thread; retain previous sound on failure, test corrupt/unsupported/nonexistent files and cancellation.

### F-08 — RELEASE BLOCKER / weak test coverage
- `.github/workflows/windows-vst3.yml:68-99`; `tests/engine_check.cpp`.
- Development branch workflow compiles the engine self-test and VST3, but does **not** run Steinberg validator, JUCE processor effect-mix/host event probes, state save/restore, automation, UI-to-parameter/PCM checks, waves thread-lifetime tests or DAW editor lifecycle.
- An automated screenshot test confirms pixels were generated, not that every clicked button, knob, state change, automation endpoint, off/on effect, preset or render matches the contract.
- **Fix/test plan:** add a JUCE-linked processor harness and host/VST3 probes, explicit requirement-to-test coverage, validator QA and DAW trials. Distinguish PASS/FAIL/NOT RUN per requirement.

## Initial requirement-to-test matrix

| Requirement | Evidence now | Required next test | Status |
|---|---|---|---|
| JUCE-free additive DSP generates finite bounded signals | Original engine_check + ASan/UBSan run | Multiple sample rates and stochastic stress | **PASS in limited test** |
| Windows VST3 compiles | GitHub Actions #8 | Actual VST3 host/validator | **PASS compile only** |
| Delays and reverbs obey 0% / 100% wet rule | Code contradicts rule | Independent impulse + wet-only PCM oracle | **FAIL code contract** |
| Wave replacement lifetime safe | Unbounded wall-clock timeout in retirement | Concurrent swap/stall/stop/restart test under sanitizer | **NOT VERIFIED / source risk** |
| No stuck notes when key FIFO is full | queue drops events | Buffered keyboard storm / paused callback | **NOT VERIFIED / source risk** |
| Mono 16k buffer has no missing samples | code caps scratch | Oversized variable-block host test | **FAIL if n exceeds scratch** |
| Host BPM invalid inputs safe | no guard | Tempo boundary tests | **NOT VERIFIED / source risk** |
| Preset save/reopen including external WAV | absolute file path only | Missing/moved resources; DAW project recall | **NOT VERIFIED** |
| 125A every interactive GUI control works | PNG captures only | Button/knob to APVTS to PCM/GUI and automation matrix | **NOT RUN** |
| Steinberg VST3 validator, Studio One lifecycle | no evidenced test | Validator, multiple instances, save/reopen, reopen editor | **NOT RUN** |

## Repair order; do not churn Windows Actions for individual source edits
1. Write tests/oracles for F-01 (wave lifetime), F-03 (mono), F-04 (key FIFO) and F-05 (tempo) before or with fixes. Address thread safety before release.
2. Fix F-07 WAV failure UX and F-06 project portability according to defined policy.
3. Resolve F-02 as an **intentional** wet/dry sound contract; run level-matched regression and approve changed behavior.
4. Add complete processor/UI/host test harness and Steinberg validator, then do one consolidated Windows build with attached screenshots, VST3 validation, state/automation/recall and Studio One acceptance.
5. Audit all remaining DSP implementations, randomizer rules, CPU/thread performance and third-party licensing separately; this document does not claim exhaustive coverage.

**No functional source was changed in this audit commit. No QA gate is upgraded to FULL / FINAL.**
