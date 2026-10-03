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

    auto addPercent = [&](const TChar* title, ParamID id, double def) {
        auto* parameter = new RangeParameter(
            title, id, STR16("%"), 0.0, 100.0, def * 100.0);
        parameter->setPrecision(0);
        parameters.addParameter(parameter);
    };

    addPercent(STR16("Intensity"), kFoundation, 0.50);


    return kResultOk;
}

tresult PLUGIN_API Controller::setComponentState(IBStream* state) {
    if (!state)
        return kResultFalse;

    IBStreamer stream(state, kLittleEndian);
    float values[kStateValueCount] {};
    if (!readState(stream, values))
        return kResultFalse;

    setParamNormalized(
        kFoundation, std::clamp<double>(values[1], 0.0, 1.0));

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


    return nullptr;
}

} // namespace Noctomorph
