# 125A-Noctomorph

125A Spectral Synthesizer – Advanced additive synthesis for evolving pads, dark atmospheres, drones and cinematic textures. Based on V4JB, with a redesigned, streamlined interface.

**Windows QA release candidate, NOT published/FINAL.** Original V4JB 0.0.4 is the compatible baseline; the 125A user interface and branding are separate from the original MIT-licensed project. The candidate requires a successful Windows VST3 build, real Steinberg validator, and Studio One save/close/reopen acceptance before a release declaration. The original manufacturer/plugin ID is retained for host project compatibility.

See [BASELINE.md](BASELINE.md) for source provenance, toolchain pinning, external factory-resource handling and QA requirements. Original V4JB source is MIT licensed (see [LICENSE](LICENSE)).

## Stufenloses GUI-Resize (ohne Zoom-Buttons)

This JUCE editor already supports smooth, continuous resizing by dragging the window's bottom-right corner rather than choosing a fixed zoom percentage. The implementation is documented in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) and in the central 125A guideline [`125A-Engineering/STANDARDS/GUI-GUIDELINES.md`](https://github.com/challanger2000/125A-Engineering/blob/main/STANDARDS/GUI-GUIDELINES.md) (section *Continuous GUI resize*).

**Owner status (2026-10-29):** acceptable **for private use as-is**. No new QA, feature work, or public release is requested. Windows VST3 CI passed 21/21 processor tests and the actual Steinberg validator passed 47/47; free resizing and project reload in Studio One have not been independently checked and must not be labelled host-PASS.
