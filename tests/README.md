# Noctomorph audit tests

These targets compile the real processor and editor, with JUCE 9.0.4 and all
494 original factory presets. They do not replace the Projucer Windows product
build and do not constitute a VST3 host or Studio One test.

Dependencies: CMake >=3.22, C++17, Ninja or Visual Studio 2022. On Debian the
JUCE test build needs `libasound2-dev libx11-dev libxext-dev libxi-dev
libxrandr-dev libxinerama-dev libxcursor-dev libfreetype-dev libfontconfig1-dev
libgl1-mesa-dev xvfb`. A display is needed for the editor tests.

Use independent dependency checkouts outside the product checkout:

```sh
git clone --branch 9.0.4 --depth 1 https://github.com/juce-framework/JUCE.git /tmp/noctomorph-JUCE
# JUCE tag resolves to 3fb5cbc0340758596dac39f7c8de47d562b4896e
git clone --no-checkout https://github.com/iamtew/V4JB.git /tmp/noctomorph-upstream
git -C /tmp/noctomorph-upstream checkout --detach 2b3bc12400ad3337ba6d8f2e37dc61672c39b951
cmake -S tests -B build/audit -G Ninja \
  -DJUCE_SOURCE=/tmp/noctomorph-JUCE \
  -DV4JB_ASSETS=/tmp/noctomorph-upstream \
  -DCMAKE_BUILD_TYPE=Debug -DAUDIT_SANITIZERS=ON
cmake --build build/audit --parallel 2
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 \
  xvfb-run -a ctest --test-dir build/audit --output-on-failure
```

In this cloud instance the dependencies were downloaded against signed Debian
indexes and extracted without root into `/tmp/noctomorph-sysroot`; CMake/Ninja
are in `/tmp/noctomorph-tools`. The completed build is `build/audit`. Xvfb was
started on `:91`. Rebuild with `CPATH` pointing to the sysroot's `usr/include`
and `usr/include/freetype2`, `LIBRARY_PATH` to `usr/lib/x86_64-linux-gnu`, and
`PKG_CONFIG_PATH` to that directory's `pkgconfig` plus `usr/share/pkgconfig`.
Use `DISPLAY=:91` for tests. Sandbox ptrace prevents LeakSanitizer from running;
the recorded sanitizer runs used the approved execution outside that sandbox,
without disabling leak detection.

For Windows use `cmake -S tests -B build/audit-tests -A x64` with the same
dependency paths, build `--config Release`, and run `ctest -C Release`.
Checks use unconditional `CHECK`, not assertions removed by NDEBUG.
The manual Windows workflow now includes these tests and the official SDK
3.8.1 validator, pinned to `3cdf9ca5d1f5b1b21e0a86832aa4abe55607bd96`.
This workflow's new steps remain Windows NOT RUN until an actual run completes.

`processor_check` accepts `blocks`, `keys`, `wet`, `tempo`, `state`, `gui`,
`waves`, `realtime`, `presets`, or `midi`. The GUI probe creates
`control-inventory.tsv` in its working directory; run it from the build directory.
It exercises attachments and native JUCE event handlers. It does not assert
every widget's sonic effect or simulate the DAW's actual mouse routing.
The `realtime` probe counts C++ allocations and, on Linux, wrapped
malloc/calloc/realloc calls during one 32769-frame callback containing SysEx
and a note. This is a narrow heap regression, not a deadline qualification.

The before/fix evidence is in `docs/audit-evidence`. Original product sources
were exported using `git archive a855d2d477f699326572646265e60d6728bd78f0`,
without resetting the working branch or creating a worktree. `PRODUCT_SOURCE`
can point at such an export to build `processor_check` against it. The new
AudioReadSlot standalone target requires the fixed source, so build only
`processor_check engine_check version_check` for the old export. GUI component
IDs were added as instrumentation: their absence in old exports is not proof
of disconnected controls. The original GUI failure evidence concerns the stale
Morph selection, recorded before the inventory was expanded.
