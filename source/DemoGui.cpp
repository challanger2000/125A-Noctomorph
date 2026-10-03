#include "DemoGui.h"

#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/cfont.h"
#include "vstgui/lib/cgradient.h"
#include "vstgui/lib/cgraphicspath.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <utility>

namespace Noctomorph {
namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr VSTGUI::CColor kBgTop {255, 255, 255, 255};
constexpr VSTGUI::CColor kBgBottom {244, 244, 242, 255};
constexpr VSTGUI::CColor kPanel {20, 23, 26, 245};
constexpr VSTGUI::CColor kPanelEdge {67, 74, 80, 220};
constexpr VSTGUI::CColor kText {12, 12, 12, 255};
constexpr VSTGUI::CColor kMuted {80, 80, 80, 255};
constexpr VSTGUI::CColor kAccent {112, 145, 164, 255};
constexpr VSTGUI::CColor kAccentHot {245, 245, 245, 255};

void fillRound(
    VSTGUI::CDrawContext* c,
    const VSTGUI::CRect& r,
    double radius,
    VSTGUI::CColor a,
    VSTGUI::CColor b) {

    auto* path = c->createRoundRectGraphicsPath(r, radius);
    if (!path)
        return;
    auto* gradient = VSTGUI::CGradient::create(0.0, 1.0, a, b);
    if (gradient) {
        c->fillLinearGradient(
            path, *gradient,
            {r.left, r.top}, {r.left, r.bottom});
        gradient->forget();
    }
    path->forget();
}

void strokeRound(
    VSTGUI::CDrawContext* c,
    const VSTGUI::CRect& r,
    double radius,
    VSTGUI::CColor color,
    double width = 1.0) {

    auto* path = c->createRoundRectGraphicsPath(r, radius);
    if (!path)
        return;
    c->setFrameColor(color);
    c->setLineWidth(width);
    c->drawGraphicsPath(path, VSTGUI::CDrawContext::kPathStroked);
    path->forget();
}

} // namespace

DemoFaceplate::DemoFaceplate(const VSTGUI::CRect& size)
: VSTGUI::CView(size) {
    setMouseEnabled(false);
}

DemoFaceplate::DemoFaceplate(const DemoFaceplate& other)
: VSTGUI::CView(other) {}

void DemoFaceplate::draw(VSTGUI::CDrawContext* c) {
    const auto r = getViewSize();
    c->setDrawMode(VSTGUI::kAntiAliasing);
    fillRound(c, r, 0.0, kBgTop, kBgBottom);
    c->setFont(VSTGUI::kNormalFont, 18.0, VSTGUI::kBoldFace);
    c->setFontColor(kText);
    c->drawString(VSTGUI::UTF8String("NOCTOMORPH"),
        VSTGUI::CRect{20, 18, r.right - 20, 46}, VSTGUI::kCenterText);
    c->setFont(VSTGUI::kNormalFont, 8.5, VSTGUI::kNormalFace);
    c->setFontColor(kMuted);
    c->drawString(VSTGUI::UTF8String("125A  INDUSTRIAL DRONE GENERATOR"),
        VSTGUI::CRect{20, 45, r.right - 20, 63}, VSTGUI::kCenterText);
    setDirty(false);
}

DemoKnob::DemoKnob(
    const VSTGUI::CRect& size,
    VSTGUI::IControlListener* listener,
    int32_t tag,
    std::string label,
    float defaultValue)
: VSTGUI::CKnobBase(size, listener, tag, nullptr),
  label_(std::move(label)) {

    setStartAngle(static_cast<float>(135.0 / 180.0 * kPi));
    setRangeAngle(static_cast<float>(270.0 / 180.0 * kPi));
    setDefaultValue(defaultValue);
    setTransparency(true);
    setWantsFocus(true);
}

DemoKnob::DemoKnob(const DemoKnob& other)
: VSTGUI::CKnobBase(other), label_(other.label_) {}

