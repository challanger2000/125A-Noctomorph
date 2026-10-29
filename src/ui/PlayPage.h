#pragma once
#include "MorphPage.h"

// Compact default performance surface for the *unchanged* V4JB DSP.
// All visible controls bind directly to existing host/APVTS parameter IDs.
namespace ui
{
struct PlayPage final : juce::Component
{
    explicit PlayPage (V4JBAudioProcessor& p)
        : spectrum (p, 0), morphPad (p),
          tilt (p.apvts, ParamID::spTilt, "TILT"),
          depth (p.apvts, ParamID::spDepth, "DEPTH"),
          motion (p.apvts, ParamID::spModDepth, "MOTION"),
          speed (p.apvts, ParamID::spModRate, "SPEED"),
          attack (p.apvts, ParamID::env (0, "a"), "ATTACK"),
          release (p.apvts, ParamID::env (0, "r"), "RELEASE"),
          space (p.apvts, ParamID::rvMix, "SPACE"),
          width (p.apvts, ParamID::width, "WIDTH"),
          reverbOn (p.apvts, ParamID::rvOn, "REVERB ON")
    {
        addAndMakeVisible (spectrum);
        addAndMakeVisible (morphPad);
        for (auto* k : { &tilt, &depth, &motion, &speed,
                         &attack, &release, &space, &width })
            addAndMakeVisible (*k);
        addAndMakeVisible (reverbOn);
        tilt.slider.setTooltip ("Original spectral tilt, default 1.20.");
        depth.slider.setTooltip ("Spectral shaping depth; algorithm remains available in Advanced.");
        motion.slider.setTooltip ("Original spectral modulation depth.");
        speed.slider.setTooltip ("Original spectral modulation rate.");
        attack.slider.setTooltip ("Amplifier envelope 1 attack.");
        release.slider.setTooltip ("Amplifier envelope 1 release.");
        space.slider.setTooltip ("Reverb mix. Activate REVERB ON to hear the effect.");
        width.slider.setTooltip ("Stereo width, 1.00 is neutral.");
    }

    void refresh()
    {
        spectrum.refresh();
        morphPad.repaint();
    }

    static void panel (juce::Graphics& g, juce::Rectangle<int> r,
                       const juce::String& name)
    {
        const auto box = r.toFloat().reduced (1.f);
        g.setGradientFill (juce::ColourGradient (kDusk.brighter (0.08f),
                                                 box.getX(), box.getY(),
                                                 kNight.brighter (0.10f),
                                                 box.getX(), box.getBottom(), false));
        g.fillRoundedRectangle (box, 8.f);
        g.setColour (kFog.withAlpha (0.22f));
        g.drawRoundedRectangle (box.reduced (0.5f), 8.f, 1.f);
        g.setColour (kAmber);
        g.setFont (boldFont (12.f));
        g.drawText (name, r.reduced (20, 12).removeFromTop (19),
                    juce::Justification::centredLeft);
    }

    void paint (juce::Graphics& g) override
    {
        auto all = getLocalBounds().reduced (20);
        const int upperH = juce::jmin (390, all.getHeight() * 58 / 100);
        auto upper = all.removeFromTop (upperH);
        auto left = upper.removeFromLeft (upper.getWidth() * 57 / 100).reduced (4);
        auto right = upper.reduced (4);
        panel (g, left, "SPECTRAL SOURCE");
        panel (g, right, "MORPH  /  A - B - C - D");

        all.removeFromTop (10);
        panel (g, all.reduced (4, 0), "SOUND SHAPING");

        g.setColour (kFog.withAlpha (0.60f));
        g.setFont (boldFont (11.f));
        auto hint = left.reduced (22, 34).removeFromBottom (25);
        g.drawText ("Live source spectrum · play a MIDI note", hint,
                    juce::Justification::centredLeft);
    }

    void resized() override
    {
        auto all = getLocalBounds().reduced (20);
        const int upperH = juce::jmin (390, all.getHeight() * 58 / 100);
        auto upper = all.removeFromTop (upperH);
        auto left = upper.removeFromLeft (upper.getWidth() * 57 / 100).reduced (4);
        auto right = upper.reduced (4);
        spectrum.setBounds (left.reduced (25, 44).withTrimmedBottom (14));
        morphPad.setBounds (right.reduced (22, 39).withTrimmedBottom (4));

        all.removeFromTop (10);
        auto controls = all.reduced (8, 0);
        controls.removeFromTop (36);
        const int rowH = controls.getHeight() / 2;
        auto first = controls.removeFromTop (rowH);
        auto second = controls.removeFromTop (rowH);
        const auto distribute = [] (juce::Rectangle<int> row,
                                     std::initializer_list<juce::Component*> knobs)
        {
            const int w = row.getWidth() / (int) knobs.size();
            for (auto* k : knobs)
                k->setBounds (row.removeFromLeft (w).reduced (16, 3));
        };
        distribute (first, { &tilt, &depth, &motion, &speed });
        distribute (second, { &attack, &release, &space, &width });
        auto spaceArea = space.getBounds();
        space.setBounds (spaceArea.withTrimmedBottom (24));
        reverbOn.setBounds (spaceArea.removeFromBottom (24).reduced (18, 2));
    }

    SpectrumBars spectrum;
    MorphPad morphPad;
    Knob tilt, depth, motion, speed, attack, release, space, width;
    Toggle reverbOn;
};
} // namespace ui
