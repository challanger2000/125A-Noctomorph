# Host BPM normalization — isolated safety fix

Fix branch: `fix/host-tempo-guard-20261029` based on the current GUI+audit development HEAD, no GUI or preset edits.

- `src/dsp/TransportSafety.h`: pure C++17, no JUCE dependency, `safeHostBpm`.
- Invalid / unreasonably tiny or large BPM (not finite, <=0, <1 or >1000) become 120 BPM. Normal musical rates from 1 to 1000 BPM are **unchanged**.
- `src/PluginProcessor.cpp` normalizes host-reported BPM immediately before `Engine::setTransport`; the same normalized value reaches the synced delay.
- `tests/transport_safety_check.cpp` rejects nonfinite delay seconds and checks values at boundaries without relying on `assert` (works in release builds with NDEBUG).

### Evidence

Local Linux g++17 `-O2 -DNDEBUG -Wall -Wextra -Werror -fsanitize=address,undefined`: `transport_safety_check OK` (one run; sanitizer error count 0).

### Still NOT RUN

JUCE-linked plugin processor and VST3 host test, Steinberg validator, Studio One offline/realtime, MIDI sync through host callback. This is an **isolated candidate**, not approved release; do not merge without review. After consolidating other safety fixes, run the necessary Windows CI once rather than making numerous per-commit builds.
