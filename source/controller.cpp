#include "controller.h"
#include "DemoGui.h"
#include "parameters.h"
#include "state_format.h"

#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vstparameters.h"

#include <algorithm>
#include <cstring>

using namespace Steinberg;
using namespace Steinberg::Vst;

namespace Noctomorph {

tresult PLUGIN_API Controller::initialize(FUnknown* context) {
    const auto result = EditControllerEx1::initialize(context);
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

    addPercent(STR16("Foundation"), kFoundation, 0.30);
    addPercent(STR16("World"), kWorld, 0.42);
    addPercent(STR16("Texture"), kTexture, 0.34);
    addPercent(STR16("Body"), kBody, 0.30);
    addPercent(STR16("Tension"), kTension, 0.32);
    addPercent(STR16("Motion"), kMotion, 0.45);
    addPercent(STR16("Evolve"), kEvolve, 0.45);
    addPercent(STR16("Events"), kEvents, 0.00);
    addPercent(STR16("Space"), kSpace, 0.45);
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

VSTGUI::CView* Controller::createCustomView(
    VSTGUI::UTF8StringPtr name,
    const VSTGUI::UIAttributes& attributes,
    const VSTGUI::IUIDescription*,
    VSTGUI::VST3Editor* editor) {

    if (!name || !editor)
        return nullptr;

    VSTGUI::CPoint origin {0, 0};
    VSTGUI::CPoint size {100, 100};
    attributes.getPointAttribute("origin", origin);
    attributes.getPointAttribute("size", size);
    const VSTGUI::CRect rect(
        origin.x, origin.y, origin.x + size.x, origin.y + size.y);

    if (std::strcmp(name, "Faceplate") == 0)
        return new DemoFaceplate(rect);

    if (std::strcmp(name, "Archetype") == 0)
        return new DemoArchetypeSelector(
            rect, editor, kArchetype, 0.0f);

    auto knob = [&](const char* viewName,
                    ParamID id,
                    const char* label,
                    float defaultValue) -> VSTGUI::CView* {
        if (std::strcmp(name, viewName) != 0)
            return nullptr;
        return new DemoKnob(
            rect, editor, id, label, defaultValue);
    };

    if (auto* v = knob("Foundation", kFoundation, "FOUNDATION", 0.30f)) return v;
    if (auto* v = knob("World",      kWorld,      "WORLD",      0.42f)) return v;
    if (auto* v = knob("Texture",    kTexture,    "TEXTURE",    0.34f)) return v;
    if (auto* v = knob("Body",       kBody,       "BODY",       0.30f)) return v;
    if (auto* v = knob("Space",      kSpace,      "SPACE",      0.45f)) return v;
    if (auto* v = knob("Tension",    kTension,    "TENSION",    0.32f)) return v;
    if (auto* v = knob("Motion",     kMotion,     "MOTION",     0.45f)) return v;
    if (auto* v = knob("Evolve",     kEvolve,     "EVOLVE",     0.45f)) return v;
    if (auto* v = knob("Events",     kEvents,     "EVENTS",     0.00f)) return v;
    if (auto* v = knob("Output",     kOutput,     "OUTPUT",     0.50f)) return v;

    return nullptr;
}

} // namespace Noctomorph