void DemoKnob::draw(VSTGUI::CDrawContext* c) {
    const auto r = getViewSize();
    const auto center = VSTGUI::CPoint{
        r.getCenter().x,
        r.top + 66.0
    };
    const double value = std::clamp(
        static_cast<double>(getValueNormalized()), 0.0, 1.0);

    c->setDrawMode(VSTGUI::kAntiAliasing);

    const double outerRadius = 49.0;
    const double innerRadius = 39.0;

    for (int i = 0; i <= 20; ++i) {
        const bool major = (i % 5) == 0;
        const double a =
            (135.0 + 270.0 * static_cast<double>(i) / 20.0) *
            kPi / 180.0;
        const double ro = outerRadius + 6.0;
        const double ri = ro - (major ? 8.0 : 4.0);
        c->setFrameColor(
            major ? VSTGUI::CColor{119, 133, 143, 225}
                  : VSTGUI::CColor{61, 69, 75, 190});
        c->setLineWidth(major ? 1.5 : 1.0);
        c->drawLine(
            {center.x + std::cos(a) * ri,
             center.y + std::sin(a) * ri},
            {center.x + std::cos(a) * ro,
             center.y + std::sin(a) * ro});
    }

    VSTGUI::CRect shadow {
        center.x - outerRadius - 3.0,
        center.y - outerRadius - 1.0,
        center.x + outerRadius + 3.0,
        center.y + outerRadius + 5.0
    };
    c->setFillColor({0, 0, 0, 120});
    c->drawEllipse(shadow, VSTGUI::kDrawFilled);

    VSTGUI::CRect knob {
        center.x - outerRadius,
        center.y - outerRadius,
        center.x + outerRadius,
        center.y + outerRadius
    };
    c->setFillColor({8, 8, 8, 255});
    c->drawEllipse(knob, VSTGUI::kDrawFilled);
    c->setFrameColor({0, 0, 0, 255});
    c->setLineWidth(2.0);
    c->drawEllipse(knob, VSTGUI::kDrawStroked);

    VSTGUI::CRect cap {
        center.x - innerRadius,
        center.y - innerRadius,
        center.x + innerRadius,
        center.y + innerRadius
    };
    c->setFillColor({18, 18, 18, 255});
    c->drawEllipse(cap, VSTGUI::kDrawFilled);

    const double angle =
        (135.0 + 270.0 * value) * kPi / 180.0;
    c->setFrameColor(kAccentHot);
    c->setLineWidth(3.0);
    c->drawLine(
        {center.x + std::cos(angle) * 12.0,
         center.y + std::sin(angle) * 12.0},
        {center.x + std::cos(angle) * 33.0,
         center.y + std::sin(angle) * 33.0});

    c->setFont(VSTGUI::kNormalFont, 11.0, VSTGUI::kBoldFace);
    c->setFontColor(kText);
    c->drawString(
        VSTGUI::UTF8String(label_.c_str()),
        VSTGUI::CRect{r.left, r.top + 121.0, r.right, r.top + 140.0},
        VSTGUI::kCenterText);

    char valueText[16] {};
    std::snprintf(
        valueText, sizeof(valueText), "%d%%",
        static_cast<int>(std::lround(value * 100.0)));
    c->setFont(VSTGUI::kNormalFont, 9.0, VSTGUI::kNormalFace);
    c->setFontColor(kMuted);
    c->drawString(
        VSTGUI::UTF8String(valueText),
        VSTGUI::CRect{r.left, r.top + 140.0, r.right, r.bottom},
        VSTGUI::kCenterText);

    setDirty(false);
}

VSTGUI::CMouseEventResult DemoKnob::onMouseDown(
    VSTGUI::CPoint& where,
    const VSTGUI::CButtonState& buttons) {

    if (buttons.isLeftButton() && buttons.isControlSet()) {
        beginEdit();
        setValueNormalized(getDefaultValue());
        valueChanged();
        invalid();
        endEdit();
        return VSTGUI::kMouseDownEventHandledButDontNeedMovedOrUpEvents;
    }
    return VSTGUI::CKnobBase::onMouseDown(where, buttons);
}

DemoRandomButton::DemoRandomButton(
    const VSTGUI::CRect& size,
    VSTGUI::IControlListener* listener,
    int32_t tag,
    float defaultValue)
: VSTGUI::CControl(size, listener, tag, nullptr) {
    setDefaultValue(defaultValue);
    setTransparency(true);
    setWantsFocus(true);
}

DemoRandomButton::DemoRandomButton(const DemoRandomButton& other)
: VSTGUI::CControl(other) {}

void DemoRandomButton::draw(VSTGUI::CDrawContext* c) {
    const auto r = getViewSize();
    c->setDrawMode(VSTGUI::kAntiAliasing);
    fillRound(c, r, 7.0, {20, 20, 20, 255}, {0, 0, 0, 255});
    strokeRound(c, r, 7.0, {0, 0, 0, 255}, 1.0);
    c->setFont(VSTGUI::kNormalFont, 12.0, VSTGUI::kBoldFace);
    c->setFontColor({255, 255, 255, 255});
    c->drawString(VSTGUI::UTF8String("RANDOM"), r, VSTGUI::kCenterText);
    setDirty(false);
}

VSTGUI::CMouseEventResult DemoRandomButton::onMouseDown(
    VSTGUI::CPoint& where,
    const VSTGUI::CButtonState& buttons) {
    if (!buttons.isLeftButton() || !getViewSize().pointInside(where))
        return VSTGUI::kMouseEventNotHandled;
    beginEdit();
    float v = getValueNormalized() + 0.38196601125f;
    if (v >= 1.0f) v -= 1.0f;
    setValueNormalized(v);
    valueChanged();
    invalid();
    endEdit();
    return VSTGUI::kMouseDownEventHandledButDontNeedMovedOrUpEvents;
}

} // namespace Noctomorph
