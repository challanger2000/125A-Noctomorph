#include "NoctomorphCore.h"

#include <algorithm>
#include <cmath>

namespace noctomorph {
namespace {

constexpr double kTwoPi = 6.283185307179586476925286766559;
constexpr float kSqrtHalf = 0.7071067811865475244f;

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

    gate_ = false;
    midiNote_ = 36;
    velocity_ = 1.0f;
    envelope_ = 0.0f;

    oscillatorPhase_.fill(0.0);
    oscillatorDrift_.fill(0.0f);
    noiseStateL_ = 0.0f;
    noiseStateR_ = 0.0f;
    textureHpState_ = 0.0f;

    chaosX_ = 0.11 + 0.02 * rng_.bipolar();
    chaosY_ = 0.03 * rng_.bipolar();
    chaosZ_ = 0.03 * rng_.bipolar();
    controlCountdown_ = 0;

    for (auto& r : resonators_)
        r.reset();

    bodyExcitation_ = 0.0f;

    worldVoice_.reset();
    textureVoice_.reset();
    eventVoice_.reset();

    eventCount_ = 0;
    eventCountdown_ = static_cast<int>(sampleRate_ * (2.0 + 3.0 * rng_.uniform01()));

    for (auto& d : delays_)
        d.reset();
    reverbDamping_.fill(0.0f);

