#pragma once

#include "vstgui/lib/controls/cknob.h"
#include "vstgui/lib/controls/ccontrol.h"
#include "vstgui/lib/cview.h"

#include <string>

namespace Noctomorph {

class DemoFaceplate final : public VSTGUI::CView {
public:
    explicit DemoFaceplate(const VSTGUI::CRect& size);
    DemoFaceplate(const DemoFaceplate& other);
    VSTGUI::CBaseObject* newCopy() const override {
        return new DemoFaceplate(*this);
    }
    void draw(VSTGUI::CDrawContext* context) override;
};

class DemoKnob final : public VSTGUI::CKnobBase {
public:
    DemoKnob(
        const VSTGUI::CRect& size,
        VSTGUI::IControlListener* listener,
        int32_t tag,
        std::string label,
        float defaultValue);
    DemoKnob(const DemoKnob& other);
    VSTGUI::CBaseObject* newCopy() const override {
        return new DemoKnob(*this);
    }

    void draw(VSTGUI::CDrawContext* context) override;
    VSTGUI::CMouseEventResult onMouseDown(
        VSTGUI::CPoint& where,
        const VSTGUI::CButtonState& buttons) override;

private:
    std::string label_;
};

class DemoArchetypeSelector final : public VSTGUI::CControl {
public:
    DemoArchetypeSelector(
        const VSTGUI::CRect& size,
        VSTGUI::IControlListener* listener,
        int32_t tag,
        float defaultValue);
    DemoArchetypeSelector(const DemoArchetypeSelector& other);
    VSTGUI::CBaseObject* newCopy() const override {
        return new DemoArchetypeSelector(*this);
    }

    void draw(VSTGUI::CDrawContext* context) override;
    VSTGUI::CMouseEventResult onMouseDown(
        VSTGUI::CPoint& where,
        const VSTGUI::CButtonState& buttons) override;
};

} // namespace Noctomorph
