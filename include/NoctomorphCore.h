#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace noctomorph {

enum class Archetype : std::uint8_t {
    Void = 0,
    Ruins,
    Industrial,
    Wasteland,
    Abyss,
    Nocturne,
    Count
};

struct Clip {
    const float* left = nullptr;
    const float* right = nullptr; // nullptr = mono
    std::size_t frames = 0;
    double sampleRate = 48000.0;
    bool loop = false;
    float excitationGain = 1.0f; // BODY-exciter calibration only
};

struct Parameters {
    float foundation = 0.30f;
    float world = 0.42f;
    float texture = 0.34f;
    float body = 0.30f;
    float tension = 0.32f;
    float motion = 0.45f;
    float evolve = 0.45f;
    float events = 0.00f;
    float space = 0.45f;
    float output = 0.50f;
};

class Engine {
public:
    void prepare(double sampleRate) noexcept;
    void reset(std::uint64_t seed = 0x125A4E4F43544F4DULL) noexcept;

    void setParameters(const Parameters& parameters) noexcept;
    void setArchetype(Archetype archetype) noexcept;

    void setWorldClip(const Clip* clip) noexcept;
    void setTextureClip(const Clip* clip) noexcept;
    void setEventClip(const Clip* clip) noexcept;
    void setBodyExciterClip(const Clip* clip) noexcept;

    void setWorldPool(const Clip* const* clips, std::size_t count) noexcept;
    void setTexturePool(const Clip* const* clips, std::size_t count) noexcept;
    void setEventPool(const Clip* const* clips, std::size_t count) noexcept;
    void setBodyExciterPool(const Clip* const* clips, std::size_t count) noexcept;

    void noteOn(int midiNote, float velocity) noexcept;
    void noteOff() noexcept;

    void process(float* left, float* right, std::size_t frames) noexcept;

    std::uint64_t eventCount() const noexcept { return eventCount_; }
    bool active() const noexcept { return gate_ || envelope_ > 1.0e-5f; }
    double sampleRate() const noexcept { return sampleRate_; }

private:
    struct Rng {
        std::uint64_t state = 0x125A4E4F43544F4DULL;
        void seed(std::uint64_t value) noexcept;
        std::uint64_t nextU64() noexcept;
        float uniform01() noexcept;
        float bipolar() noexcept;
    };

    struct StreamVoice {
        const Clip* clip = nullptr;
        double position = 0.0;
        double rate = 1.0;
        bool active = false;

        void reset() noexcept;
        void start(const Clip* source, double playbackRate = 1.0, double startPosition = 0.0) noexcept;
        void process(float& left, float& right) noexcept;
    };

    struct Resonator {
        float b0 = 0.0f;
        float a1 = 0.0f;
        float a2 = 0.0f;
        float z1L = 0.0f;
        float z2L = 0.0f;
        float z1R = 0.0f;
        float z2R = 0.0f;
        float pan = 0.0f;

        void reset() noexcept;
        void configure(double sampleRate, double frequencyHz, double decaySeconds, float gain, float stereoPan) noexcept;
        void process(float excitation, float& left, float& right) noexcept;
    };

    struct DelayLine {
        static constexpr std::size_t kCapacity = 32768;
        std::array<float, kCapacity> data {};
        std::size_t index = 0;
        std::size_t length = 1;

        void reset() noexcept;
        void setLength(std::size_t samples) noexcept;
        float read() const noexcept;
        void write(float value) noexcept;
        void advance() noexcept;
    };

    void updateControlState() noexcept;
    void configureBody() noexcept;
    void configurePresence() noexcept;
    void triggerEvent() noexcept;
    void ensureLongStreams() noexcept;
    void processSpace(float dryL, float dryR, float& outL, float& outR) noexcept;

    static float clamp01(float value) noexcept;
    static float midiToHz(int note) noexcept;

    double sampleRate_ = 48000.0;
    Parameters parameters_ {};
    Archetype archetype_ = Archetype::Nocturne;
    Rng rng_ {};
    Rng presenceRng_ {};

    bool gate_ = false;
    int midiNote_ = 36;
    float velocity_ = 1.0f;
    float envelope_ = 0.0f;

    std::array<double, 4> oscillatorPhase_ {};
    std::array<float, 4> oscillatorDrift_ {};

    float noiseStateL_ = 0.0f;
    float noiseStateR_ = 0.0f;
    float textureHpState_ = 0.0f;
    float motionPan_ = 0.0f;
    float motionSpectral_ = 0.0f;

    // Lorenz-like deterministic macro-state.
    double chaosX_ = 0.1;
    double chaosY_ = 0.0;
    double chaosZ_ = 0.0;
    int controlCountdown_ = 0;
    static constexpr int kControlPeriod = 64;

    std::array<Resonator, 12> resonators_ {};
    std::array<Resonator, 4> presenceResonators_ {};
    float bodyExcitation_ = 0.0f;
    float presenceNoiseLowpass_ = 0.0f;

    static constexpr std::size_t kWorldPoolCapacity = 3;
    static constexpr std::size_t kTexturePoolCapacity = 3;
    static constexpr std::size_t kEventPoolCapacity = 6;
    static constexpr std::size_t kBodyPoolCapacity = 4;

    std::array<const Clip*, kWorldPoolCapacity> worldPool_ {};
    std::array<const Clip*, kTexturePoolCapacity> texturePool_ {};
    std::array<const Clip*, kEventPoolCapacity> eventPool_ {};
    std::array<const Clip*, kBodyPoolCapacity> bodyPool_ {};
    std::size_t worldPoolCount_ = 0;
    std::size_t texturePoolCount_ = 0;
    std::size_t eventPoolCount_ = 0;
    std::size_t bodyPoolCount_ = 0;

    std::array<StreamVoice, kWorldPoolCapacity> worldVoices_ {};
    std::array<StreamVoice, kTexturePoolCapacity> textureVoices_ {};
    std::array<float, kWorldPoolCapacity> worldLayerGain_ {};
    std::array<float, kWorldPoolCapacity> worldLayerTarget_ {};
    std::array<float, kTexturePoolCapacity> textureLayerGain_ {};
    std::array<float, kTexturePoolCapacity> textureLayerTarget_ {};

    StreamVoice eventVoice_ {};
    StreamVoice bodyExciterVoice_ {};
    float bodyExciterLowpass_ = 0.0f;

    std::uint64_t eventCount_ = 0;
    int eventCountdown_ = 0;

    std::array<DelayLine, 4> delays_ {};
    std::array<float, 4> reverbDamping_ {};
};

} // namespace noctomorph
