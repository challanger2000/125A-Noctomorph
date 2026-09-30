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
constexpr VSTGUI::CColor kBgTop {22, 25, 28, 255};
constexpr VSTGUI::CColor kBgBottom {7, 9, 11, 255};
constexpr VSTGUI::CColor kPanel {20, 23, 26, 245};
constexpr VSTGUI::CColor kPanelEdge {67, 74, 80, 220};
constexpr VSTGUI::CColor kText {222, 225, 226, 255};
constexpr VSTGUI::CColor kMuted {123, 132, 139, 255};
constexpr VSTGUI::CColor kAccent {112, 145, 164, 255};
constexpr VSTGUI::CColor kAccentHot {175, 210, 228, 255};

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

    VSTGUI::CRect header = r;
    header.bottom = header.top + 72.0;
    c->setFillColor({12, 14, 17, 230});
    c->drawRect(header, VSTGUI::kDrawFilled);

    c->setFont(VSTGUI::kNormalFont, 30.0, VSTGUI::kBoldFace);
    c->setFontColor(kText);
    VSTGUI::CRect title = header;
    title.left += 44.0;
    title.right -= 44.0;
    c->drawString(
        VSTGUI::UTF8String("125A  NOCTOMORPH"),
        title, VSTGUI::kLeftText);

    c->setFont(VSTGUI::kNormalFont, 10.5, VSTGUI::kNormalFace);
    c->setFontColor(kMuted);
    VSTGUI::CRect version = header;
    version.right -= 34.0;
    c->drawString(
        VSTGUI::UTF8String("DEMO GUI  |  v0.1.0"),
        version, VSTGUI::kRightText);

    VSTGUI::CRect topPanel {34, 170, 1086, 357};
    VSTGUI::CRect bottomPanel {34, 365, 1086, 552};
    fillRound(c, topPanel, 12.0, {29, 33, 36, 245}, {15, 18, 21, 245});
    fillRound(c, bottomPanel, 12.0, {28, 32, 35, 245}, {14, 17, 20, 245});
    strokeRound(c, topPanel, 12.0, kPanelEdge, 1.0);
    strokeRound(c, bottomPanel, 12.0, kPanelEdge, 1.0);

    c->setFont(VSTGUI::kNormalFont, 9.0, VSTGUI::kBoldFace);
    c->setFontColor({91, 102, 111, 255});
    c->drawString(
        VSTGUI::UTF8String("WORLD / MATERIAL"),
        VSTGUI::CRect{48, 176, 210, 192},
        VSTGUI::kLeftText);
    c->drawString(
        VSTGUI::UTF8String("BEHAVIOR"),
        VSTGUI::CRect{48, 371, 210, 387},
        VSTGUI::kLeftText);

    c->setFont(VSTGUI::kNormalFont, 9.0, VSTGUI::kNormalFace);
    c->setFontColor({92, 100, 106, 255});
    c->drawString(
        VSTGUI::UTF8String("CTRL + LEFT CLICK = RESET"),
        VSTGUI::CRect{44, 575, 360, 602},
        VSTGUI::kLeftText);

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
    c->setFillColor({28, 32, 35, 255});
    c->drawEllipse(knob, VSTGUI::kDrawFilled);
    c->setFrameColor({80, 89, 96, 255});
    c->setLineWidth(2.0);
    c->drawEllipse(knob, VSTGUI::kDrawStroked);

    VSTGUI::CRect cap {
        center.x - innerRadius,
        center.y - innerRadius,
        center.x + innerRadius,
        center.y + innerRadius
    };
    c->setFillColor({45, 50, 54, 255});
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

DemoArchetypeSelector::DemoArchetypeSelector(
    const VSTGUI::CRect& size,
    VSTGUI::IControlListener* listener,
    int32_t tag,
    float defaultValue)
: VSTGUI::CControl(size, listener, tag, nullptr) {

    setDefaultValue(defaultValue);
    setTransparency(true);
    setWantsFocus(true);
}

DemoArchetypeSelector::DemoArchetypeSelector(
    const DemoArchetypeSelector& other)
: VSTGUI::CControl(other) {}

void DemoArchetypeSelector::draw(VSTGUI::CDrawContext* c) {
    static constexpr std::array<const char*, 6> labels {
        "VOID", "RUINS", "INDUSTRIAL",
        "WASTELAND", "ABYSS", "NOCTURNE"
    };

    const auto r = getViewSize();
    const double cellWidth = r.getWidth() / 6.0;
    const int selected = std::clamp(
        static_cast<int>(std::lround(
            std::clamp<double>(getValueNormalized(), 0.0, 1.0) * 5.0)),
        0, 5);

    c->setDrawMode(VSTGUI::kAntiAliasing);
    c->setFont(VSTGUI::kNormalFont, 10.5, VSTGUI::kBoldFace);

    for (int i = 0; i < 6; ++i) {
        VSTGUI::CRect cell {
            r.left + i * cellWidth + 2.0,
            r.top + 2.0,
            r.left + (i + 1) * cellWidth - 2.0,
            r.bottom - 2.0
        };

        if (i == selected) {
            fillRound(
                c, cell, 6.0,
                {62, 77, 87, 255},
                {31, 41, 48, 255});
            strokeRound(c, cell, 6.0, kAccentHot, 1.5);
            c->setFontColor(kText);
        } else {
            fillRound(
                c, cell, 6.0,
                {30, 34, 38, 255},
                {18, 21, 24, 255});
            strokeRound(c, cell, 6.0, {55, 62, 68, 220}, 1.0);
            c->setFontColor(kMuted);
        }

        c->drawString(
            VSTGUI::UTF8String(labels[static_cast<std::size_t>(i)]),
            cell, VSTGUI::kCenterText);
    }

    setDirty(false);
}

VSTGUI::CMouseEventResult DemoArchetypeSelector::onMouseDown(
    VSTGUI::CPoint& where,
    const VSTGUI::CButtonState& buttons) {

    if (!buttons.isLeftButton() || !getViewSize().pointInside(where))
        return VSTGUI::kMouseEventNotHandled;

    beginEdit();

    if (buttons.isControlSet()) {
        setValueNormalized(getDefaultValue());
    } else {
        const auto r = getViewSize();
        const double rel =
            std::clamp((where.x - r.left) / r.getWidth(), 0.0, 0.999999);
        const int index = std::clamp(
            static_cast<int>(rel * 6.0), 0, 5);
        setValueNormalized(static_cast<float>(index) / 5.0f);
    }

    valueChanged();
    invalid();
    endEdit();
    return VSTGUI::kMouseDownEventHandledButDontNeedMovedOrUpEvents;
}

} // namespace Noctomorph
