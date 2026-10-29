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
        panel (g, right, "MORPH / A - B - C - D");

        all.removeFromTop (10);
        const auto shaping = all.reduced (4, 0);
        panel (g, shaping, "SOUND SHAPING");

        // Four vertical functional pairs, not an anonymous eight-knob grid.
        const char* const titles[] = { "01  TONE", "02  MOVEMENT", "03  ENVELOPE", "04  SPACE" };
        auto groups = shaping.reduced (10, 0).withTrimmedTop (37);
        const int groupW = groups.getWidth() / 4;
        for (int i = 0; i < 4; ++i)
        {
            auto cell = groups.removeFromLeft (groupW).reduced (4, 2);
            auto rect = cell.toFloat();
            g.setColour (kNight.withAlpha (0.35f));
            g.fillRoundedRectangle (rect, 5.f);
            g.setColour (kFog.withAlpha (0.12f));
            g.drawRoundedRectangle (rect.reduced (0.5f), 5.f, 1.f);
            g.setColour (kFog.withAlpha (0.90f));
            g.setFont (boldFont (11.f));
            g.drawText (titles[i], cell.reduced (12, 7).removeFromTop (20),
                        juce::Justification::centredLeft, false);
        }

        g.setColour (kFog.withAlpha (0.67f));
        g.setFont (boldFont (11.f));
        g.drawText ("Live source spectrum - play a MIDI note",
                    left.reduced (22, 34).removeFromBottom (25),
                    juce::Justification::centredLeft, false);
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
        auto groups = all.reduced (14, 0).withTrimmedTop (37);
        const int groupW = groups.getWidth() / 4;
        const std::pair<juce::Component*, juce::Component*> pairings[] = {
            { &tilt, &depth }, { &motion, &speed },
            { &attack, &release }, { &space, &width }
        };

        for (int i = 0; i < 4; ++i)
        {
            auto cell = groups.removeFromLeft (groupW).reduced (4, 2);
            cell.removeFromTop (27);
            const int rowH = cell.getHeight() / 2;
            pairings[i].first->setBounds (cell.removeFromTop (rowH).reduced (14, 0));
            pairings[i].second->setBounds (cell.removeFromTop (rowH).reduced (14, 0));
        }

        // A real reverb switch in its section header (not taking space from the knob).
        const auto canvas = getLocalBounds().reduced (20);
        const int spaceX = canvas.getRight() - juce::jmax (1, (canvas.getWidth() - 28) / 4);
        reverbOn.setBounds (spaceX + 95, all.getY() + 43, 106, 22);
    }

    SpectrumBars spectrum;
    MorphPad morphPad;
    Knob tilt, depth, motion, speed, attack, release, space, width;
    Toggle reverbOn;
};
} // namespace ui
