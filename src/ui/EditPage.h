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
        auto knob = [&] (const char* id, const char* name) { auto* k = knobs.add (new Knob (s, id, name)); addAndMakeVisible (k); return k; };
        auto tog = [&] (const char* id, const char* name) { auto* t = toggles.add (new Toggle (s, id, name)); addAndMakeVisible (t); return t; };
        auto combo = [&] (const char* id) { auto* c = combos.add (new Combo (s, id)); addAndMakeVisible (c); return c; };

        cols[0] = { "DRIVE",  tog (I::drOn, "On"), nullptr, nullptr, { knob (I::drAmt, "Drive"), knob (I::drMix, "Mix") } };
        cols[1] = { "MOD",    tog (I::mdOn, "On"), combo (I::mdType), nullptr,
                    { knob (I::mdRate, "Rate"), knob (I::mdDepth, "Depth"), knob (I::mdMix, "Mix") } };
        cols[2] = { "DELAY",  tog (I::dlOn, "On"), combo (I::dlDiv), tog (I::dlSync, "Sync"),
                    { knob (I::dlTime, "Time"), knob (I::dlFb, "Feedback"), knob (I::dlMix, "Mix") } };
        cols[3] = { "REVERB", tog (I::rvOn, "On"), nullptr, nullptr,
                    { knob (I::rvSize, "Size"), knob (I::rvDamp, "Damping"), knob (I::rvMix, "Mix") } };
        cols[4] = { "MASTER", nullptr, nullptr, nullptr, { knob (I::width, "Width"), knob (I::spread, "Spread") } };
    }
    void paint (juce::Graphics& g) override
    {
        paintCard (g, getLocalBounds(), "12  EFFECTS & MASTER");
        g.setFont (boldFont (10.f));
        g.setColour (kFog.withAlpha (0.7f));
        for (auto& c : cols)
            g.drawText (c.title, c.titleArea, juce::Justification::centredLeft, false);
    }
    void resized() override
    {
        auto r = getLocalBounds().reduced (8, 4);
        r.removeFromTop (20);
        const int w = r.getWidth() / 5;
        for (auto& c : cols)
        {
            auto col = r.removeFromLeft (w).reduced (3, 0);
            auto head = col.removeFromTop (20);
            c.titleArea = head.removeFromLeft (46);
            if (c.on != nullptr)
                c.on->setBounds (head.removeFromLeft (34));
            auto line2 = col.removeFromTop (c.combo != nullptr || c.extra != nullptr ? 24 : 0);
            if (c.extra != nullptr)
                c.extra->setBounds (line2.removeFromLeft (44).reduced (0, 2));
            if (c.combo != nullptr)
                c.combo->setBounds (line2.reduced (2, 2));
            const int kh = juce::jmin (64, col.getHeight() / 2);
            for (size_t i = 0; i < c.knobs.size(); ++i)
                c.knobs[i]->setBounds (col.getX() + (int) (i % 2) * (col.getWidth() / 2), col.getY() + (int) (i / 2) * kh,
                                       col.getWidth() / 2, kh);
        }
    }
    struct Column
    {
        const char* title = "";
        juce::ToggleButton* on = nullptr;
        juce::ComboBox* combo = nullptr;
        juce::ToggleButton* extra = nullptr;
        std::vector<Knob*> knobs;
        juce::Rectangle<int> titleArea;
    };
    Column cols[5];
    juce::OwnedArray<Knob> knobs;
    juce::OwnedArray<Toggle> toggles;
    juce::OwnedArray<Combo> combos;
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
            rt.amount.setSliderStyle (juce::Slider::LinearBar);
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
            g.drawText ("routes push the main knob", routeArea.removeFromBottom (12), juce::Justification::centredLeft, false);
        }
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (6, 4);
        auto head = r.removeFromTop (20);
        head.removeFromLeft (18);
        modBtn.setBounds (head.removeFromRight (40));
        type.setBounds (head.reduced (2, 0));
        r.removeFromTop (3);
        bars.setBounds (r.removeFromTop (64));
        r.removeFromTop (3);
        main.setBounds (r.removeFromLeft (86));
        routeArea = r;
        auto pr = r;
        const int ph = juce::jlimit (24, 38, pr.getHeight() / 4);
        for (auto* s : params)
            s->setBounds (pr.removeFromTop (ph));
        auto rr = r;
        for (auto& rt : routes)
        {
            auto line = rr.removeFromTop (juce::jmin (28, rr.getHeight() / 3));
            rt.source.setBounds (line.removeFromLeft (line.getWidth() / 2 + 6).reduced (0, 3));
            rt.amount.setBounds (line.reduced (3, 6));
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
            addAndMakeVisible (slots.add (new SlotCard (p, i)));

        sourceBtn.onClick = [this] { setTop (0); };
        fxBtn.onClick     = [this] { setTop (1); };
        for (auto* b : { &sourceBtn, &fxBtn, &bankA, &bankB, &bankC, &bankD })
            addAndMakeVisible (*b);
        bankA.onClick = [this] { setBank (0); };
        bankB.onClick = [this] { setBank (1); };
        bankC.onClick = [this] { setBank (2); };
        bankD.onClick = [this] { setBank (3); };

        setTop (0);
        setBank (0);
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

    void setBank (int selected)
    {
        activeBank = juce::jlimit (0, 3, selected);
        juce::TextButton* buttons[] = { &bankA, &bankB, &bankC, &bankD };
        for (int i = 0; i < 4; ++i)
            buttons[i]->setToggleState (i == activeBank, juce::dontSendNotification);
        for (int i = 0; i < slots.size(); ++i)
            slots[i]->setVisible (i / 3 == activeBank);
        resized();
        repaint();
    }

    void refresh()
    {
        if (spectral.isVisible())
            spectral.bars.refresh();
        for (auto* s : slots)
            if (s->isVisible())
                s->refresh();
    }

    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().reduced (12);
        auto top = r.removeFromTop (r.getHeight() * 46 / 100);
        g.setColour (kFog);
        g.setFont (boldFont (12.f));
        g.drawText ("SPECTRAL SOURCE  /  EFFECTS & MASTER",
                    top.removeFromTop (38), juce::Justification::centredLeft, false);
        g.drawText ("MODULE CHAIN  /  SLOTS " +
                    juce::String (activeBank == 0 ? "02-04" :
                                  activeBank == 1 ? "05-07" :
                                  activeBank == 2 ? "08-10" : "11"),
                    r.removeFromTop (40), juce::Justification::centredLeft, false);
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (10);
        auto top = r.removeFromTop (r.getHeight() * 46 / 100);
        auto head = top.removeFromTop (38);
        fxBtn.setBounds (head.removeFromRight (124).reduced (3, 5));
        sourceBtn.setBounds (head.removeFromRight (124).reduced (3, 5));
        auto full = top.reduced (4);
        spectral.setBounds (full);
        fx.setBounds (full);

        auto bankBar = r.removeFromTop (40);
        bankD.setBounds (bankBar.removeFromRight (96).reduced (3, 5));
        bankC.setBounds (bankBar.removeFromRight (96).reduced (3, 5));
        bankB.setBounds (bankBar.removeFromRight (96).reduced (3, 5));
        bankA.setBounds (bankBar.removeFromRight (96).reduced (3, 5));

        const int w = r.getWidth() / 3;
        for (int i = 0; i < slots.size(); ++i)
            if (i / 3 == activeBank)
                slots[i]->setBounds (r.getX() + (i % 3) * w + 4, r.getY() + 2,
                                      w - 8, r.getHeight() - 4);
    }

    SpectralCard spectral;
    FxCard fx;
    juce::OwnedArray<SlotCard> slots;
    juce::TextButton sourceBtn { "SOURCE" }, fxBtn { "FX & MASTER" };
    juce::TextButton bankA { "02-04" }, bankB { "05-07" },
                     bankC { "08-10" }, bankD { "11" };
    int activeBank = 0, topPage = 0;
};
} // namespace ui
