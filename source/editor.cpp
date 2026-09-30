#include "controller.h"

#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"

#include <cstring>

namespace Noctomorph {

Steinberg::IPlugView* PLUGIN_API Controller::createView(
    Steinberg::FIDString name) {

    if (std::strcmp(name, Steinberg::Vst::ViewType::kEditor) != 0)
        return nullptr;

    auto* editor = new VSTGUI::VST3Editor(
        this, "view", "noctomorph-demo.uidesc");
    editor->setAllowedZoomFactors({1.0, 1.5});
    return editor;
}

} // namespace Noctomorph
