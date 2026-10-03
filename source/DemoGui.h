#pragma once

#include "vstgui/lib/cview.h"


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


} // namespace Noctomorph
