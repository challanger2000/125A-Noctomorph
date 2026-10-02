#include "NoctomorphCore.h"

#include <algorithm>
#include <cmath>

namespace noctomorph {
namespace {

constexpr double kTwoPi = 6.283185307179586476925286766559;
constexpr float kSqrtHalf = 0.7071067811865475244f;

struct ArchetypeTraits {
    float foundationGain;
    float worldGain;
    float textureGain;
    float bodyGain;
    float eventRate;
    float worldRate;
    float textureRate;
};

ArchetypeTraits traitsFor(Archetype archetype) noexcept {
    switch (archetype) {
        case Archetype::Void:
            return {0.22f, 0.00f, 1.30f, 0.28f, 0.35f, 0.64f, 0.58f};
        case Archetype::Ruins:
            return {0.28f, 1.10f, 0.45f, 0.78f, 0.70f, 0.76f, 0.70f};
        case Archetype::Industrial:
            return {0.18f, 1.30f, 1.45f, 0.72f, 1.20f, 1.03f, 1.12f};
        case Archetype::Wasteland:
            return {0.22f, 0.48f, 1.55f, 0.42f, 0.70f, 1.28f, 1.34f};
        case Archetype::Abyss:
            return {0.95f, 0.00f, 0.18f, 1.55f, 0.40f, 0.52f, 0.48f};
        case Archetype::Nocturne:
        default:
            return {0.42f, 0.22f, 0.58f, 0.68f, 0.55f, 0.72f, 0.66f};
    }
}

float sanitize01(float value) noexcept {
    if (!std::isfinite(value))
        return 0.0f;
    return std::clamp(value, 0.0f, 1.0f);
}

float softClip(float x) noexcept {
    if (!std::isfinite(x))
        return 0.0f;
    const float ax = std::fabs(x);
    return x / (1.0f + 0.35f * ax);
}

float safeOutput(float x) noexcept {
    if (!std::isfinite(x))
        return 0.0f;
    constexpr float kCeiling = 0.89125094f; // -1.0 dBFS
    return kCeiling * std::tanh(x / kCeiling);
}

} // namespace

void Engine::Rng::seed(std::uint64_t value) noexcept {
    state = value ? value : 0x125A4E4F43544F4DULL;
}

std::uint64_t Engine::Rng::nextU64() noexcept {
    std::uint64_t x = state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    state = x;
    return x * 2685821657736338717ULL;
}

float Engine::Rng::uniform01() noexcept {
    constexpr double scale = 1.0 / 9007199254740992.0;
    return static_cast<float>((nextU64() >> 11) * scale);
}

float Engine::Rng::bipolar() noexcept {
    return 2.0f * uniform01() - 1.0f;
}

void Engine::StreamVoice::reset() noexcept {
    clip = nullptr;
    position = 0.0;
    rate = 1.0;
    active = false;
}

void Engine::StreamVoice::start(
    const Clip* source,
    double playbackRate,
    double startPosition) noexcept {

    clip = source;
    if (!clip || !clip->left || clip->frames < 2 || !std::isfinite(playbackRate)) {
        reset();
        return;
    }

    rate = std::clamp(playbackRate, 0.05, 4.0);
    position = std::clamp(startPosition, 0.0, static_cast<double>(clip->frames - 2));
    active = true;
}

void Engine::StreamVoice::process(float& left, float& right) noexcept {
    left = 0.0f;
    right = 0.0f;

    if (!active || !clip || !clip->left || clip->frames < 2)
        return;

    const std::size_t i0 = static_cast<std::size_t>(position);
    const std::size_t i1 = std::min(i0 + 1, clip->frames - 1);
    const float frac = static_cast<float>(position - static_cast<double>(i0));

    const auto sampleAt = [&](const float* p) noexcept {
        const float a = p ? p[i0] : 0.0f;
        const float b = p ? p[i1] : 0.0f;
        return a + frac * (b - a);
    };

    left = sampleAt(clip->left);
    right = clip->right ? sampleAt(clip->right) : left;

    if (clip->loop && clip->frames > 64) {
        const std::size_t fadeFrames = std::min<std::size_t>(
            clip->frames / 4,
            std::max<std::size_t>(32, static_cast<std::size_t>(0.020 * clip->sampleRate)));

        const double fadeStart = static_cast<double>(clip->frames - fadeFrames - 1);
        if (position >= fadeStart) {
            const double rel = std::clamp(
                position - fadeStart,
                0.0,
                static_cast<double>(fadeFrames - 1));
            const float mix = static_cast<float>(
                rel / static_cast<double>(std::max<std::size_t>(1, fadeFrames - 1)));

            const std::size_t s0 = static_cast<std::size_t>(rel);
            const std::size_t s1 = std::min(s0 + 1, clip->frames - 1);
            const float sFrac = static_cast<float>(rel - static_cast<double>(s0));

            const auto startSample = [&](const float* p) noexcept {
                const float a = p ? p[s0] : 0.0f;
                const float b = p ? p[s1] : 0.0f;
                return a + sFrac * (b - a);
            };

            const float sl = startSample(clip->left);
            const float sr = clip->right ? startSample(clip->right) : sl;
            left = (1.0f - mix) * left + mix * sl;
            right = (1.0f - mix) * right + mix * sr;
        }
    }

    position += rate;
    if (position >= static_cast<double>(clip->frames - 1)) {
        if (clip->loop) {
            position = std::fmod(position, static_cast<double>(clip->frames - 1));
        } else {
            active = false;
        }
    }
}

void Engine::Resonator::reset() noexcept {
    z1L = z2L = z1R = z2R = 0.0f;
}

void Engine::Resonator::configure(
    double sampleRate,
    double frequencyHz,
    double decaySeconds,
    float gain,
    float stereoPan) noexcept {

    const double sr = std::max(sampleRate, 1.0);
    const double f = std::clamp(frequencyHz, 12.0, 0.45 * sr);
    const double decay = std::clamp(decaySeconds, 0.03, 25.0);

    // Radius chosen so amplitude is roughly -60 dB after decaySeconds.
    const double radius = std::exp(std::log(0.001) / (decay * sr));
    const double w = kTwoPi * f / sr;

    a1 = static_cast<float>(2.0 * radius * std::cos(w));
    a2 = static_cast<float>(-(radius * radius));
    b0 = std::clamp(std::isfinite(gain) ? gain : 0.0f, 0.0f, 2.0f);
    pan = std::clamp(stereoPan, -1.0f, 1.0f);
}

void Engine::Resonator::process(float excitation, float& left, float& right) noexcept {
    const float pan01 = 0.5f * (pan + 1.0f);
    const float gL = std::sqrt(std::max(0.0f, 1.0f - pan01));
    const float gR = std::sqrt(std::max(0.0f, pan01));

    const float inL = excitation * gL;
    const float inR = excitation * gR;

    const float yL = b0 * inL + a1 * z1L + a2 * z2L;
    const float yR = b0 * inR + a1 * z1R + a2 * z2R;

    z2L = z1L;
    z1L = std::isfinite(yL) ? yL : 0.0f;
    z2R = z1R;
    z1R = std::isfinite(yR) ? yR : 0.0f;

    left += z1L;
    right += z1R;
}

void Engine::DelayLine::reset() noexcept {
    data.fill(0.0f);
    index = 0;
}

void Engine::DelayLine::setLength(std::size_t samples) noexcept {
    length = std::clamp<std::size_t>(samples, 1, kCapacity);
    if (index >= length)
        index = 0;
}

float Engine::DelayLine::read() const noexcept {
    return data[index];
}

void Engine::DelayLine::write(float value) noexcept {
    data[index] = std::isfinite(value) ? value : 0.0f;
}

void Engine::DelayLine::advance() noexcept {
    ++index;
    if (index >= length)
        index = 0;
}

float Engine::clamp01(float value) noexcept {
    return sanitize01(value);
}

float Engine::midiToHz(int note) noexcept {
    const int clamped = std::clamp(note, 0, 127);
    return 440.0f * std::pow(2.0f, (static_cast<float>(clamped) - 69.0f) / 12.0f);
}

void Engine::prepare(double sampleRate) noexcept {
    sampleRate_ = std::clamp(sampleRate, 8000.0, 384000.0);

    const std::array<double, 4> seconds {0.071, 0.089, 0.113, 0.137};
    for (std::size_t i = 0; i < delays_.size(); ++i) {
        delays_[i].setLength(static_cast<std::size_t>(
            std::max(1.0, std::round(seconds[i] * sampleRate_))));
    }

    reset();
}

void Engine::reset(std::uint64_t seedValue) noexcept {
    rng_.seed(seedValue);
    presenceRng_.seed(seedValue ^ 0x50524553454E4345ULL);

    gate_ = false;
    midiNote_ = 36;
    velocity_ = 1.0f;
    envelope_ = 0.0f;

    oscillatorPhase_.fill(0.0);
    oscillatorDrift_.fill(0.0f);
    noiseStateL_ = 0.0f;
    noiseStateR_ = 0.0f;
    textureHpState_ = 0.0f;
    motionPan_ = 0.0f;
    motionSpectral_ = 0.0f;

    chaosX_ = 0.11 + 0.02 * rng_.bipolar();
    chaosY_ = 0.03 * rng_.bipolar();
    chaosZ_ = 0.03 * rng_.bipolar();
    controlCountdown_ = 0;
    structureCountdown_ = 0;

    for (auto& r : resonators_)
        r.reset();
    for (auto& r : presenceResonators_)
        r.reset();

    bodyExcitation_ = 0.0f;
    presenceNoiseLowpass_ = 0.0f;

    for (auto& voice : worldVoices_)
        voice.reset();
    for (auto& voice : textureVoices_)
        voice.reset();
    for (auto& voice : worldNextVoices_) voice.reset();
    for (auto& voice : textureNextVoices_) voice.reset();
    worldCrossfade_.fill(0.0f);
    textureCrossfade_.fill(0.0f);
    worldLayerGain_ = {0.72f, 0.24f, 0.08f};
    worldLayerTarget_ = worldLayerGain_;
    textureLayerGain_ = {0.58f, 0.30f, 0.12f};
    textureLayerTarget_ = textureLayerGain_;
    worldSceneInitialised_ = false;
    textureSceneInitialised_ = false;
    eventVoice_.reset();
    bodyExciterVoice_.reset();
    bodyExciterLowpass_ = 0.0f;

    eventCount_ = 0;
    eventCountdown_ = static_cast<int>(sampleRate_ * (2.0 + 3.0 * rng_.uniform01()));

    for (auto& d : delays_)
        d.reset();
    reverbDamping_.fill(0.0f);

    configureBody();
    configurePresence();
}

void Engine::setParameters(const Parameters& p) noexcept {
    parameters_.foundation = clamp01(p.foundation);
    parameters_.world = clamp01(p.world);
    parameters_.texture = clamp01(p.texture);
    parameters_.body = clamp01(p.body);
    parameters_.tension = clamp01(p.tension);
    parameters_.motion = clamp01(p.motion);
    parameters_.evolve = clamp01(p.evolve);
    parameters_.events = clamp01(p.events);
    parameters_.space = clamp01(p.space);
    parameters_.output = clamp01(p.output);
}

void Engine::setArchetype(Archetype archetype) noexcept {
    const auto index = static_cast<unsigned>(archetype);
    archetype_ = index < static_cast<unsigned>(Archetype::Count)
        ? archetype
        : Archetype::Nocturne;
    configureBody();
    configurePresence();
}

void Engine::setWorldPool(
    const Clip* const* clips, std::size_t count) noexcept {
    worldPool_.fill(nullptr);
    worldPoolCount_ = std::min(count, worldPool_.size());
    for (std::size_t i = 0; i < worldPoolCount_; ++i)
        worldPool_[i] = clips ? clips[i] : nullptr;
    for (auto& voice : worldVoices_)
        voice.reset();
    for (auto& voice : worldNextVoices_) voice.reset();
    worldCrossfade_.fill(0.0f);
    worldLayerGain_ = {0.72f, 0.24f, 0.08f};
    worldLayerTarget_ = worldLayerGain_;
    worldSceneInitialised_ = false;
}

void Engine::setTexturePool(
    const Clip* const* clips, std::size_t count) noexcept {
    texturePool_.fill(nullptr);
    texturePoolCount_ = std::min(count, texturePool_.size());
    for (std::size_t i = 0; i < texturePoolCount_; ++i)
        texturePool_[i] = clips ? clips[i] : nullptr;
    for (auto& voice : textureVoices_)
        voice.reset();
    for (auto& voice : textureNextVoices_) voice.reset();
    textureCrossfade_.fill(0.0f);
    textureLayerGain_ = {0.58f, 0.30f, 0.12f};
    textureLayerTarget_ = textureLayerGain_;
    textureSceneInitialised_ = false;
}

void Engine::setEventPool(
    const Clip* const* clips, std::size_t count) noexcept {
    eventPool_.fill(nullptr);
    eventPoolCount_ = std::min(count, eventPool_.size());
    for (std::size_t i = 0; i < eventPoolCount_; ++i)
        eventPool_[i] = clips ? clips[i] : nullptr;
    eventVoice_.reset();
}

void Engine::setBodyExciterPool(
    const Clip* const* clips, std::size_t count) noexcept {
    bodyPool_.fill(nullptr);
    bodyPoolCount_ = std::min(count, bodyPool_.size());
    for (std::size_t i = 0; i < bodyPoolCount_; ++i)
        bodyPool_[i] = clips ? clips[i] : nullptr;
    bodyExciterVoice_.reset();
    bodyExciterLowpass_ = 0.0f;
}

void Engine::setWorldClip(const Clip* clip) noexcept {
    const Clip* clips[1] {clip};
    setWorldPool(clips, clip ? 1u : 0u);
}

void Engine::setTextureClip(const Clip* clip) noexcept {
    const Clip* clips[1] {clip};
    setTexturePool(clips, clip ? 1u : 0u);
}

void Engine::setEventClip(const Clip* clip) noexcept {
    const Clip* clips[1] {clip};
    setEventPool(clips, clip ? 1u : 0u);
}

void Engine::setBodyExciterClip(const Clip* clip) noexcept {
    const Clip* clips[1] {clip};
    setBodyExciterPool(clips, clip ? 1u : 0u);
}

void Engine::noteOn(int midiNote, float velocity) noexcept {
    midiNote_ = std::clamp(midiNote, 0, 127);
    velocity_ = clamp01(velocity);
    gate_ = velocity_ > 0.0f;

    // Retrigger the acoustic world without resetting macro evolution.
    if (gate_) {
        ensureLongStreams();
        // Note-On starts the scene itself, not a recognizable recorded hit.
        // Real BODY exciters are reserved for explicit EVENTS.
        // INDUSTRIAL must open as a continuous environment, never as a struck
        // modal object. Other archetypes keep the legacy prototype excitation.
        if (archetype_ != Archetype::Industrial)
            bodyExcitation_ += 0.05f + 0.16f * velocity_;
    }
}

void Engine::noteOff() noexcept {
    gate_ = false;
}

void Engine::configureBody() noexcept {
    const float root = midiToHz(midiNote_);
    const float archetype = static_cast<float>(static_cast<unsigned>(archetype_));
    const float tension = parameters_.tension;

    static constexpr std::array<float, 12> baseRatios {
        0.50f, 0.76f, 1.00f, 1.41f, 1.73f, 2.03f,
        2.51f, 2.97f, 3.66f, 4.37f, 5.11f, 6.23f
    };
    static constexpr std::array<double, 12> industrialFrequencies {
        43.0, 61.0, 89.0, 127.0, 173.0, 239.0,
        317.0, 421.0, 557.0, 733.0, 967.0, 1279.0
    };

    for (std::size_t i = 0; i < resonators_.size(); ++i) {
        const float parity = (i & 1u) ? -1.0f : 1.0f;
        double frequency = 0.0;
        double decay = 0.0;
        float gain = 0.0f;

        if (archetype_ == Archetype::Industrial) {
            const double tensionWarp =
                1.0 + static_cast<double>(tension) *
                    (0.012 + 0.0025 * static_cast<double>(i)) * parity;
            frequency = industrialFrequencies[i] * tensionWarp;
            decay =
                0.10 + 0.018 * static_cast<double>(i) +
                0.22 * static_cast<double>(parameters_.body);
            gain =
                (0.055f / (1.0f + 0.20f * static_cast<float>(i))) *
                (0.22f + 0.78f * parameters_.body);
        } else {
            const float archetypeWarp =
                1.0f + 0.0075f * archetype * parity +
                0.010f * static_cast<float>(i % 3);
            const float tensionWarp =
                1.0f + tension *
                    (0.018f + 0.009f * static_cast<float>(i)) * parity;
            frequency =
                static_cast<double>(root) *
                static_cast<double>(baseRatios[i] * archetypeWarp * tensionWarp);
            decay =
                0.32 +
                (0.12 + 0.11 * static_cast<double>(i)) *
                (0.65 + 2.7 * parameters_.body);
            float lowModeScale = 1.0f;
            if (i == 0)
                lowModeScale =
                    (archetype_ == Archetype::Abyss ||
                     archetype_ == Archetype::Void) ? 0.55f : 0.24f;
            else if (i == 1)
                lowModeScale =
                    (archetype_ == Archetype::Abyss ||
                     archetype_ == Archetype::Void) ? 0.72f : 0.42f;
            else if (i == 2)
                lowModeScale = 0.82f;
            gain =
                lowModeScale *
                (0.090f / (1.0f + 0.16f * static_cast<float>(i))) *
                (0.25f + 0.75f * parameters_.body);
        }

        const float pan = std::clamp(
            -0.82f + 1.64f * static_cast<float>(i) /
                static_cast<float>(resonators_.size() - 1),
            -1.0f,
            1.0f);

        resonators_[i].configure(sampleRate_, frequency, decay, gain, pan);
    }
}

void Engine::configurePresence() noexcept {
    static constexpr std::array<double, 4> kBaseFormants {
        430.0, 780.0, 1320.0, 2150.0
    };
    static constexpr std::array<float, 4> kPan {
        -0.58f, 0.42f, -0.24f, 0.62f
    };

    const double driftX = std::tanh(chaosX_ * 0.055);
    const double driftY = std::tanh(chaosY_ * 0.050);
    const float tension = parameters_.tension;

    for (std::size_t i = 0; i < presenceResonators_.size(); ++i) {
        const double sign = (i & 1u) ? -1.0 : 1.0;
        const double drift =
            1.0 +
            sign * (0.018 * driftX + 0.012 * driftY) *
                (0.25 + 0.75 * parameters_.evolve) *
                (0.20 + 0.80 * parameters_.motion);

        const double frequency =
            kBaseFormants[i] *
            drift *
            (1.0 + sign * 0.018 * static_cast<double>(tension));

        const double decay =
            0.035 +
            0.018 * static_cast<double>(i) +
            0.035 * static_cast<double>(parameters_.evolve);

        const float gain =
            (0.045f / (1.0f + 0.22f * static_cast<float>(i))) *
            (0.45f + 0.55f * tension);

        presenceResonators_[i].configure(
            sampleRate_, frequency, decay, gain, kPan[i]);
    }
}

void Engine::ensureLongStreams() noexcept {
    const auto traits = traitsFor(archetype_);

    auto worldClipInUse = [&](const Clip* candidate, std::size_t except) noexcept {
        for (std::size_t i = 0; i < worldVoices_.size(); ++i) {
            if (i != except && worldVoices_[i].active &&
                worldVoices_[i].clip == candidate)
                return true;
        }
        return false;
    };

    auto textureClipInUse = [&](const Clip* candidate, std::size_t except) noexcept {
        for (std::size_t i = 0; i < textureVoices_.size(); ++i) {
            if (i != except && textureVoices_[i].active &&
                textureVoices_[i].clip == candidate)
                return true;
        }
        return false;
    };

    for (std::size_t voiceIndex = 0; voiceIndex < worldVoices_.size(); ++voiceIndex) {
        auto& voice = worldVoices_[voiceIndex];
        if (voice.active || worldPoolCount_ == 0)
            continue;

        const std::size_t startIndex =
            archetype_ == Archetype::Industrial && !worldSceneInitialised_
                ? std::min<std::size_t>(voiceIndex, worldPoolCount_ - 1)
                : std::min<std::size_t>(
                    static_cast<std::size_t>(rng_.uniform01() * worldPoolCount_),
                    worldPoolCount_ - 1);

        const Clip* clip = nullptr;
        for (std::size_t offset = 0; offset < worldPoolCount_; ++offset) {
            const Clip* candidate =
                worldPool_[(startIndex + offset) % worldPoolCount_];
            if (candidate && !worldClipInUse(candidate, voiceIndex)) {
                clip = candidate;
                break;
            }
        }
        if (!clip) {
            for (std::size_t offset = 0; offset < worldPoolCount_; ++offset) {
                const Clip* candidate =
                    worldPool_[(startIndex + offset) % worldPoolCount_];
                if (candidate) {
                    clip = candidate;
                    break;
                }
            }
        }
        if (!clip)
            continue;

        const double sourceRatio = clip->sampleRate / sampleRate_;
        const double slotSpread =
            1.0 + 0.035 * static_cast<double>(voiceIndex);
        const double rate =
            sourceRatio * static_cast<double>(traits.worldRate) * slotSpread *
            (1.0 + static_cast<double>(parameters_.motion) *
                (-0.14 + 0.28 * rng_.uniform01()));
        const double start =
            clip->frames > 8
                ? rng_.uniform01() * 0.35 * static_cast<double>(clip->frames - 2)
                : 0.0;
        voice.start(clip, rate, start);
    }

    if (worldPoolCount_ > 0)
        worldSceneInitialised_ = true;

    if (archetype_ == Archetype::Industrial) {
        constexpr double kFadeSeconds = 2.5;
        for (std::size_t i = 0; i < worldVoices_.size(); ++i) {
            auto& current = worldVoices_[i];
            auto& next = worldNextVoices_[i];
            if (!current.active || !current.clip || next.active)
                continue;
            const double remaining =
                static_cast<double>(current.clip->frames - 1) - current.position;
            const double triggerFrames =
                kFadeSeconds * sampleRate_ * std::max(0.01, std::abs(current.rate));
            if (remaining > triggerFrames)
                continue;
            const Clip* clip = nullptr;
            const std::size_t startIndex = std::min<std::size_t>(
                static_cast<std::size_t>(rng_.uniform01() * worldPoolCount_),
                worldPoolCount_ - 1);
            for (std::size_t offset = 0; offset < worldPoolCount_; ++offset) {
                const Clip* candidate = worldPool_[(startIndex + offset) % worldPoolCount_];
                if (candidate && candidate != current.clip) { clip = candidate; break; }
            }
            if (!clip) clip = current.clip;
            const double sourceRatio = clip->sampleRate / sampleRate_;
            const double slotSpread = 1.0 + 0.035 * static_cast<double>(i);
            const double rate = sourceRatio * static_cast<double>(traits.worldRate) *
                slotSpread * (1.0 + static_cast<double>(parameters_.motion) *
                (-0.14 + 0.28 * rng_.uniform01()));
            const double start = clip->frames > 8
                ? rng_.uniform01() * 0.20 * static_cast<double>(clip->frames - 2)
                : 0.0;
            next.start(clip, rate, start);
            worldCrossfade_[i] = 0.0f;
        }
    }

    for (std::size_t voiceIndex = 0;
         voiceIndex < textureVoices_.size();
         ++voiceIndex) {
        auto& voice = textureVoices_[voiceIndex];
        if (voice.active || texturePoolCount_ == 0)
            continue;

        const std::size_t startIndex =
            archetype_ == Archetype::Industrial && !textureSceneInitialised_
                ? std::min<std::size_t>(voiceIndex, texturePoolCount_ - 1)
                : std::min<std::size_t>(
                    static_cast<std::size_t>(rng_.uniform01() * texturePoolCount_),
                    texturePoolCount_ - 1);

        const Clip* clip = nullptr;
        for (std::size_t offset = 0; offset < texturePoolCount_; ++offset) {
            const Clip* candidate =
                texturePool_[(startIndex + offset) % texturePoolCount_];
            if (candidate && !textureClipInUse(candidate, voiceIndex)) {
                clip = candidate;
                break;
            }
        }
        if (!clip) {
            for (std::size_t offset = 0; offset < texturePoolCount_; ++offset) {
                const Clip* candidate =
                    texturePool_[(startIndex + offset) % texturePoolCount_];
                if (candidate) {
                    clip = candidate;
                    break;
                }
            }
        }
        if (!clip)
            continue;

        const double sourceRatio = clip->sampleRate / sampleRate_;
        const double slotSpread =
            1.0 - 0.045 * static_cast<double>(voiceIndex);
        const double rate =
            sourceRatio * static_cast<double>(traits.textureRate) * slotSpread *
            (1.0 + static_cast<double>(parameters_.motion) *
                (-0.32 + 0.64 * rng_.uniform01()));
        const double start =
            clip->frames > 8
                ? rng_.uniform01() * 0.35 * static_cast<double>(clip->frames - 2)
                : 0.0;
        voice.start(clip, rate, start);
    }
    if (texturePoolCount_ > 0)
        textureSceneInitialised_ = true;

    if (archetype_ == Archetype::Industrial) {
        constexpr double kFadeSeconds = 1.8;
        for (std::size_t i = 0; i < textureVoices_.size(); ++i) {
            auto& current = textureVoices_[i];
            auto& next = textureNextVoices_[i];
            if (!current.active || !current.clip || next.active)
                continue;
            const double remaining =
                static_cast<double>(current.clip->frames - 1) - current.position;
            const double triggerFrames =
                kFadeSeconds * sampleRate_ * std::max(0.01, std::abs(current.rate));
            if (remaining > triggerFrames)
                continue;
            const Clip* clip = nullptr;
            const std::size_t startIndex = std::min<std::size_t>(
                static_cast<std::size_t>(rng_.uniform01() * texturePoolCount_),
                texturePoolCount_ - 1);
            for (std::size_t offset = 0; offset < texturePoolCount_; ++offset) {
                const Clip* candidate = texturePool_[(startIndex + offset) % texturePoolCount_];
                if (candidate && candidate != current.clip) { clip = candidate; break; }
            }
            if (!clip) clip = current.clip;
            const double sourceRatio = clip->sampleRate / sampleRate_;
            const double slotSpread = 1.0 - 0.045 * static_cast<double>(i);
            const double rate = sourceRatio * static_cast<double>(traits.textureRate) *
                slotSpread * (1.0 + static_cast<double>(parameters_.motion) *
                (-0.32 + 0.64 * rng_.uniform01()));
            const double start = clip->frames > 8
                ? rng_.uniform01() * 0.20 * static_cast<double>(clip->frames - 2)
                : 0.0;
            next.start(clip, rate, start);
            textureCrossfade_[i] = 0.0f;
        }
    }

}

void Engine::updateControlState() noexcept {
    // Slow bounded Lorenz-like evolution. The state is deterministic and
    // intentionally not exposed as arbitrary "random modulation".
    const double speed = 0.20 + 1.80 * parameters_.evolve;
    const double dt = 0.0017 * speed;
    const double sigma = 10.0;
    const double rho = 24.0 + 5.0 * parameters_.tension;
    const double beta = 8.0 / 3.0;

    const double dx = sigma * (chaosY_ - chaosX_);
    const double dy = chaosX_ * (rho - chaosZ_) - chaosY_;
    const double dz = chaosX_ * chaosY_ - beta * chaosZ_;

    chaosX_ += dt * dx;
    chaosY_ += dt * dy;
    chaosZ_ += dt * dz;

    if (!std::isfinite(chaosX_) ||
        !std::isfinite(chaosY_) ||
        !std::isfinite(chaosZ_) ||
        std::fabs(chaosX_) > 100.0 ||
        std::fabs(chaosY_) > 100.0 ||
        std::fabs(chaosZ_) > 100.0) {
        chaosX_ = 0.11;
        chaosY_ = 0.0;
        chaosZ_ = 0.0;
    }

    const float cx = static_cast<float>(std::tanh(chaosX_ * 0.08));
    const float cy = static_cast<float>(std::tanh(chaosY_ * 0.08));
    const float cz = static_cast<float>(std::tanh((chaosZ_ - 24.0) * 0.05));

    const float motionDepth = parameters_.evolve * parameters_.motion;
    oscillatorDrift_[0] = 0.006f * motionDepth * cx;
    oscillatorDrift_[1] = 0.009f * motionDepth * cy;
    oscillatorDrift_[2] = 0.013f * motionDepth * cz;
    oscillatorDrift_[3] = 0.017f * motionDepth * (0.5f * cx - 0.5f * cy);
    motionPan_ = parameters_.motion * cx;
    motionSpectral_ = parameters_.motion * cy;

    // 20–90 second deterministic scene redistribution. The sources keep
    // playing while their prominence moves, so the world evolves without
    // obvious preset switching.
    worldLayerTarget_[0] = 0.28f + 0.72f * clamp01(0.5f + 0.5f * cx);
    worldLayerTarget_[1] = 0.16f + 0.68f * clamp01(0.5f + 0.5f * cy);
    worldLayerTarget_[2] = 0.10f + 0.56f * clamp01(0.5f + 0.5f * cz);
    textureLayerTarget_[0] = 0.20f + 0.68f * clamp01(0.5f - 0.5f * cy);
    textureLayerTarget_[1] = 0.14f + 0.70f * clamp01(0.5f + 0.5f * cz);
    textureLayerTarget_[2] = 0.08f + 0.62f * clamp01(0.5f - 0.5f * cx);

    // Resonant coefficients are expensive (exp/cos) and the structure evolves
    // on a cinematic time scale. Update them far below the 64-sample macro
    // modulation rate; fast pan/drift motion remains unchanged.
    if (--structureCountdown_ <= 0) {
        configureBody();
        if (archetype_ == Archetype::Nocturne)
            configurePresence();
        structureCountdown_ = kStructureControlPeriods;
    }

    if (parameters_.events <= 0.0f) {
        eventCountdown_ = static_cast<int>(sampleRate_);
    } else if (eventCountdown_ <= 0) {
        triggerEvent();
    }
}

void Engine::triggerEvent() noexcept {
    if (!gate_ || parameters_.events <= 0.0f)
        return;

    ++eventCount_;

    const float force =
        (0.15f + 0.70f * parameters_.events) *
        (0.55f + 0.45f * rng_.uniform01());

    bodyExcitation_ += force;

    if (eventPoolCount_ > 0) {
        const std::size_t index = std::min<std::size_t>(
            static_cast<std::size_t>(rng_.uniform01() * eventPoolCount_),
            eventPoolCount_ - 1);
        const Clip* clip = eventPool_[index];
        if (clip) {
            const double sourceRatio = clip->sampleRate / sampleRate_;
            const double rate = sourceRatio * (0.72 + 0.58 * rng_.uniform01());
            const double start =
                clip->frames > 32
                    ? rng_.uniform01() * static_cast<double>(clip->frames / 5)
                    : 0.0;
            eventVoice_.start(clip, rate, start);
        }
    }

    if (bodyPoolCount_ > 0) {
        const std::size_t index = std::min<std::size_t>(
            static_cast<std::size_t>(rng_.uniform01() * bodyPoolCount_),
            bodyPoolCount_ - 1);
        const Clip* clip = bodyPool_[index];
        if (clip) {
            const double sourceRatio = clip->sampleRate / sampleRate_;
            const double rate = sourceRatio * (0.78 + 0.44 * rng_.uniform01());
            bodyExciterVoice_.start(clip, rate, 0.0);
        }
    }

    const float eventAmount = clamp01(parameters_.events);
    const auto traits = traitsFor(archetype_);
    const float archetypeFactor =
        1.0f / std::max(0.25f, traits.eventRate);

    // EVENTS must scale from genuinely sparse to dense.
    // Low settings are cinematic punctuation, not a constant random ticker.
    const double baseGapSeconds =
        2.0 +
        40.0 * static_cast<double>((1.0f - eventAmount) * (1.0f - eventAmount));

    const double evolveAcceleration =
        1.0 - 0.25 * static_cast<double>(parameters_.evolve * eventAmount);

    const double gapSeconds =
        std::clamp(
            archetypeFactor *
                baseGapSeconds *
                evolveAcceleration *
                (0.65 + 0.70 * rng_.uniform01()),
            0.75,
            70.0);

    eventCountdown_ = std::max(1, static_cast<int>(gapSeconds * sampleRate_));
}

void Engine::processSpace(float dryL, float dryR, float& outL, float& outR) noexcept {
    const float wet = parameters_.space;
    if (wet <= 0.0f) {
        outL = dryL;
        outR = dryR;
        return;
    }

    const float mid = 0.5f * (dryL + dryR);
    const float side = 0.5f * (dryL - dryR);
    const std::array<float, 4> input {
        0.72f * dryL + 0.18f * mid,
        0.72f * dryR + 0.18f * mid,
        0.54f * mid + 0.30f * side,
        0.54f * mid - 0.30f * side
    };
    std::array<float, 4> d {};
    for (std::size_t i = 0; i < d.size(); ++i)
        d[i] = delays_[i].read();

    std::array<float, 4> mixed {
        0.5f * ( d[0] + d[1] + d[2] + d[3]),
        0.5f * ( d[0] - d[1] + d[2] - d[3]),
        0.5f * ( d[0] + d[1] - d[2] - d[3]),
        0.5f * ( d[0] - d[1] - d[2] + d[3])
    };

    // INDUSTRIAL needs a large room while held, but must not turn into a
    // self-sustaining metallic drone after Note-Off. Reduce the feedback loop
    // during release; the existing delay energy still decays naturally.
    const float heldFeedback = 0.64f + 0.30f * wet;
    const float releaseFeedback =
        archetype_ == Archetype::Industrial
            ? (0.48f + 0.22f * wet)
            : heldFeedback;
    const float feedback = gate_ ? heldFeedback : releaseFeedback;
    const float damping = 0.18f + 0.62f * wet;
    for (std::size_t i = 0; i < delays_.size(); ++i) {
        reverbDamping_[i] +=
            damping * (mixed[i] - reverbDamping_[i]);
        const float injected =
            input[i] * (0.17f + 0.05f * static_cast<float>(i)) +
            feedback * reverbDamping_[i];
        delays_[i].write(softClip(injected));
        delays_[i].advance();
    }

    const float wetL = kSqrtHalf * (d[0] + d[2]);
    const float wetR = kSqrtHalf * (d[1] + d[3]);

    // At 100 % SPACE the output is truly 100 % wet.
    outL = (1.0f - wet) * dryL + wet * wetL;
    outR = (1.0f - wet) * dryR + wet * wetR;
}

void Engine::process(float* left, float* right, std::size_t frames) noexcept {
    if (!left || !right || frames == 0)
        return;

    const auto traits = traitsFor(archetype_);
    const float foundation = parameters_.foundation * traits.foundationGain;
    const float world = parameters_.world * traits.worldGain;
    const float texture = parameters_.texture * traits.textureGain;
    const float body = parameters_.body * traits.bodyGain;
    const float tension = parameters_.tension;

    const float attackSeconds = 0.035f;
    const float releaseSeconds =
        archetype_ == Archetype::Industrial
            ? (0.55f + 0.75f * parameters_.space)
            : (2.5f + 4.5f * parameters_.space);
    const float attackCoeff = static_cast<float>(
        1.0 - std::exp(-1.0 / (std::max(0.001f, attackSeconds) * sampleRate_)));
    const float releaseCoeff = static_cast<float>(
        1.0 - std::exp(-1.0 / (std::max(0.05f, releaseSeconds) * sampleRate_)));
    const float sceneFadeSeconds =
        90.0f - 70.0f * parameters_.evolve;
    const float sceneFadeCoeff = static_cast<float>(
        1.0 - std::exp(-1.0 / (
            std::max(5.0f, sceneFadeSeconds) * sampleRate_)));

    const float baseHz = midiToHz(midiNote_);
    static constexpr std::array<std::array<float, 4>, 6> kFoundationRatios {{
        {{0.50f, 1.013f, 2.071f, 3.491f}},  // VOID
        {{0.50f, 1.00f, 1.337f, 2.003f}},  // RUINS
        {{0.25f, 0.503f, 0.997f, 1.487f}},  // INDUSTRIAL: sub/machine mass
        {{0.50f, 0.997f, 1.861f, 3.127f}}, // WASTELAND
        {{0.125f,0.25f, 0.503f, 0.709f}},  // ABYSS
        {{0.50f, 1.00f, 1.259f, 1.887f}}   // NOCTURNE
    }};
    static constexpr std::array<std::array<float, 4>, 6> kFoundationWeights {{
        {{0.18f,0.26f,0.42f,0.52f}},
        {{0.18f,0.52f,0.42f,0.20f}},
        {{0.58f,0.34f,0.14f,0.06f}},
        {{0.05f,0.24f,0.48f,0.70f}},
        {{1.00f,0.58f,0.18f,0.08f}},
        {{0.12f,0.42f,0.58f,0.38f}}
    }};
    const auto archetypeIndex =
        std::min<std::size_t>(static_cast<std::size_t>(archetype_), 5u);
    const auto& ratios = kFoundationRatios[archetypeIndex];
    const auto& foundationWeights = kFoundationWeights[archetypeIndex];

    for (std::size_t n = 0; n < frames; ++n) {
        if (--controlCountdown_ <= 0) {
            updateControlState();
            controlCountdown_ = kControlPeriod;
        }

        if (eventCountdown_ > 0)
            --eventCountdown_;

        if (gate_)
            envelope_ += attackCoeff * (velocity_ - envelope_);
        else
            envelope_ += releaseCoeff * (0.0f - envelope_);

        if (envelope_ < 1.0e-8f)
            envelope_ = 0.0f;

        if (gate_)
            ensureLongStreams();

        float dryL = 0.0f;
        float dryR = 0.0f;

        // FOUNDATION: dark tonal field. Tension deliberately bends upper voices
        // away from a simple consonant stack.
        if (foundation > 0.0f && envelope_ > 0.0f) {
            float sumL = 0.0f;
            float sumR = 0.0f;
            static constexpr std::array<float, 4> foundationPan {
                0.0f, 0.0f, -0.34f, 0.38f
            };

            for (std::size_t i = 0; i < oscillatorPhase_.size(); ++i) {
                const float tensionWarp =
                    1.0f +
                    tension * (i >= 2 ? (0.018f + 0.014f * static_cast<float>(i)) : 0.004f);
                const double hz =
                    static_cast<double>(baseHz) *
                    static_cast<double>(ratios[i] * tensionWarp) *
                    static_cast<double>(1.0f + oscillatorDrift_[i]);

                oscillatorPhase_[i] += kTwoPi * hz / sampleRate_;
                if (oscillatorPhase_[i] >= kTwoPi)
                    oscillatorPhase_[i] = std::fmod(oscillatorPhase_[i], kTwoPi);

                const float sample =
                    foundationWeights[i] *
                    static_cast<float>(std::sin(oscillatorPhase_[i]));

                const float pan01 = 0.5f * (foundationPan[i] + 1.0f);
                const float gL = std::sqrt(std::max(0.0f, 1.0f - pan01));
                const float gR = std::sqrt(std::max(0.0f, pan01));
                sumL += sample * gL;
                sumR += sample * gR;
            }

            const float tonalGain =
                (archetype_ == Archetype::Industrial ? 0.072f : 0.105f) *
                foundation * envelope_;
            dryL += tonalGain * sumL;
            dryR += tonalGain * sumR;
        }

        float worldL = 0.0f;
        float worldR = 0.0f;
        float worldGainSum = 0.0f;
        for (std::size_t i = 0; i < worldVoices_.size(); ++i) {
            worldLayerGain_[i] +=
                sceneFadeCoeff *
                (worldLayerTarget_[i] - worldLayerGain_[i]);

            float l = 0.0f;
            float r = 0.0f;
            worldVoices_[i].process(l, r);
            if (archetype_ == Archetype::Industrial && worldNextVoices_[i].active) {
                float nextL = 0.0f, nextR = 0.0f;
                worldNextVoices_[i].process(nextL, nextR);
                worldCrossfade_[i] = std::min(
                    1.0f, worldCrossfade_[i] +
                    static_cast<float>(1.0 / std::max(1.0, 2.5 * sampleRate_)));
                const float x = worldCrossfade_[i];
                const float a = std::sqrt(std::max(0.0f, 1.0f - x));
                const float b = std::sqrt(x);
                l = a * l + b * nextL;
                r = a * r + b * nextR;
                if (x >= 1.0f || !worldVoices_[i].active) {
                    worldVoices_[i] = worldNextVoices_[i];
                    worldNextVoices_[i].reset();
                    worldCrossfade_[i] = 0.0f;
                }
            }
            if (!worldVoices_[i].active)
                continue;

            const float slotPan =
                (static_cast<float>(i) - 1.0f) *
                    (archetype_ == Archetype::Industrial ? 0.30f : 0.18f) +
                (archetype_ == Archetype::Industrial ? 0.20f : 0.14f) * motionPan_;
            const float g = std::max(0.0f, worldLayerGain_[i]);
            worldL += g * l * (1.0f - slotPan);
            worldR += g * r * (1.0f + slotPan);
            worldGainSum += g;
        }
        if (worldGainSum > 1.0f) {
            worldL /= worldGainSum;
            worldR /= worldGainSum;
        }

        if (world > 0.0f && envelope_ > 0.0f) {
            const float worldScale =
                archetype_ == Archetype::Industrial ? 1.16f : 0.98f;
            dryL += worldScale * world * envelope_ * worldL;
            dryR += worldScale * world * envelope_ * worldR;
        }

        float textureL = 0.0f;
        float textureR = 0.0f;
        float textureGainSum = 0.0f;
        bool anyTextureActive = false;
        for (std::size_t i = 0; i < textureVoices_.size(); ++i) {
            textureLayerGain_[i] +=
                sceneFadeCoeff *
                (textureLayerTarget_[i] - textureLayerGain_[i]);

            float l = 0.0f;
            float r = 0.0f;
            textureVoices_[i].process(l, r);
            if (archetype_ == Archetype::Industrial && textureNextVoices_[i].active) {
                float nextL = 0.0f, nextR = 0.0f;
                textureNextVoices_[i].process(nextL, nextR);
                textureCrossfade_[i] = std::min(
                    1.0f, textureCrossfade_[i] +
                    static_cast<float>(1.0 / std::max(1.0, 1.8 * sampleRate_)));
                const float x = textureCrossfade_[i];
                const float a = std::sqrt(std::max(0.0f, 1.0f - x));
                const float b = std::sqrt(x);
                l = a * l + b * nextL;
                r = a * r + b * nextR;
                if (x >= 1.0f || !textureVoices_[i].active) {
                    textureVoices_[i] = textureNextVoices_[i];
                    textureNextVoices_[i].reset();
                    textureCrossfade_[i] = 0.0f;
                }
            }
            if (!textureVoices_[i].active)
                continue;

            anyTextureActive = true;
            const float slotPan =
                (1.0f - static_cast<float>(i)) *
                    (archetype_ == Archetype::Industrial ? 0.34f : 0.22f) -
                (archetype_ == Archetype::Industrial ? 0.24f : 0.18f) * motionPan_;
            const float g = std::max(0.0f, textureLayerGain_[i]);
            textureL += g * l * (1.0f - slotPan);
            textureR += g * r * (1.0f + slotPan);
            textureGainSum += g;
        }
        if (textureGainSum > 1.0f) {
            textureL /= textureGainSum;
            textureR /= textureGainSum;
        }

        // Synthetic fallback texture: filtered noise, intentionally subordinate
        // to real sources once a texture clip is assigned.
        const float rndL = rng_.bipolar();
        const float rndR = rng_.bipolar();
        const float lpBase = 0.004f + 0.024f * (1.0f - tension);
        const float lpCoeff = std::clamp(
            lpBase * (1.0f + 0.35f * motionSpectral_),
            0.0015f, 0.040f);
        noiseStateL_ += lpCoeff * (rndL - noiseStateL_);
        noiseStateR_ += lpCoeff * (rndR - noiseStateR_);
        const float hpIn = 0.5f * (noiseStateL_ + noiseStateR_);
        textureHpState_ += 0.0025f * (hpIn - textureHpState_);
        const float darkNoiseL = noiseStateL_ - 0.45f * textureHpState_;
        const float darkNoiseR = noiseStateR_ - 0.45f * textureHpState_;

        if (texture > 0.0f && envelope_ > 0.0f) {
            const float realWeight = anyTextureActive ? 0.88f : 0.0f;
            const float synthWeight = anyTextureActive ? 0.12f : 1.0f;
            const float pan = -0.22f * motionPan_;
            const float realTextureScale =
                archetype_ == Archetype::Industrial ? 0.76f : 0.62f;
            const float noiseScale =
                archetype_ == Archetype::Industrial ? 0.012f : 0.035f;
            dryL += texture * envelope_ * (1.0f - pan) *
                (realTextureScale * realWeight * textureL + noiseScale * synthWeight * darkNoiseL);
            dryR += texture * envelope_ * (1.0f + pan) *
                (realTextureScale * realWeight * textureR + noiseScale * synthWeight * darkNoiseR);
        }

        float eventL = 0.0f, eventR = 0.0f;
        eventVoice_.process(eventL, eventR);
        if (eventVoice_.active) {
            dryL += 0.32f * envelope_ * eventL;
            dryR += 0.32f * envelope_ * eventR;
        }

        // NOCTURNE PRESENCE: an abstract quasi-vocal field made only from
        // deterministic noise exciting drifting formant resonances. No literal
        // choir, words or vocal sample is used.
        if (archetype_ == Archetype::Nocturne && envelope_ > 0.0f) {
            const float presenceNoise = presenceRng_.bipolar();
            const float presenceLpCoeff = static_cast<float>(
                1.0 - std::exp(-kTwoPi * 95.0 / sampleRate_));
            presenceNoiseLowpass_ +=
                presenceLpCoeff * (presenceNoise - presenceNoiseLowpass_);
            const float presenceExcitation =
                0.11f * (presenceNoise - 0.70f * presenceNoiseLowpass_);

            float presenceL = 0.0f;
            float presenceR = 0.0f;
            for (auto& resonator : presenceResonators_)
                resonator.process(presenceExcitation, presenceL, presenceR);

            const float presenceAmount =
                0.060f *
                (0.35f + 0.65f * parameters_.evolve) *
                (0.45f + 0.55f * parameters_.tension);

            dryL += presenceAmount * envelope_ * presenceL;
            dryR += presenceAmount * envelope_ * presenceR;
        }

        // IMPOSSIBLE BODY: synthetic impulses and optional real recorded
        // exciters drive the same inharmonic modal bank. The exciter itself is
        // not mixed to the output here; only the resonant body's response is.
        bodyExcitation_ *= 0.9925f;
        float exciterL = 0.0f;
        float exciterR = 0.0f;
        bodyExciterVoice_.process(exciterL, exciterR);
        const float realExciterMono = 0.5f * (exciterL + exciterR);
        const float bodyHpCoeff =
            static_cast<float>(1.0 - std::exp(-kTwoPi * 42.0 / sampleRate_));
        bodyExciterLowpass_ +=
            bodyHpCoeff * (realExciterMono - bodyExciterLowpass_);
        const float realExciter =
            softClip(2.25f * (realExciterMono - bodyExciterLowpass_));

        const float industrialContinuousExciter =
            archetype_ == Archetype::Industrial && gate_
                ? 0.010f * envelope_ *
                    (0.35f * noiseStateL_ + 0.35f * noiseStateR_ +
                     0.30f * rng_.bipolar())
                : 0.0f;
        const float syntheticExciter =
            bodyExcitation_ *
            (0.18f + 0.82f * std::fabs(rng_.bipolar())) *
            rng_.bipolar() +
            industrialContinuousExciter;

        const float excitationGain =
            bodyExciterVoice_.clip
                ? std::clamp(bodyExciterVoice_.clip->excitationGain, 0.0f, 4.0f)
                : 1.0f;

        const float excitationNoise =
            syntheticExciter +
            (bodyExciterVoice_.active
                ? 0.010f * excitationGain * realExciter
                : 0.0f);

        float bodyL = 0.0f;
        float bodyR = 0.0f;
        if (body > 0.0f) {
            for (auto& resonator : resonators_)
                resonator.process(excitationNoise, bodyL, bodyR);
            dryL += body * envelope_ * bodyL;
            dryR += body * envelope_ * bodyR;
        }

        float spacedL = 0.0f;
        float spacedR = 0.0f;
        processSpace(dryL, dryR, spacedL, spacedR);

        // OUTPUT: 0 % = silence, 50 % = nominal -6.02 dB trim,
        // 100 % = nominal unity. The default leaves headroom for real-source
        // layers and modal/event summation.
        const float gain = parameters_.output;
        left[n] = safeOutput(spacedL * gain);
        right[n] = safeOutput(spacedR * gain);

        if (!std::isfinite(left[n])) left[n] = 0.0f;
        if (!std::isfinite(right[n])) right[n] = 0.0f;
    }
}

} // namespace noctomorph
