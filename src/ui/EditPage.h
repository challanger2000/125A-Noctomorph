#pragma once

#include "Theme.h"

// Edit page: the 12-slot chain. Slot 1 (spectral) and slot 12 (effects and
// master) are fixed cards on top; slots 2..11 are the module cards below.
namespace ui
{
struct SpectralCard : public juce::Component
{
    SpectralCard (V4JBAudioProcessor& p)
        : bars (p, 0), algo (p.apvts, ParamID::spAlgo)
    {
        auto& s = p.apvts;
        namespace I = ParamID;
        const std::pair<const char*, const char*> defs[] = {
            { I::spDepth, "Depth" }, { I::spTilt, "Tilt" }, { I::spPartials, "Partials" },
            { I::spModDepth, "Mod Depth" }, { I::spModRate, "Mod Rate" }, { I::sub1, "Sub -1" },
            { I::nzMix, "Noise" }, { I::nzWidth, "Width" }, { I::nzLow, "Low Cut" },
            { I::nzHigh, "High Cut" }, { I::nzDecay, "N. Decay" }, { I::sub2, "Sub -2" } };
        for (auto& d : defs)
            addAndMakeVisible (knobs.add (new Knob (s, d.first, d.second)));
        addAndMakeVisible (bars);
        addAndMakeVisible (algo);
    }
    void paint (juce::Graphics& g) override { paintCard (g, getLocalBounds(), "1  SPECTRAL"); }
    void resized() override
    {
        auto r = getLocalBounds().reduced (8, 4);
        auto head = r.removeFromTop (20);
        algo.setBounds (head.removeFromRight (160));
        r.removeFromTop (4);
        bars.setBounds (r.removeFromTop (48));
        r.removeFromTop (4);
        const int h = r.getHeight() / 2, w = r.getWidth() / 6;
        for (int i = 0; i < knobs.size(); ++i)
            knobs[i]->setBounds (r.getX() + (i % 6) * w, r.getY() + (i / 6) * h, w, h);
    }
    SpectrumBars bars;
    Combo algo;
    juce::OwnedArray<Knob> knobs;
};

struct FxCard : public juce::Component
{
    FxCard (V4JBAudioProcessor& p)
    {
        auto& s = p.apvts;
        namespace I = ParamID;
        auto knob = [&] (const char* id, const char* name)
        { auto* k = knobs.add (new Knob (s, id, name)); addAndMakeVisible (k); return k; };
        auto tog = [&] (const char* id, const char* name)
        { auto* t = toggles.add (new Toggle (s, id, name)); addAndMakeVisible (t); return t; };
        auto combo = [&] (const char* id)
        { auto* c = combos.add (new Combo (s, id)); addAndMakeVisible (c); return c; };

        cols[0] = { "DRIVE",  tog (I::drOn, "ON"), nullptr, nullptr,
                    { knob (I::drAmt, "DRIVE"), knob (I::drMix, "MIX") } };
        cols[1] = { "MOD", tog (I::mdOn, "ON"), combo (I::mdType), nullptr,
                    { knob (I::mdRate, "RATE"), knob (I::mdDepth, "DEPTH"), knob (I::mdMix, "MIX") } };
        cols[2] = { "DELAY", tog (I::dlOn, "ON"), combo (I::dlDiv), tog (I::dlSync, "SYNC"),
                    { knob (I::dlTime, "TIME"), knob (I::dlFb, "FEEDBACK"), knob (I::dlMix, "MIX") } };
        cols[3] = { "REVERB", tog (I::rvOn, "ON"), nullptr, nullptr,
                    { knob (I::rvSize, "SIZE"), knob (I::rvDamp, "DAMPING"), knob (I::rvMix, "MIX") } };
        cols[4] = { "MASTER", nullptr, nullptr, nullptr,
                    { knob (I::width, "WIDTH"), knob (I::spread, "SPREAD") } };

        for (int i = 0; i < 5; ++i)
        {
            auto* button = pageButtons.add (new juce::TextButton (cols[i].title));
            button->onClick = [this, i] { setEffect (i); };
            addAndMakeVisible (button);
        }
        setEffect (0);
    }

