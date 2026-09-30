#include "NoctomorphCore.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

namespace {

bool runCase(double sampleRate, std::size_t blockSize) {
    constexpr double renderedSeconds = 8.0;
    const std::size_t totalFrames =
        static_cast<std::size_t>(sampleRate * renderedSeconds);

    auto engine = std::make_unique<noctomorph::Engine>();
    engine->prepare(sampleRate);
    engine->reset(0x504552464F524DULL);

    noctomorph::Parameters p;
    p.foundation = 0.75f;
    p.world = 0.0f;
    p.texture = 0.55f;
    p.body = 0.80f;
    p.tension = 0.80f;
    p.evolve = 0.90f;
    p.events = 0.55f;
    p.space = 0.80f;
    p.output = 0.50f;
    engine->setParameters(p);
    engine->setArchetype(noctomorph::Archetype::Nocturne);
    engine->noteOn(36, 1.0f);

    std::vector<float> left(blockSize);
    std::vector<float> right(blockSize);

    // Warm up caches and control state.
    for (int i = 0; i < 32; ++i)
        engine->process(left.data(), right.data(), blockSize);

    const auto start = std::chrono::steady_clock::now();
    std::size_t rendered = 0;
    double checksum = 0.0;

    while (rendered < totalFrames) {
        const std::size_t n = std::min(blockSize, totalFrames - rendered);
        engine->process(left.data(), right.data(), n);
        checksum += left[n - 1] + right[n - 1];
        rendered += n;
    }

    const auto stop = std::chrono::steady_clock::now();
    const double elapsed =
        std::chrono::duration<double>(stop - start).count();
    const double realtimeFraction = elapsed / renderedSeconds;
    const double realtimeMultiple =
        elapsed > 0.0 ? renderedSeconds / elapsed : 999.0;
    const double nsPerSample =
        1.0e9 * elapsed / static_cast<double>(totalFrames);

    std::cout
        << std::fixed << std::setprecision(4)
        << "sr=" << sampleRate
        << " block=" << blockSize
        << " elapsed=" << elapsed << "s"
        << " realtime_fraction=" << realtimeFraction
        << " realtime_multiple=" << realtimeMultiple
        << " ns_per_sample=" << nsPerSample
        << " checksum=" << checksum
        << "\n";

    // Deliberately generous CI gate: one synthetic Nocturne instance must
    // remain faster than realtime even on a shared runner. Detailed p95/p99
    // host profiling comes later with the VST3 wrapper.
    return std::isfinite(checksum) && realtimeFraction < 1.0;
}

} // namespace

int main() {
    bool ok = true;
    for (double sr : {48000.0, 96000.0, 192000.0}) {
        for (std::size_t block : {64u, 257u, 1024u})
            ok = runCase(sr, block) && ok;
    }

    std::cout << "Noctomorph performance probe: "
              << (ok ? "PASS" : "FAIL") << "\n";
    return ok ? 0 : 1;
}