    configureBody();
}

void Engine::setParameters(const Parameters& p) noexcept {
    parameters_.foundation = clamp01(p.foundation);
    parameters_.world = clamp01(p.world);
    parameters_.texture = clamp01(p.texture);
    parameters_.body = clamp01(p.body);
    parameters_.tension = clamp01(p.tension);
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
}

void Engine::setWorldClip(const Clip* clip) noexcept {
    worldClip_ = clip;
    worldVoice_.reset();
}

void Engine::setTextureClip(const Clip* clip) noexcept {
    textureClip_ = clip;
    textureVoice_.reset();
}

void Engine::setEventClip(const Clip* clip) noexcept {
    eventClip_ = clip;
    eventVoice_.reset();
}

void Engine::noteOn(int midiNote, float velocity) noexcept {
    midiNote_ = std::clamp(midiNote, 0, 127);
    velocity_ = clamp01(velocity);
    gate_ = velocity_ > 0.0f;

    // Retrigger the acoustic world without resetting macro evolution.
    if (gate_) {
        ensureLongStreams();
        bodyExcitation_ += 0.08f + 0.22f * velocity_;
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

    for (std::size_t i = 0; i < resonators_.size(); ++i) {
        const float parity = (i & 1u) ? -1.0f : 1.0f;
        const float archetypeWarp =
            1.0f + 0.0075f * archetype * parity + 0.010f * static_cast<float>(i % 3);
        const float tensionWarp =
            1.0f + tension * (0.018f + 0.009f * static_cast<float>(i)) * parity;
        const double frequency =
            static_cast<double>(root) *
            static_cast<double>(baseRatios[i] * archetypeWarp * tensionWarp);

        const double decay =
            0.32 +
            (0.12 + 0.11 * static_cast<double>(i)) *
            (0.65 + 2.7 * parameters_.body);

        const float gain =
            (0.090f / (1.0f + 0.16f * static_cast<float>(i))) *
            (0.25f + 0.75f * parameters_.body);

        const float pan = std::clamp(
            -0.82f + 1.64f * static_cast<float>(i) /
                static_cast<float>(resonators_.size() - 1),
            -1.0f,
            1.0f);

        resonators_[i].configure(sampleRate_, frequency, decay, gain, pan);
    }
}

void Engine::ensureLongStreams() noexcept {
    if (worldClip_ && !worldVoice_.active) {
        const double sourceRatio = worldClip_->sampleRate / sampleRate_;
        const double rate =
            sourceRatio * (0.88 + 0.16 * rng_.uniform01());
        const double start =
            worldClip_->frames > 8
                ? rng_.uniform01() * static_cast<double>(worldClip_->frames - 2)
                : 0.0;
        worldVoice_.start(worldClip_, rate, start);
    }

    if (textureClip_ && !textureVoice_.active) {
        const double sourceRatio = textureClip_->sampleRate / sampleRate_;
        const double rate =
            sourceRatio * (0.62 + 0.42 * rng_.uniform01());
        const double start =
            textureClip_->frames > 8
                ? rng_.uniform01() * static_cast<double>(textureClip_->frames - 2)
                : 0.0;
        textureVoice_.start(textureClip_, rate, start);
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

    oscillatorDrift_[0] = 0.006f * parameters_.evolve * cx;
    oscillatorDrift_[1] = 0.009f * parameters_.evolve * cy;
    oscillatorDrift_[2] = 0.013f * parameters_.evolve * cz;
    oscillatorDrift_[3] = 0.017f * parameters_.evolve * (0.5f * cx - 0.5f * cy);

    // Reconfigure infrequently: body is slowly deformed by tension/evolution.
    configureBody();

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

    if (eventClip_) {
        const double sourceRatio = eventClip_->sampleRate / sampleRate_;
        const double rate = sourceRatio * (0.74 + 0.54 * rng_.uniform01());
        eventVoice_.start(eventClip_, rate, 0.0);
    }

    const float eventAmount = clamp01(parameters_.events);
    const float archetypeFactor =
        0.88f + 0.08f * static_cast<float>(static_cast<unsigned>(archetype_));

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

    const float input = 0.5f * (dryL + dryR);
    std::array<float, 4> d {};
    for (std::size_t i = 0; i < d.size(); ++i)
        d[i] = delays_[i].read();

    std::array<float, 4> mixed {
        0.5f * ( d[0] + d[1] + d[2] + d[3]),
        0.5f * ( d[0] - d[1] + d[2] - d[3]),
        0.5f * ( d[0] + d[1] - d[2] - d[3]),
        0.5f * ( d[0] - d[1] - d[2] + d[3])
    };

    const float feedback = 0.64f + 0.30f * wet;
    const float damping = 0.18f + 0.62f * wet;
    for (std::size_t i = 0; i < delays_.size(); ++i) {
        reverbDamping_[i] +=
            damping * (mixed[i] - reverbDamping_[i]);
        const float injected =
            input * (0.17f + 0.05f * static_cast<float>(i)) +
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

    const float foundation = parameters_.foundation;
    const float world = parameters_.world;
    const float texture = parameters_.texture;
    const float body = parameters_.body;
    const float tension = parameters_.tension;

    const float attackSeconds = 0.035f;
    const float releaseSeconds = 2.5f + 4.5f * parameters_.space;
    const float attackCoeff = static_cast<float>(
        1.0 - std::exp(-1.0 / (std::max(0.001f, attackSeconds) * sampleRate_)));
    const float releaseCoeff = static_cast<float>(
        1.0 - std::exp(-1.0 / (std::max(0.05f, releaseSeconds) * sampleRate_)));

    const float baseHz = midiToHz(midiNote_);
    static constexpr std::array<float, 4> ratios {0.50f, 1.00f, 1.4983f, 2.1189f};
    static constexpr std::array<float, 4> foundationWeights {0.34f, 1.00f, 0.55f, 0.30f};

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
            float sum = 0.0f;
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

                const float amp = foundationWeights[i];
                sum += amp * static_cast<float>(std::sin(oscillatorPhase_[i]));
            }
            const float tonal = 0.11f * foundation * envelope_ * sum;
            dryL += tonal;
            dryR += tonal;
        }

        float worldL = 0.0f, worldR = 0.0f;
        worldVoice_.process(worldL, worldR);
        if (world > 0.0f && envelope_ > 0.0f) {
            dryL += 0.95f * world * envelope_ * worldL;
            dryR += 0.95f * world * envelope_ * worldR;
        }

        float textureL = 0.0f, textureR = 0.0f;
        textureVoice_.process(textureL, textureR);

        // Synthetic fallback texture: filtered noise, intentionally subordinate
        // to real sources once a texture clip is assigned.
        const float rndL = rng_.bipolar();
        const float rndR = rng_.bipolar();
        const float lpCoeff = 0.004f + 0.024f * (1.0f - tension);
        noiseStateL_ += lpCoeff * (rndL - noiseStateL_);
        noiseStateR_ += lpCoeff * (rndR - noiseStateR_);
        const float hpIn = 0.5f * (noiseStateL_ + noiseStateR_);
        textureHpState_ += 0.0025f * (hpIn - textureHpState_);
        const float darkNoiseL = noiseStateL_ - 0.45f * textureHpState_;
        const float darkNoiseR = noiseStateR_ - 0.45f * textureHpState_;

        if (texture > 0.0f && envelope_ > 0.0f) {
            const float realWeight = textureVoice_.active ? 0.80f : 0.0f;
            const float synthWeight = textureVoice_.active ? 0.20f : 1.0f;
            dryL += texture * envelope_ *
                (0.62f * realWeight * textureL + 0.035f * synthWeight * darkNoiseL);
            dryR += texture * envelope_ *
                (0.62f * realWeight * textureR + 0.035f * synthWeight * darkNoiseR);
        }

        float eventL = 0.0f, eventR = 0.0f;
        eventVoice_.process(eventL, eventR);
        if (eventVoice_.active) {
            dryL += 0.32f * envelope_ * eventL;
            dryR += 0.32f * envelope_ * eventR;
        }

        // IMPOSSIBLE BODY: a sparse excitation enters an inharmonic modal bank.
        bodyExcitation_ *= 0.9925f;
        const float excitationNoise =
            bodyExcitation_ *
            (0.18f + 0.82f * std::fabs(rng_.bipolar())) *
            rng_.bipolar();

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
