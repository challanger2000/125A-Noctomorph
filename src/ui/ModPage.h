#pragma once

#include "Theme.h"

// Mod page: the four envelopes (Env 1 = amp) and three LFOs that slots can route.
namespace ui
{
struct EnvCard : public juce::Component
{
    EnvCard (APVTS& s, int i) : index (i)
    {
        const std::pair<const char*, const char*> defs[] = { { "a", "Attack" }, { "d", "Decay" }, { "s", "Sustain" },
                                                             { "r", "Release" }, { "vel", "Velocity" } };
        for (auto& d : defs)
            addAndMakeVisible (knobs.add (new Knob (s, ParamID::env (i, d.first), d.second)));
    }
    void paint (juce::Graphics& g) override
    {
        paintCard (g, getLocalBounds(), "ENV " + juce::String (index + 1) + (index == 0 ? "  (AMP)" : ""));
    }
    void resized() override
    {
        auto r = getLocalBounds().reduced (8, 4);
        r.removeFromTop (20);
        const int w = r.getWidth() / knobs.size();
        for (auto* k : knobs)
            k->setBounds (r.removeFromLeft (w).withSizeKeepingCentre (w, juce::jmin (r.getHeight(), 140)));
    }
    int index;
    juce::OwnedArray<Knob> knobs;
};

struct LfoCard : public juce::Component
{
    LfoCard (APVTS& s, int i)
        : index (i), rate (s, ParamID::lfo (i, "rate"), "Rate"), shape (s, ParamID::lfo (i, "shape")),
          div (s, ParamID::lfo (i, "div")), sync (s, ParamID::lfo (i, "sync"), "Sync")
    {
        addAndMakeVisible (rate);
        addAndMakeVisible (shape);
        addAndMakeVisible (div);
        addAndMakeVisible (sync);
    }
    void paint (juce::Graphics& g) override { paintCard (g, getLocalBounds(), "LFO " + juce::String (index + 1)); }
    void resized() override
    {
        auto r = getLocalBounds().reduced (8, 4);
        r.removeFromTop (20);
        rate.setBounds (r.removeFromLeft (120).withSizeKeepingCentre (120, juce::jmin (r.getHeight(), 120)));
        auto col = r.reduced (8, 0).withSizeKeepingCentre (r.getWidth() - 16, 96);
        shape.setBounds (col.removeFromTop (26).reduced (0, 2));
        col.removeFromTop (4);
        auto line = col.removeFromTop (26);
        sync.setBounds (line.removeFromLeft (54).reduced (0, 2));
        div.setBounds (line.reduced (4, 2));
    }
    int index;
    Knob rate;
    Combo shape, div;
    Toggle sync;
};

struct ModPage : public juce::Component
{
    ModPage (V4JBAudioProcessor& p)
    {
        for (int i = 0; i < v4jb::kNumEnvs; ++i)
            addAndMakeVisible (envs.add (new EnvCard (p.apvts, i)));
        for (int i = 0; i < v4jb::kNumLfos; ++i)
            addAndMakeVisible (lfos.add (new LfoCard (p.apvts, i)));
    }
    void paint (juce::Graphics& g) override
    {
        g.setColour (kAmber);
        g.setFont (boldFont (13.f));
        g.drawText ("ENVELOPES", envTitle, juce::Justification::centredLeft, false);
        g.drawText ("LOW-FREQUENCY OSCILLATORS", lfoTitle, juce::Justification::centredLeft, false);
        g.setColour (kFog.withAlpha (0.65f));
        g.setFont (boldFont (11.f));
        g.drawText ("Route envelopes, LFOs and MIDI from SOUND > module MOD.",
                    hint, juce::Justification::centred, false);
    }
    void resized() override
    {
        auto r = getLocalBounds().reduced (12);
        hint = r.removeFromBottom (26);
        envTitle = r.removeFromTop (28);
        auto envelopeArea = r.removeFromTop (r.getHeight() * 61 / 100);
        const int ew = envelopeArea.getWidth() / 2, eh = envelopeArea.getHeight() / 2;
        for (int i = 0; i < envs.size(); ++i)
            envs[i]->setBounds (juce::Rectangle<int> (envelopeArea.getX() + (i % 2) * ew,
                                                      envelopeArea.getY() + (i / 2) * eh,
                                                      ew, eh).reduced (5));
        lfoTitle = r.removeFromTop (33);
        const int lw = r.getWidth() / lfos.size();
        for (auto* l : lfos)
            l->setBounds (r.removeFromLeft (lw).reduced (5));
    }
    juce::OwnedArray<EnvCard> envs;
    juce::OwnedArray<LfoCard> lfos;
    juce::Rectangle<int> hint, envTitle, lfoTitle;
};
} // namespace ui
