#include "controller.h"
#include "parameters.h"
#include "state_format.h"

#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vstparameters.h"

#include <algorithm>

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Noctomorph {

tresult PLUGIN_API Controller::initialize(FUnknown* context) {
    const auto result = EditController::initialize(context);
    if (result != kResultOk)
        return result;

    auto* archetype = new StringListParameter(
        STR16("Archetype"), kArchetype, nullptr,
        ParameterInfo::kCanAutomate | ParameterInfo::kIsList);
    archetype->appendString(STR16("Void"));
    archetype->appendString(STR16("Ruins"));
    archetype->appendString(STR16("Industrial"));
    archetype->appendString(STR16("Wasteland"));
    archetype->appendString(STR16("Abyss"));
    archetype->appendString(STR16("Nocturne"));
    parameters.addParameter(archetype);

    auto addPercent = [&](const TChar* title, ParamID id, double def) {
        auto* parameter = new RangeParameter(
            title, id, STR16("%"), 0.0, 100.0, def * 100.0);
        parameter->setPrecision(0);
        parameters.addParameter(parameter);
    };

    addPercent(STR16("Foundation"), kFoundation, 0.50);
    addPercent(STR16("World"), kWorld, 0.35);
    addPercent(STR16("Texture"), kTexture, 0.25);
    addPercent(STR16("Body"), kBody, 0.35);
    addPercent(STR16("Tension"), kTension, 0.25);
    addPercent(STR16("Motion"), kMotion, 0.35);
    addPercent(STR16("Evolve"), kEvolve, 0.35);
    addPercent(STR16("Events"), kEvents, 0.18);
    addPercent(STR16("Space"), kSpace, 0.35);
    addPercent(STR16("Output"), kOutput, 0.50);

    return kResultOk;
}

tresult PLUGIN_API Controller::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float values[kStateValueCount] {};
    if (!readState(stream, values))
        return kResultFalse;

    const ParamID ids[kStateValueCount] = {
        kArchetype, kFoundation, kWorld, kTexture, kBody,
        kTension, kEvolve, kEvents, kSpace, kOutput, kMotion
    };

    for (int i = 0; i < kStateValueCount; ++i)
        setParamNormalized(ids[i], std::clamp<double>(values[i], 0.0, 1.0));

    return kResultOk;
}

} // namespace Noctomorph
