#include "NoctomorphCore.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace {

std::uint64_t fnv1a(std::uint64_t h, std::uint16_t v) noexcept {
    h ^= static_cast<std::uint8_t>(v & 0xFFu);
    h *= 1099511628211ULL;
    h ^= static_cast<std::uint8_t>((v >> 8) & 0xFFu);
    h *= 1099511628211ULL;
    return h;
}

std::int16_t quantize(float x) noexcept {
    x = std::clamp(x, -1.0f, 1.0f);
    return static_cast<std::int16_t>(std::lround(x * 32767.0f));
}

} // namespace

int main() {
    constexpr double sampleRate = 48000.0;
    constexpr int seconds = 300;
    constexpr std::size_t block = 257;
    constexpr std::size_t samplesPerSecond = 48000;

    noctomorph::Engine engine;
    engine.prepare(sampleRate);
    engine.reset(0x125A45564F4C5645ULL);

    noctomorph::Parameters p;
    p.foundation = 0.58f;
    p.world = 0.0f;
    p.texture = 0.30f;
    p.body = 0.55f;
    p.tension = 0.42f;
    p.evolve = 0.72f;
    p.events = 0.38f;
    p.space = 0.48f;
    p.output = 0.50f;

    engine.setParameters(p);
    engine.setArchetype(noctomorph::Archetype::Nocturne);
    engine.noteOn(36, 0.9f);

    std::vector<float> left(block);
    std::vector<float> right(block);

    std::vector<std::uint64_t> secondHashes;
    secondHashes.reserve(seconds);

    std::uint64_t hash = 1469598103934665603ULL;
    std::size_t secondSample = 0;
    std::size_t totalSamples = static_cast<std::size_t>(seconds * sampleRate);
    std::size_t rendered = 0;

    long double energy = 0.0;
    float peak = 0.0f;
    bool finite = true;

    std::array<std::uint64_t, 5> minuteEvents {};
    std::size_t nextMinuteSample = static_cast<std::size_t>(60.0 * sampleRate);
    std::size_t minuteIndex = 0;

    while (rendered < totalSamples) {
        const std::size_t n = std::min(block, totalSamples - rendered);
        engine.process(left.data(), right.data(), n);

        for (std::size_t i = 0; i < n; ++i) {
            const float l = left[i];
            const float r = right[i];
            finite = finite && std::isfinite(l) && std::isfinite(r);
            peak = std::max(peak, std::max(std::fabs(l), std::fabs(r)));
            energy += static_cast<long double>(l) * l;
            energy += static_cast<long double>(r) * r;

            const auto ql = static_cast<std::uint16_t>(quantize(l));
            const auto qr = static_cast<std::uint16_t>(quantize(r));
            hash = fnv1a(hash, ql);
            hash = fnv1a(hash, qr);

            ++secondSample;
            if (secondSample == samplesPerSecond) {
                secondHashes.push_back(hash);
                hash = 1469598103934665603ULL;
                secondSample = 0;
            }
        }

        rendered += n;

        while (minuteIndex < minuteEvents.size() && rendered >= nextMinuteSample) {
            minuteEvents[minuteIndex] = engine.eventCount();
            ++minuteIndex;
            nextMinuteSample += static_cast<std::size_t>(60.0 * sampleRate);
        }
    }

    std::unordered_set<std::uint64_t> unique(secondHashes.begin(), secondHashes.end());
    const std::size_t duplicateSeconds = secondHashes.size() - unique.size();
    const double rms = std::sqrt(
        static_cast<double>(energy / static_cast<long double>(2 * totalSamples)));

    bool minuteProgress = true;
    std::uint64_t previous = 0;
    for (std::uint64_t count : minuteEvents) {
        if (count <= previous)
            minuteProgress = false;
        previous = count;
    }

    std::cout
        << "Noctomorph 5-minute evolution probe\n"
        << "finite=" << (finite ? "yes" : "no") << "\n"
        << "peak=" << peak << "\n"
        << "rms=" << rms << "\n"
        << "events=" << engine.eventCount() << "\n"
        << "second_windows=" << secondHashes.size() << "\n"
        << "duplicate_second_hashes=" << duplicateSeconds << "\n"
        << "events_by_minute="
        << minuteEvents[0] << ","
        << minuteEvents[1] << ","
        << minuteEvents[2] << ","
        << minuteEvents[3] << ","
        << minuteEvents[4] << "\n";

    if (!finite)
        return 1;
    if (peak > 0.892f)
        return 2;
    if (rms <= 1.0e-6)
        return 3;
    if (duplicateSeconds != 0)
        return 4;
    if (engine.eventCount() < 5 || engine.eventCount() > 300)
        return 5;
    if (!minuteProgress)
        return 6;

    return 0;
}
