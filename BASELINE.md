# 125A Noctomorph - V4JB GUI redesign baseline

Status: **development only**. Public source repository, currently for private use; no product release and no claimed QA PASS.

## Sources and attribution
- Upstream: [iamtew/V4JB](https://github.com/iamtew/V4JB)
- Exact baseline: tag `0.0.4`, commit `2b3bc12400ad3337ba6d8f2e37dc61672c39b951`.
- License: MIT; original copyright and license are preserved in `LICENSE`.
- The 125A branch contains upstream C++ sources and the original Projucer project as an editable starting point.
- The factory XML presets and binary resources (including WAVs and the former GUI logo) remain unmodified and are fetched at build time from the **pinned upstream commit**. They are not yet vendored into this repository. No dependency on upstream HEAD.

## Toolchain
- Windows x64 / Visual Studio 2022 runner
- JUCE **9.0.4**, pinned (original V4JB build used 9.0.3). JUCE provides the VST3 plugin integration.
- Separate Steinberg VST3 SDK **3.8.1** is a validation target; this workflow does not yet run the validator. The bundled SDK revision inside JUCE is not independently asserted to match 3.8.1.
- Original Projucer, C++17, VST3 instrument target; no untested conversion to CMake/VSTGUI.

## Actions budget
Only `workflow_dispatch`, never automatic build on push or PR. Configure the workflow on the repository's default branch before running it through GitHub's Actions UI. No build has been executed by creating these files.

## GUI scope
Keep audio engine, all module chains, 494 factory presets, wave analysis, modulation, morphing, automation IDs and state logic unchanged for initial baseline. Replace only visual hierarchy and user control exposure, beginning with a static approved 125A design. Use the authoritative master logo in `125A-Branding`. Controls should not be disconnected from their original parameters.

## Gates before calling the plugin ready
1. First successful and measured baseline Windows build, DSP tests and VST3 validator.
2. Baseline screenshot and MIDI/PCM comparison against upstream at matching settings.
3. GUI-to-parameter binding tests, DAW editor lifecycle, state recall and scaling checks.
4. Owner's listening and Studio One acceptance.

All unexecuted checks are **NOT RUN**, not PASS.
