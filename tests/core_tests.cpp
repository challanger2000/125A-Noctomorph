#include "NoctomorphCore.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
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
    bool releaseHalfway = false,
    std::size_t blockSize = 257) {

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

    const std::size_t block = std::max<std::size_t>(1, blockSize);
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
        auto r = render(0xABCDEFULL, p, sr, 12.0);
        assert(allFinite(r));
        for (float x : r.left) assert(std::fabs(x) <= 0.892f);
        for (float x : r.right) assert(std::fabs(x) <= 0.892f);
    }
}


void testBlockSizeInvariance() {
    noctomorph::Parameters p;
    p.evolve = 0.71f;
    p.events = 0.43f;
    p.space = 0.57f;

    const auto reference = render(0xB10C5EEDULL, p, 48000.0, 4.0, false, 1);
    for (std::size_t block : {16u, 64u, 257u, 1024u}) {
        const auto candidate = render(0xB10C5EEDULL, p, 48000.0, 4.0, false, block);
        assert(reference.left == candidate.left);
        assert(reference.right == candidate.right);
        assert(reference.events == candidate.events);
    }
}

void testFullWetSpaceHasNoImmediateDryLeak() {
    noctomorph::Engine e;
    e.prepare(48000.0);
    e.reset(0x5AACEULL);

    noctomorph::Parameters p;
    p.foundation = 0.75f;
    p.world = 0.0f;
    p.texture = 0.0f;
    p.body = 0.0f;
    p.events = 0.0f;
    p.space = 1.0f;
    p.output = 0.5f;
    e.setParameters(p);
    e.noteOn(36, 1.0f);

    std::vector<float> l(1024), r(1024);
    e.process(l.data(), r.data(), l.size());

    for (std::size_t i = 0; i < l.size(); ++i) {
        assert(std::fabs(l[i]) < 1.0e-12f);
        assert(std::fabs(r[i]) < 1.0e-12f);
    }
}


void testWorldClipStereoAndRateConversion() {
    constexpr std::size_t frames = 4096;
    std::vector<float> srcL(frames);
    std::vector<float> srcR(frames);
    for (std::size_t i = 0; i < frames; ++i) {
        const float phase = static_cast<float>(i) / static_cast<float>(frames);
        srcL[i] = 0.4f * std::sin(6.28318530718f * 7.0f * phase);
        srcR[i] = 0.4f * std::sin(6.28318530718f * 11.0f * phase);
    }

    noctomorph::Clip clip;
    clip.left = srcL.data();
    clip.right = srcR.data();
    clip.frames = frames;
    clip.sampleRate = 24000.0;
    clip.loop = true;

    auto e = std::make_unique<noctomorph::Engine>();
    e->prepare(48000.0);
    e->reset(0x574F524C44ULL);

    noctomorph::Parameters p;
    p.foundation = 0.0f;
    p.world = 1.0f;
    p.texture = 0.0f;
    p.body = 0.0f;
    p.tension = 0.0f;
    p.evolve = 0.0f;
    p.events = 0.0f;
    p.space = 0.0f;
    p.output = 0.5f;
    e->setParameters(p);
    e->setWorldClip(&clip);
    e->noteOn(36, 1.0f);

    std::vector<float> l(8192), r(8192);
    e->process(l.data(), r.data(), l.size());

    for (float x : l) assert(std::isfinite(x));
    for (float x : r) assert(std::isfinite(x));
    assert(rms(l) > 1.0e-4);
    assert(rms(r) > 1.0e-4);

    double diffEnergy = 0.0;
    for (std::size_t i = 0; i < l.size(); ++i) {
        const double d = static_cast<double>(l[i]) - static_cast<double>(r[i]);
        diffEnergy += d * d;
    }
    assert(diffEnergy > 1.0e-6);
}

void testEventClipActuallyRenders() {
    constexpr std::size_t frames = 2048;
    std::vector<float> mono(frames, 0.0f);
    for (std::size_t i = 0; i < frames; ++i)
        mono[i] = 0.3f * std::exp(-0.004f * static_cast<float>(i));

    noctomorph::Clip clip;
    clip.left = mono.data();
    clip.right = nullptr;
    clip.frames = frames;
    clip.sampleRate = 48000.0;
    clip.loop = false;

    auto e = std::make_unique<noctomorph::Engine>();
    e->prepare(48000.0);
    e->reset(0x4556454E54ULL);

    noctomorph::Parameters p;
    p.foundation = 0.0f;
    p.world = 0.0f;
    p.texture = 0.0f;
    p.body = 0.0f;
    p.tension = 0.0f;
    p.evolve = 1.0f;
    p.events = 1.0f;
    p.space = 0.0f;
    p.output = 0.5f;
    e->setParameters(p);
    e->setEventClip(&clip);
    e->noteOn(36, 1.0f);

    constexpr std::size_t total = 48000 * 20;
    std::vector<float> l(257), r(257);
    double energy = 0.0;
    std::size_t done = 0;
    while (done < total) {
        const std::size_t n = std::min<std::size_t>(l.size(), total - done);
        e->process(l.data(), r.data(), n);
        for (std::size_t i = 0; i < n; ++i) {
            assert(std::isfinite(l[i]));
            assert(std::isfinite(r[i]));
            energy += static_cast<double>(l[i]) * l[i] +
                      static_cast<double>(r[i]) * r[i];
        }
        done += n;
    }

    assert(e->eventCount() > 0);
    assert(energy > 1.0e-4);
}


