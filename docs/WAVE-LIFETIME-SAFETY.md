# WAV retirement safety — isolated candidate

Scope: `fix/wave-lifetime-20261029`, based on GUI + audit development commit `a855d2d477f699326572646265e60d6728bd78f0`.

**Defect:** Old `WaveData` objects were retired when either 2 callbacks completed OR 10 s wall-clock time had elapsed. A blocked/suspended audio callback could still hold the old raw pointer after the timeout; the timer could destroy the object and provoke use-after-free.

**Change:** Remove *only* the unsafe wall-clock reclamation branch. Retire after two completed audio callbacks. `WaveRetirePolicy.h` is JUCE-free and covered by a deterministic stalled-callback test that keeps old data alive until two completed blocks. No locks, allocations, sleeps or blocking waits were added to `processBlock`, and no DSP algorithms changed. `waveLive` remains the original atomic raw pointer.

**Local check:** g++17 with `-O1 -g -Wall -Wextra -Werror -pthread -fsanitize=address,undefined`: `wave_retire_policy_check OK`. The policy regression is added to the existing **manual-only** Windows workflow. No Windows VST3 build executed for this branch.

**Open issue:** While a host stops invoking processBlock, retired data is held until callbacks resume or processor destruction; repeated loads could accumulate memory. A proven lifecycle/epoch strategy is still required before labeling the complete WAV subsystem safe. No JUCE-linked stalled-callback stress, Steinberg validator or Studio One recall executed. This candidate therefore requires independent review and is **NOT FINAL**.

**Regression expectations:** restart/play/stop; many WAV replacements during sustained stalled callback; valid and failed imports; host state-recall. Strictly distinguish the policy unit test from end-to-end race testing.