    void setEffect (int index)
    {
        activeEffect = juce::jlimit (0, 4, index);
        for (int i = 0; i < 5; ++i)
        {
            pageButtons[i]->setToggleState (i == activeEffect, juce::dontSendNotification);
            const auto& group = cols[i];
            if (group.on != nullptr) group.on->setVisible (i == activeEffect);
            if (group.combo != nullptr) group.combo->setVisible (i == activeEffect);
            if (group.extra != nullptr) group.extra->setVisible (i == activeEffect);
            for (auto* k : group.knobs) k->setVisible (i == activeEffect);
        }
        resized();
        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        paintCard (g, getLocalBounds(), "12  EFFECTS & MASTER");
        auto label = getLocalBounds().reduced (22, 4).withTrimmedTop (63).removeFromTop (22);
        g.setColour (kFog.withAlpha (0.88f));
        g.setFont (boldFont (11.f));
        g.drawText (juce::String (cols[activeEffect].title) + "  /  EFFECT SETTINGS",
                    label, juce::Justification::centredLeft, false);
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (14, 4);
        r.removeFromTop (27);
        auto nav = r.removeFromTop (36);
        const int navW = nav.getWidth() / 5;
        for (auto* b : pageButtons)
            b->setBounds (nav.removeFromLeft (navW).reduced (4, 3));
        r.removeFromTop (24);
        auto options = r.removeFromTop (32);
        auto& col = cols[activeEffect];
        if (col.on != nullptr)
            col.on->setBounds (options.removeFromLeft (90).reduced (4, 3));
        if (col.extra != nullptr)
            col.extra->setBounds (options.removeFromLeft (100).reduced (4, 3));
        if (col.combo != nullptr)
            col.combo->setBounds (options.removeFromLeft (245).reduced (4, 3));

        const int n = (int) col.knobs.size();
        const int width = juce::jmin (n * 185, r.getWidth() - 20);
        auto knobsArea = r.withSizeKeepingCentre (width, r.getHeight());
        for (auto* k : col.knobs)
            k->setBounds (knobsArea.removeFromLeft (width / n).reduced (16, 1));
    }

    struct Column
    {
        const char* title;
        juce::ToggleButton* on;
        juce::ComboBox* combo;
        juce::ToggleButton* extra;
        std::vector<Knob*> knobs;
    };
    Column cols[5];
    juce::OwnedArray<Knob> knobs;
    juce::OwnedArray<Toggle> toggles;
    juce::OwnedArray<Combo> combos;
    juce::OwnedArray<juce::TextButton> pageButtons;
    int activeEffect = 0;
};

// One user slot. MOD flips the lower half between the module's knobs and its
// three modulation routes (envelope, LFO, MIDI -> main knob).
struct SlotCard : public juce::Component
{
    SlotCard (V4JBAudioProcessor& p, int slotIndex)
        : proc (p), index (slotIndex), bars (p, slotIndex + 1),
          type (p.apvts, ParamID::slot (slotIndex, "type")),
          main (p.apvts, ParamID::slot (slotIndex, "main"), "Main"),
          routes { { p.apvts, ParamID::slot (slotIndex, "es") },
                   { p.apvts, ParamID::slot (slotIndex, "ls") },
                   { p.apvts, ParamID::slot (slotIndex, "ms") } }
    {
        typeParam = p.apvts.getRawParameterValue (ParamID::slot (slotIndex, "type"));
        addAndMakeVisible (bars);
        addAndMakeVisible (type);
        addAndMakeVisible (main);
        for (int k = 0; k < 4; ++k)
            addAndMakeVisible (params.add (new HSlider (p.apvts, ParamID::slotP (slotIndex, k), "")));
        const char* amtKeys[3] = { "ea", "la", "ma" };
        for (int i = 0; i < 3; ++i)
        {
            auto& rt = routes[i];
            rt.amount.setSliderStyle (juce::Slider::LinearHorizontal);
            rt.amount.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
            rt.amount.setColour (juce::Slider::trackColourId, kAmber.withAlpha (0.6f));
            rt.amount.setPopupDisplayEnabled (true, false, nullptr);
            rt.att = std::make_unique<APVTS::SliderAttachment> (p.apvts, ParamID::slot (slotIndex, amtKeys[i]), rt.amount);
            addChildComponent (rt.source);
            addChildComponent (rt.amount);
        }
        modBtn.setClickingTogglesState (true);
        modBtn.onClick = [this] { showRoutes (modBtn.getToggleState()); };
        addAndMakeVisible (modBtn);
        refresh();
    }

