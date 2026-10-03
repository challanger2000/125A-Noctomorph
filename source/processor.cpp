#include "processor.h"
#include "ids.h"
#include "parameters.h"
#include "state_format.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/vstspeaker.h"

#include <algorithm>
#include <array>
#include <cmath>

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Noctomorph {
namespace {

constexpr std::uint64_t kPrototypeSeed = 0x125A4E4F43545653ULL;

int archetypeIndex(float normalized) noexcept {
    const float v = std::clamp(
        std::isfinite(normalized) ? normalized : 0.0f, 0.0f, 1.0f);
    return std::clamp(static_cast<int>(std::floor(v * 6.0f)), 0, 5);
}

noctomorph::Archetype archetypeFromIndex(int index) noexcept {
    switch (std::clamp(index, 0, 5)) {
        case 0: return noctomorph::Archetype::Void;
        case 1: return noctomorph::Archetype::Ruins;
        case 2: return noctomorph::Archetype::Industrial;
        case 3: return noctomorph::Archetype::Wasteland;
        case 4: return noctomorph::Archetype::Abyss;
        default: return noctomorph::Archetype::Nocturne;
    }
}

float archetypeNormalized(noctomorph::Archetype archetype) noexcept {
    return static_cast<float>(static_cast<unsigned>(archetype)) / 5.0f;
}

} // namespace

Processor::Processor() {
    setControllerClass(ControllerUID);
}

tresult PLUGIN_API Processor::initialize(FUnknown* context) {
    const auto result = AudioEffect::initialize(context);
    if (result != kResultOk)
        return result;

    addAudioOutput(STR16("Stereo Out"), SpeakerArr::kStereo);
    addEventInput(STR16("Event In"), 16);

    // Asset loading occurs during component initialization, never in process().
    // Failure leaves a valid synthetic-fallback instrument.
    assetsLoaded_ = assetBank_.load();
    applyAssetProfile();
    return kResultOk;
}

tresult PLUGIN_API Processor::setupProcessing(ProcessSetup& setup) {
    const auto result = AudioEffect::setupProcessing(setup);
    if (result != kResultOk)
        return result;

    sampleRate_ = setup.sampleRate > 1.0 ? setup.sampleRate : 48000.0;
    engine_.prepare(sampleRate_);
    resetEngine();
    return kResultOk;
}

tresult PLUGIN_API Processor::setProcessing(TBool state) {
    if (state)
        resetEngine();
    return kResultOk;
}

tresult PLUGIN_API Processor::setActive(TBool state) {
    if (state)
        resetEngine();
    return AudioEffect::setActive(state);
}

tresult PLUGIN_API Processor::canProcessSampleSize(int32 symbolicSampleSize) {
    return symbolicSampleSize == kSample32 ? kResultTrue : kResultFalse;
}

tresult PLUGIN_API Processor::setBusArrangements(
    SpeakerArrangement* inputs, int32 numIns,
    SpeakerArrangement* outputs, int32 numOuts) {

    if (numIns != 0 || !outputs || numOuts != 1 ||
        outputs[0] != SpeakerArr::kStereo)
        return kResultFalse;

    return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
}

void Processor::applyAssetProfile() noexcept {
    if (!assetsLoaded_) {
        engine_.setWorldPool(nullptr, 0);
        engine_.setTexturePool(nullptr, 0);
        engine_.setBodyExciterPool(nullptr, 0);
        engine_.setEventPool(nullptr, 0);
        return;
    }

    const auto scene = assetBank_.sceneFor(noctomorph::Archetype::Industrial);
    engine_.setWorldPool(scene.world.data(), scene.worldCount);
    engine_.setTexturePool(scene.texture.data(), scene.textureCount);
    engine_.setBodyExciterPool(scene.body.data(), scene.bodyCount);
    engine_.setEventPool(scene.event.data(), scene.eventCount);
}
void Processor::resetEngine() noexcept {
    activeNoteId_ = -1;
    activePitch_ = -1;
    transportWasPlaying_ = false;
    engine_.reset(droneSeed_);
    archetype_ = noctomorph::Archetype::Industrial;
    engine_.setArchetype(archetype_);
    applyAssetProfile();
    updateEngineParameters();
}

void Processor::applyParameter(ParamID id, float normalized) noexcept {
    const float v = std::clamp(
        std::isfinite(normalized) ? normalized : 0.0f, 0.0f, 1.0f);

    switch (id) {
        case kArchetype:
            randomControl_ = v;
            droneSeed_ =
                0x125A4E4F43540000ULL ^
                (static_cast<std::uint64_t>(v * 4294967295.0f) * 0x9E3779B97F4A7C15ULL);
            randomChanged_ = true;
            break;
        case kFoundation:
            intensity_ = v;
            break;
        case kWorld:      parameters_.world = v; break;
        case kTexture:    parameters_.texture = v; break;
        case kBody:       parameters_.body = v; break;
        case kTension:    parameters_.tension = v; break;
        case kMotion:     parameters_.motion = v; break;
        case kEvolve:     parameters_.evolve = v; break;
        case kEvents:     parameters_.events = v; break;
        case kSpace:      parameters_.space = v; break;
        case kOutput:     parameters_.output = v; break;
        default: break;
    }
}

void Processor::updateEngineParameters() noexcept {
    const float i = std::clamp(intensity_, 0.0f, 1.0f);
    const float dna = std::clamp(randomControl_, 0.0f, 1.0f);

    // RANDOM selects one of eight deliberately separated drone families.
    // The fractional part still varies each family without collapsing the
    // audible differences between them.
    const float scaled = std::min(dna * 8.0f, 7.999f);
    const int family = static_cast<int>(scaled);
    const float morph = scaled - static_cast<float>(family);

    static constexpr float kFoundation[8] = {
        0.82f, 0.30f, 0.55f, 0.18f, 0.70f, 0.24f, 0.46f, 0.62f
    };
    static constexpr float kWorld[8] = {
        0.64f, 0.84f, 0.36f, 0.52f, 0.28f, 0.72f, 0.44f, 0.58f
    };
    static constexpr float kTexture[8] = {
        0.20f, 0.48f, 0.86f, 0.72f, 0.38f, 0.92f, 0.60f, 0.32f
    };
    static constexpr float kBody[8] = {
        0.16f, 0.42f, 0.24f, 0.68f, 0.52f, 0.18f, 0.76f, 0.34f
    };
    static constexpr float kTension[8] = {
        0.18f, 0.66f, 0.42f, 0.82f, 0.30f, 0.74f, 0.54f, 0.94f
    };
    static constexpr float kMotion[8] = {
        0.12f, 0.54f, 0.86f, 0.28f, 0.72f, 0.94f, 0.40f, 0.64f
    };
    static constexpr float kEvolve[8] = {
        0.28f, 0.72f, 0.48f, 0.88f, 0.36f, 0.62f, 0.96f, 0.54f
    };
    static constexpr float kSpace[8] = {
        0.08f, 0.22f, 0.14f, 0.34f, 0.18f, 0.10f, 0.28f, 0.20f
    };

    const float micro = (morph - 0.5f) * 0.12f;
    // INTENSITY is timbral density/aggression, not a volume fader.
    parameters_.foundation = std::clamp(kFoundation[family] + 0.10f * i, 0.0f, 1.0f);
    parameters_.world = std::clamp(kWorld[family] + 0.08f * i, 0.0f, 1.0f);
    parameters_.texture = std::clamp(kTexture[family] + 0.42f * i + micro, 0.0f, 1.0f);
    parameters_.body = std::clamp(kBody[family] + 0.30f * i, 0.0f, 1.0f);
    parameters_.tension = std::clamp(kTension[family] + 0.34f * i + micro, 0.0f, 1.0f);
    parameters_.motion = std::clamp(kMotion[family] + 0.36f * i, 0.0f, 1.0f);
    parameters_.evolve = std::clamp(kEvolve[family] + 0.28f * i, 0.0f, 1.0f);
    parameters_.events = 0.0f;
    parameters_.space = std::clamp(kSpace[family] + 0.14f * i, 0.0f, 0.55f);

    // Family-specific gain staging plus intensity compensation. The drone
    // should sit at a useful instrument level while INTENSITY changes timbre
    // much more than loudness.
    static constexpr float kFamilyOutput[8] = {
        0.94f, 0.86f, 0.90f, 0.82f, 0.88f, 0.80f, 0.84f, 0.82f
    };
    parameters_.output = std::clamp(
        kFamilyOutput[family] - 0.10f * i, 0.68f, 0.96f);

    archetype_ = noctomorph::Archetype::Industrial;
    engine_.setParameters(parameters_);
    engine_.setArchetype(archetype_);

    if (randomChanged_ && activePitch_ >= 0) {
        const int pitch = activePitch_;
        engine_.reset(droneSeed_);
        engine_.setArchetype(noctomorph::Archetype::Industrial);
        applyAssetProfile();
        engine_.setParameters(parameters_);
        engine_.noteOn(pitch, 1.0f);
    }
    randomChanged_ = false;
}

void Processor::handleNoteOn(const Event& event) noexcept {
    if (event.type != Event::kNoteOnEvent || event.noteOn.velocity <= 0.0f)
        return;

    // Monophonic scene instrument: each new note starts the same reproducible
    // world from its initial state. No time-based randomness is used.
    engine_.reset(droneSeed_);
    engine_.setArchetype(noctomorph::Archetype::Industrial);
    updateEngineParameters();
    engine_.noteOn(event.noteOn.pitch, event.noteOn.velocity);

    activeNoteId_ = event.noteOn.noteId;
    activePitch_ = event.noteOn.pitch;
}

void Processor::handleNoteOff(const Event& event) noexcept {
    int32 noteId = -1;
    int16 pitch = -1;

    if (event.type == Event::kNoteOffEvent) {
        noteId = event.noteOff.noteId;
        pitch = event.noteOff.pitch;
    } else if (event.type == Event::kNoteOnEvent &&
               event.noteOn.velocity <= 0.0f) {
        noteId = event.noteOn.noteId;
        pitch = event.noteOn.pitch;
    } else {
        return;
    }

    const bool idMatches =
        activeNoteId_ >= 0 && noteId >= 0 && activeNoteId_ == noteId;
    const bool pitchMatches =
        (activeNoteId_ < 0 || noteId < 0) && activePitch_ == pitch;

    if (idMatches || pitchMatches) {
        engine_.noteOff();
        activeNoteId_ = -1;
        activePitch_ = -1;
    }
}

tresult PLUGIN_API Processor::process(ProcessData& data) {
    auto consumeAllParameters = [&]() {
        bool changed = false;
        if (!data.inputParameterChanges)
            return changed;

        const int32 count = data.inputParameterChanges->getParameterCount();
        for (int32 i = 0; i < count; ++i) {
            auto* queue = data.inputParameterChanges->getParameterData(i);
            if (!queue)
                continue;

            const int32 pointCount = queue->getPointCount();
            for (int32 p = 0; p < pointCount; ++p) {
                int32 offset = 0;
                ParamValue value = 0.0;
                if (queue->getPoint(p, offset, value) == kResultTrue) {
                    applyParameter(
                        queue->getParameterId(),
                        static_cast<float>(value));
                    changed = true;
                }
            }
        }
        return changed;
    };

    if (data.numSamples <= 0) {
        if (consumeAllParameters())
            updateEngineParameters();
        return kResultOk;
    }

    if (data.numOutputs < 1)
        return kResultFalse;

    auto& out = data.outputs[0];
    if (out.numChannels != 2 || !out.channelBuffers32 ||
        !out.channelBuffers32[0] || !out.channelBuffers32[1])
        return kResultFalse;

    float* left = out.channelBuffers32[0];
    float* right = out.channelBuffers32[1];
    std::fill(left, left + data.numSamples, 0.0f);
    std::fill(right, right + data.numSamples, 0.0f);

    struct ParamCursor {
        IParamValueQueue* queue = nullptr;
        int32 point = 0;
    };
    std::array<ParamCursor, kParamCount> cursors {};

    auto slotFor = [](ParamID id) noexcept -> int {
        switch (id) {
            case kArchetype: return 0;
            case kFoundation: return 1;
            case kWorld: return 2;
            case kTexture: return 3;
            case kBody: return 4;
            case kTension: return 5;
            case kEvolve: return 6;
            case kEvents: return 7;
            case kSpace: return 8;
            case kOutput: return 9;
            case kMotion: return 10;
            default: return -1;
        }
    };

    if (data.inputParameterChanges) {
        const int32 count = data.inputParameterChanges->getParameterCount();
        for (int32 i = 0; i < count; ++i) {
            auto* queue = data.inputParameterChanges->getParameterData(i);
            if (!queue)
                continue;
            const int slot = slotFor(queue->getParameterId());
            if (slot >= 0)
                cursors[static_cast<std::size_t>(slot)].queue = queue;
        }
    }

    int32 eventIndex = 0;
    const int32 eventCount =
        data.inputEvents ? data.inputEvents->getEventCount() : 0;

    auto applyParametersAt = [&](int32 sampleOffset) {
        bool changed = false;
        for (auto& cursor : cursors) {
            if (!cursor.queue)
                continue;

            const int32 count = cursor.queue->getPointCount();
            while (cursor.point < count) {
                int32 offset = 0;
                ParamValue value = 0.0;
                if (cursor.queue->getPoint(cursor.point, offset, value) !=
                    kResultTrue) {
                    ++cursor.point;
                    continue;
                }

                offset = std::clamp(offset, 0, data.numSamples);
                if (offset > sampleOffset)
                    break;

                applyParameter(
                    cursor.queue->getParameterId(),
                    static_cast<float>(value));
                ++cursor.point;
                changed = true;
            }
        }
        if (changed)
            updateEngineParameters();
    };

    auto applyEventsAt = [&](int32 sampleOffset) {
        while (eventIndex < eventCount) {
            Event event {};
            if (data.inputEvents->getEvent(eventIndex, event) != kResultOk) {
                ++eventIndex;
                continue;
            }

            const int32 offset =
                std::clamp(event.sampleOffset, 0, data.numSamples);
            if (offset > sampleOffset)
                break;

            if (event.type == Event::kNoteOnEvent &&
                event.noteOn.velocity > 0.0f)
                handleNoteOn(event);
            else
                handleNoteOff(event);

            ++eventIndex;
        }
    };

    bool transportPlaying = transportWasPlaying_;
    if (data.processContext)
        transportPlaying =
            (data.processContext->state & ProcessContext::kPlaying) != 0;

    if (transportWasPlaying_ && !transportPlaying)
        resetEngine();
    transportWasPlaying_ = transportPlaying;

    float peak = 0.0f;

    for (int32 sample = 0; sample < data.numSamples; ++sample) {
        applyParametersAt(sample);
        applyEventsAt(sample);

        engine_.process(left + sample, right + sample, 1);
        peak = std::max(
            peak, std::max(std::fabs(left[sample]), std::fabs(right[sample])));
    }

    applyParametersAt(data.numSamples);
    applyEventsAt(data.numSamples);

    out.silenceFlags = peak < 1.0e-12f ? 0x3ull : 0;
    return kResultOk;
}

tresult PLUGIN_API Processor::setState(IBStream* state) {
    if (!state)
        return kResultFalse;
    IBStreamer stream(state, kLittleEndian);
    float values[kStateValueCount] {};
    if (!readState(stream, values))
        return kResultFalse;
    randomControl_ = values[0];
    intensity_ = values[1];
    droneSeed_ = 0x125A4E4F43540000ULL ^
        (static_cast<std::uint64_t>(randomControl_ * 4294967295.0f) *
         0x9E3779B97F4A7C15ULL);
    archetype_ = noctomorph::Archetype::Industrial;
    applyAssetProfile();
    updateEngineParameters();
    return kResultOk;
}

tresult PLUGIN_API Processor::getState(IBStream* state) {
    if (!state)
        return kResultFalse;
    IBStreamer stream(state, kLittleEndian);
    const float values[kStateValueCount] = {randomControl_, intensity_};
    return writeState(stream, values) ? kResultOk : kResultFalse;
}

uint32 PLUGIN_API Processor::getTailSamples() {
    const double samples = std::max(0.0, sampleRate_) * 20.0;
    return static_cast<uint32>(
        std::min<double>(samples, 0xFFFFFFFEu));
}

} // namespace Noctomorph
