#include "DemoGui.h"

#include "vstgui/lib/cdrawcontext.h"
#include "vstgui/lib/cfont.h"
#include "vstgui/lib/cgradient.h"
#include "vstgui/lib/cgraphicspath.h"


namespace Noctomorph {
namespace {

constexpr VSTGUI::CColor kBgTop {255, 255, 255, 255};
constexpr VSTGUI::CColor kBgBottom {244, 244, 242, 255};
constexpr VSTGUI::CColor kText {12, 12, 12, 255};
constexpr VSTGUI::CColor kMuted {80, 80, 80, 255};

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
    c->setFont(VSTGUI::kNormalFont, 11.0, VSTGUI::kBoldFace);
    c->setFontColor(kText);
    c->drawString(VSTGUI::UTF8String("INTENSITY"),
        VSTGUI::CRect{20, 222, r.right - 20, 242}, VSTGUI::kCenterText);
    setDirty(false);
}

} // namespace Noctomorph