    void showRoutes (bool on)
    {
        for (auto& rt : routes)
        {
            rt.source.setVisible (on);
            rt.amount.setVisible (on);
        }
        lastType = -1;  // re-run label visibility
        refresh();
    }

    // Timer tick: bars, and labels when the module type changed (UI or automation).
    void refresh()
    {
        bars.refresh();
        const int t = juce::jlimit (0, v4jb::kNumModuleTypes - 1, (int) std::lround (typeParam->load()));
        if (t == lastType)
            return;
        lastType = t;
        const auto& info = v4jb::kModuleInfo[t];
        main.label.setText (info.labels[0], juce::dontSendNotification);
        main.setVisible (t != 0);
        const bool routesOn = modBtn.getToggleState();
        for (int k = 0; k < 4; ++k)
        {
            params[k]->label.setText (info.labels[k + 1], juce::dontSendNotification);
            params[k]->setVisible (! routesOn && info.labels[k + 1][0] != 0);
        }
    }

    void paint (juce::Graphics& g) override
    {
        paintCard (g, getLocalBounds(), juce::String (index + 2));
        if (modBtn.getToggleState())
        {
            g.setFont (boldFont (9.f));
            g.setColour (kFog.withAlpha (0.6f));
            g.drawText ("routes modulate MAIN", routeArea.withTrimmedTop (routeArea.getHeight() - 15),
                        juce::Justification::centredLeft, false);
        }
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (14, 8);
        auto header = r.removeFromTop (30);
        header.removeFromLeft (60); // slot number is painted by the card
        modBtn.setBounds (header.removeFromRight (100).reduced (5, 2));
        type.setBounds (header.removeFromLeft (juce::jmin (365, header.getWidth())).reduced (5, 2));
        r.removeFromTop (8);

        // One full-width inspector, split into spectrum/main and parameter routes.
        auto left = r.removeFromLeft (r.getWidth() * 38 / 100).reduced (8, 2);
        bars.setBounds (left.removeFromTop (juce::jmin (115, left.getHeight() / 2)));
        main.setBounds (left.withSizeKeepingCentre (135, juce::jmin (140, left.getHeight())));
        routeArea = r.reduced (9, 3);
        auto pr = routeArea;
        const int rowH = juce::jmax (32, pr.getHeight() / 4);
        for (auto* slider : params)
            slider->setBounds (pr.removeFromTop (juce::jmin (rowH, pr.getHeight())).reduced (8, 3));
        auto rr = routeArea;
        const int routeH = juce::jmax (36, rr.getHeight() / 3);
        for (auto& rt : routes)
        {
            auto line = rr.removeFromTop (juce::jmin (routeH, rr.getHeight()));
            rt.source.setBounds (line.removeFromLeft (juce::jmin (210, line.getWidth() / 2)).reduced (3, 4));
            rt.amount.setBounds (line.reduced (8, 6));
        }
    }

    struct Route
    {
        Route (APVTS& st, const juce::String& id) : source (st, id) {}
        Combo source;
        juce::Slider amount;
        std::unique_ptr<APVTS::SliderAttachment> att;
    };

