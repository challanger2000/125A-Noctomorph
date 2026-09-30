#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "NoctomorphCore.h"

namespace Noctomorph {

class Processor final : public Steinberg::Vst::AudioEffect {
public:
    Processor();

    static Steinberg::FUnknown* createInstance(void*) {
        return static_cast<Steinberg::Vst::IAudioProcessor*>(new Processor());
    }

    Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) override;
    Steinberg::tresult PLUGIN_API setupProcessing(Steinberg::Vst::ProcessSetup& setup) override;
    Steinberg::tresult PLUGIN_API setProcessing(Steinberg::TBool state) override;
    Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) override;
    Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) override;
    Steinberg::tresult PLUGIN_API setBusArrangements(
        Steinberg::Vst::SpeakerArrangement* inputs,
        Steinberg::int32 numIns,
        Steinberg::Vst::SpeakerArrangement* outputs,
        Steinberg::int32 numOuts) override;
    Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) override;
    Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) override;
    Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) override;
    Steinberg::uint32 PLUGIN_API getTailSamples() override;

private:
    void applyParameter(Steinberg::Vst::ParamID id, float normalized) noexcept;
    void updateEngineParameters() noexcept;
    void resetEngine() noexcept;
    void handleNoteOn(const Steinberg::Vst::Event& event) noexcept;
    void handleNoteOff(const Steinberg::Vst::Event& event) noexcept;

    noctomorph::Engine engine_{};
    noctomorph::Parameters parameters_{};
    // StringListParameter defaults to its first entry; keep DSP state identical.
    noctomorph::Archetype archetype_ = noctomorph::Archetype::Void;

    double sampleRate_ = 48000.0;
    Steinberg::int32 activeNoteId_ = -1;
    Steinberg::int16 activePitch_ = -1;
    bool transportWasPlaying_ = false;
};

} // namespace Noctomorph
