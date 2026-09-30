#include "NoctomorphCore.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

struct RenderResult {
    std::vector<float> left;
    std::vector<float> right;
    std::uint64_t events = 0;
};

bool allFinite(const RenderResult& r) {
    for (float x : r.left) if (!std::isfinite(x)) return false;
    for (float x : r.right) if (!std::isfinite(x)) return false;
    return true;
}

double rms(const std::vector<float>& x) {
    long double sum = 0.0;
    for (float v : x) sum += static_cast<long double>(v) * v;
    return x.empty() ? 0.0 : std::sqrt(static_cast<double>(sum / x.size()));
}

RenderResult render(
    std::uint64_t seed,
    const noctomorph::Parameters& p,
    double sampleRate,
    double seconds,
    bool releaseHalfway = false) {

    auto e = std::make_unique<noctomorph::Engine>();
    e->prepare(sampleRate);
    e->reset(seed);
    e->setParameters(p);
    e->setArchetype(noctomorph::Archetype::Nocturne);
    e->noteOn(36, 0.9f);

    const std::size_t frames = static_cast<std::size_t>(seconds * sampleRate);
    RenderResult result;
    result.left.resize(frames);
    result.right.resize(frames);

    constexpr std::size_t block = 257;
    std::size_t offset = 0;
    while (offset < frames) {
        if (releaseHalfway && offset >= frames / 2 && e->active())
            e->noteOff();

        const std::size_t n = std::min(block, frames - offset);
        e->process(result.left.data() + offset, result.right.data() + offset, n);
        offset += n;
    }

    result.events = e->eventCount();
    return result;
}

void testDeterminism() {
    noctomorph::Parameters p;
    p.evolve = 0.78f;
    p.events = 0.55f;

    auto a = render(0x12345678ULL, p, 48000.0, 8.0);
    auto b = render(0x12345678ULL, p, 48000.0, 8.0);

    assert(a.left == b.left);
    assert(a.right == b.right);
    assert(a.events == b.events);
}

void testSeedVariation() {
    noctomorph::Parameters p;
    p.evolve = 0.8f;
    p.events = 0.6f;

    auto a = render(1ULL, p, 48000.0, 6.0);
    auto b = render(2ULL, p, 48000.0, 6.0);

    bool differs = false;
    for (std::size_t i = 0; i < a.left.size(); ++i) {
        if (a.left[i] != b.left[i]) {
            differs = true;
            break;
        }
    }
    assert(differs);
}

void testZeroSemantics() {
    noctomorph::Parameters p;
    p.foundation = 0.0f;
    p.world = 0.0f;
    p.texture = 0.0f;
    p.body = 0.0f;
    p.tension = 0.0f;
    p.evolve = 0.0f;
    p.events = 0.0f;
    p.space = 0.0f;
    p.output = 0.5f;

    auto r = render(42ULL, p, 48000.0, 2.0);
    assert(rms(r.left) < 1.0e-12);
    assert(rms(r.right) < 1.0e-12);
    assert(r.events == 0);
}

void testDefaultIsAliveAndFinite() {
    noctomorph::Parameters p;
    auto r = render(99ULL, p, 48000.0, 4.0);
    assert(allFinite(r));
    assert(rms(r.left) > 1.0e-5);
    assert(rms(r.right) > 1.0e-5);
}

void testExtremeFiniteAcrossRates() {
    noctomorph::Parameters p;
    p.foundation = 1.0f;
    p.world = 1.0f;
    p.texture = 1.0f;
    p.body = 1.0f;
    p.tension = 1.0f;
    p.evolve = 1.0f;
    p.events = 1.0f;
    p.space = 1.0f;
    p.output = 1.0f;

    for (double sr : {44100.0, 48000.0, 96000.0, 192000.0}) {
        auto r = render(0xABCDEFULL, p, sr, 3.0);
        assert(allFinite(r));
        for (float x : r.left) assert(std::fabs(x) <= 1.0f);
        for (float x : r.right) assert(std::fabs(x) <= 1.0f);
    }
}

void testReleaseDecays() {
    noctomorph::Parameters p;
    p.space = 0.0f;
    p.events = 0.0f;

    auto r = render(31337ULL, p, 48000.0, 10.0, true);

    const std::size_t tailStart = static_cast<std::size_t>(9.0 * 48000.0);
    std::vector<float> tail(r.left.begin() + static_cast<std::ptrdiff_t>(tailStart), r.left.end());
    assert(rms(tail) < 0.01);
}

} // namespace

int main() {
    testDeterminism();
    testSeedVariation();
    testZeroSemantics();
    testDefaultIsAliveAndFinite();
    testExtremeFiniteAcrossRates();
    testReleaseDecays();

    std::cout << "Noctomorph core tests: PASS\n";
    return 0;
}
