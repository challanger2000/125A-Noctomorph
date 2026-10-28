# 125A Noctomorph — GUI redesign concept, phase 1 (2026-10-29)

**Status: implemented proposal, pending real Windows render and owner visual approval.**

## Fixed intent
- Use the excellent original V4JB 0.0.4 synthesis engine without changing audio code, factory presets, MIDI, parameter IDs or project state.
- Replace the "fog at night" purple/amber visual language with 125A industrial-dark: anthracite and graphite, restrained steel blue, silver text, clean fine borders.
- Reuse the authoritative `125A_Logo_Master_FINAL.svg` unchanged, sourced from `125A-Branding`. The previous V4JB logo is no longer the UI brand.
- No gratuitous faux-vintage controls, thick neon borders, multiple competing button sizes, or unrelated layout reflow.
- Preserve resizable editor and four existing functional views: SOUND (old EDIT), MODULATION, MORPH, WAVES.

## Geometry and hierarchy (original 1220 × 800 canvas retained)
1. Header 64px: 125A master logo; product descriptor and version; preset previous/name/next; save and randomizer; poly/transpose/glide/economy/output.
2. SOUND page, top ~44%: spectral shaping and effects/master remain present and fully wired. They share one clearly separated row.
3. SOUND page, lower area: module bank selector `01–05 / 06–10`, **five large cards in a single row** (instead of ten crowded cards in two rows). Cards still expose original module selection, main, four parameters and MOD routes. All ten remain live and automatable, including when hidden.
4. MODULATION keeps four envelopes and three LFOs.
5. MORPH retains original XY pad, paths and macro controls.
6. WAVES retains the two analyzed file sources and controls.

## Functional invariants
- No APVTS additions, removals, ID changes, presets or DSP edits.
- Bank selector is *editor-only presentation state*: switching bank does not change sound or save/recall semantics.
- Host automation and preset loads continue to update controls even on the hidden bank.
- Existing on/off, reset/default and host attachments remain unchanged.
- Factory presets and binary WAV assets are still taken from immutable upstream commit `2b3bc12400ad3337ba6d8f2e37dc61672c39b951`.
- All code paths, dark-mode readability, dimensions, touch targets and JUCE 9.0.4 compatibility require a genuine build and attached-editor screenshot.

## QA plan
- Check exact diff: only `src/ui/*`, `src/PluginEditor.*`, Projucer GUI resource and brand SVG; no `src/dsp/*`, `src/parameters/*` or `src/PluginProcessor.*`.
- Build Windows VST3 manually **only after** consolidating source changes; no automatic Actions triggers.
- Test bank switching (01-05/06-10), parameter values across bank changes, preset load while bank 2 visible, hidden controls receiving automation, MOD route visibility, scaling 80/100/130/150%, hover/input, editor close/reopen.
- Collect **real JUCE render screenshots** and independent user visual acceptance before further redesign.
- Sound equivalence with the exact upstream engine and project recall are separate tests; neither is established by cosmetic source edits.

## Naming and licensing
Current product naming remains provisional; original V4JB MIT notice stays in `LICENSE`. No publication as a release.