    V4JBAudioProcessor& proc;
    int index;
    SpectrumBars bars;
    Combo type;
    Knob main;
    juce::OwnedArray<HSlider> params;
    Route routes[3];
    juce::ToggleButton modBtn { "MOD" };
    juce::Rectangle<int> routeArea;
    std::atomic<float>* typeParam = nullptr;
    int lastType = -1;
};

struct EditPage : public juce::Component
{
    EditPage (V4JBAudioProcessor& p) : spectral (p), fx (p)
    {
        addAndMakeVisible (spectral);
        addAndMakeVisible (fx);
        for (int i = 0; i < v4jb::kNumSlots; ++i)
        {
            addAndMakeVisible (slots.add (new SlotCard (p, i)));
            auto* button = slotButtons.add (new juce::TextButton (juce::String (i + 2).paddedLeft ('0', 2)));
            button->onClick = [this, i] { setSlot (i); };
            addAndMakeVisible (button);
        }

        sourceBtn.onClick = [this] { setTop (0); };
        fxBtn.onClick = [this] { setTop (1); };
        addAndMakeVisible (sourceBtn);
        addAndMakeVisible (fxBtn);
        setTop (0);
        setSlot (0);
    }

    void setTop (int selected)
    {
        topPage = juce::jlimit (0, 1, selected);
        spectral.setVisible (topPage == 0);
        fx.setVisible (topPage == 1);
        sourceBtn.setToggleState (topPage == 0, juce::dontSendNotification);
        fxBtn.setToggleState (topPage == 1, juce::dontSendNotification);
        resized();
        repaint();
    }

    void setSlot (int index)
    {
        activeSlot = juce::jlimit (0, v4jb::kNumSlots - 1, index);
        for (int i = 0; i < slots.size(); ++i)
        {
            slots[i]->setVisible (i == activeSlot);
            slotButtons[i]->setToggleState (i == activeSlot, juce::dontSendNotification);
        }
        resized();
        repaint();
    }

    void refresh()
    {
        if (spectral.isVisible())
            spectral.bars.refresh();
        for (int i = 0; i < slots.size(); ++i)
        {
            const auto v = slots[i]->typeParam->load();
            const int index = juce::jlimit (0, v4jb::kNumModuleTypes - 1, (int) std::lround (v));
            const auto name = juce::String (i + 2).paddedLeft ('0', 2) + "  " +
                              juce::String (v4jb::kModuleInfo[index].name);
            if (slotButtons[i]->getButtonText() != name)
                slotButtons[i]->setButtonText (name);
        }
        slots[activeSlot]->refresh();
    }

    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().reduced (12);
        auto top = r.removeFromTop (r.getHeight() * 46 / 100);
        g.setColour (kFog);
        g.setFont (boldFont (12.f));
        g.drawText ("SPECTRAL SOURCE  /  EFFECTS & MASTER",
                    top.removeFromTop (38), juce::Justification::centredLeft, false);
        g.drawText ("MODULE CHAIN  /  SELECT ONE OF 10 SLOTS",
                    r.removeFromTop (50), juce::Justification::centredLeft, false);
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (10);
        auto top = r.removeFromTop (r.getHeight() * 46 / 100);
        auto topBar = top.removeFromTop (38);
        fxBtn.setBounds (topBar.removeFromRight (124).reduced (3, 5));
        sourceBtn.setBounds (topBar.removeFromRight (124).reduced (3, 5));
        auto upper = top.reduced (4);
        spectral.setBounds (upper);
        fx.setBounds (upper);

        auto slotBar = r.removeFromTop (50);
        // Slot buttons are always visible; modules are never hidden or skipped.
        const int width = slotBar.getWidth() / slotButtons.size();
        for (auto* button : slotButtons)
            button->setBounds (slotBar.removeFromLeft (width).reduced (3, 6));

        for (int i = 0; i < slots.size(); ++i)
            if (i == activeSlot)
                slots[i]->setBounds (r.reduced (4, 2));
    }

    SpectralCard spectral;
    FxCard fx;
    juce::OwnedArray<SlotCard> slots;
    juce::OwnedArray<juce::TextButton> slotButtons;
    juce::TextButton sourceBtn { "SOURCE" }, fxBtn { "FX & MASTER" };
    int activeSlot = 0, topPage = 0;
};
} // namespace ui