void testRealBodyExciterChangesModalResponse() {
    constexpr std::size_t frames = 4096;
    std::vector<float> exciter(frames, 0.0f);
    for (std::size_t i = 0; i < frames; ++i) {
        const float t = static_cast<float>(i);
        exciter[i] =
            0.7f * std::exp(-0.0035f * t) *
            std::sin(6.28318530718f * 173.0f * t / 48000.0f);
    }

    noctomorph::Clip clip;
    clip.left = exciter.data();
    clip.right = nullptr;
    clip.frames = frames;
    clip.sampleRate = 48000.0;
    clip.loop = false;

    auto renderBody = [&](bool withRealExciter) {
        auto e = std::make_unique<noctomorph::Engine>();
        e->prepare(48000.0);
        e->reset(0x424F4459455843ULL);

        noctomorph::Parameters p;
        p.foundation = 0.0f;
        p.world = 0.0f;
        p.texture = 0.0f;
        p.body = 0.75f;
        p.tension = 0.45f;
        p.evolve = 0.0f;
        p.events = 0.0f;
        p.space = 0.0f;
        p.output = 0.5f;
        e->setParameters(p);
        if (withRealExciter)
            e->setBodyExciterClip(&clip);
        e->noteOn(36, 1.0f);

        std::vector<float> l(24000), rr(24000);
        e->process(l.data(), rr.data(), l.size());
        return l;
    };

    const auto syntheticOnly = renderBody(false);
    const auto realDriven = renderBody(true);

    double diff = 0.0;
    for (std::size_t i = 0; i < syntheticOnly.size(); ++i) {
        const double d =
            static_cast<double>(syntheticOnly[i]) -
            static_cast<double>(realDriven[i]);
        diff += d * d;
    }

    assert(diff > 1.0e-5);
    assert(rms(realDriven) > 1.0e-5);
}


void testBodyExciterDrivesModalBank() {
    constexpr std::size_t frames = 16384;
    std::vector<float> source(frames);
    for (std::size_t i = 0; i < frames; ++i) {
        const float time = static_cast<float>(i) / 48000.0f;
        const float carrier =
            0.22f * std::sin(6.28318530718f * 731.0f * time) +
            0.14f * std::sin(6.28318530718f * 1187.0f * time);
        const float pulse =
            (i % 997u) < 18u
                ? 0.35f * (1.0f - static_cast<float>(i % 997u) / 18.0f)
                : 0.0f;
        source[i] = carrier + pulse;
    }

    noctomorph::Clip clip {source.data(), nullptr, frames, 48000.0, true};

    auto makeEngine = [&]() {
        auto e = std::make_unique<noctomorph::Engine>();
        e->prepare(48000.0);
        e->reset(0x424F445945584349ULL);
        noctomorph::Parameters p;
        p.foundation = 0.0f;
        p.world = 0.0f;
        p.texture = 0.0f;
        p.body = 0.75f;
        p.tension = 0.45f;
        p.evolve = 0.0f;
        p.events = 0.0f;
        p.space = 0.0f;
        p.output = 0.5f;
        e->setParameters(p);
        e->setArchetype(noctomorph::Archetype::Industrial);
        return e;
    };

    auto syntheticOnly = makeEngine();
    auto realDriven = makeEngine();
    realDriven->setBodyExciterClip(&clip);
    syntheticOnly->noteOn(36, 1.0f);
    realDriven->noteOn(36, 1.0f);

    constexpr std::size_t total = 48000 * 3;
    std::vector<float> aL(257), aR(257), bL(257), bR(257);
    long double syntheticEnergy = 0.0;
    long double realEnergy = 0.0;
    std::size_t done = 0;

    while (done < total) {
        const auto n = std::min<std::size_t>(257, total - done);
        syntheticOnly->process(aL.data(), aR.data(), n);
        realDriven->process(bL.data(), bR.data(), n);
        for (std::size_t i = 0; i < n; ++i) {
            assert(std::isfinite(bL[i]));
            assert(std::isfinite(bR[i]));
            if (done + i > 24000) {
                syntheticEnergy += static_cast<long double>(aL[i]) * aL[i] +
                                   static_cast<long double>(aR[i]) * aR[i];
                realEnergy += static_cast<long double>(bL[i]) * bL[i] +
                              static_cast<long double>(bR[i]) * bR[i];
            }
        }
        done += n;
    }

    assert(realEnergy > syntheticEnergy * 1.5L);
    assert(realEnergy > 1.0e-5L);
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
    testBlockSizeInvariance();
    testFullWetSpaceHasNoImmediateDryLeak();
    testWorldClipStereoAndRateConversion();
    testEventClipActuallyRenders();
    testRealBodyExciterChangesModalResponse();
    testReleaseDecays();

    std::cout << "Noctomorph core tests: PASS\n";
    return 0;
}
