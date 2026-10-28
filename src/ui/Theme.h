#pragma once

#include "../PluginProcessor.h"

#include <JuceHeader.h>

// 125A anthracite GUI foundation. This is a presentation-only change;
// existing APVTS bindings, control values and synthesis are unchanged.
namespace ui
{
inline const juce::Colour kNight { 0xff11161c };  // faceplate / graphite
inline const juce::Colour kDusk  { 0xff242c35 };  // raised anthracite panels
inline const juce::Colour kFog   { 0xffb2c0cb };  // cool silver legibility
inline const juce::Colour kTeal  { 0xff6bb8dc };  // restrained 125A blue
inline const juce::Colour kAmber { 0xffd9e2eb }; // silver labels, no yellow accent
inline const juce::Colour kWhite { 0xfff4f7fa };

inline juce::Font boldFont (float h) { return juce::Font (juce::FontOptions (h).withStyle ("Bold")); }

struct PanelLnF : public juce::LookAndFeel_V4
{
    PanelLnF()
    {
        setColour (juce::ComboBox::backgroundColourId, kNight);
        setColour (juce::ComboBox::textColourId, kFog);
        setColour (juce::ComboBox::outlineColourId, kFog.withAlpha (0.3f));
        setColour (juce::ComboBox::arrowColourId, kTeal);
        setColour (juce::PopupMenu::backgroundColourId, kNight);
        setColour (juce::PopupMenu::textColourId, kFog);
        setColour (juce::PopupMenu::highlightedBackgroundColourId, kDusk.brighter (0.2f));
        setColour (juce::PopupMenu::highlightedTextColourId, kWhite);
        setColour (juce::PopupMenu::headerTextColourId, kAmber);
        setColour (juce::Slider::trackColourId, kTeal);
        setColour (juce::Slider::backgroundColourId, kNight);
        setColour (juce::Slider::thumbColourId, kFog);
        setColour (juce::Slider::textBoxTextColourId, kFog);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour (juce::Label::textColourId, kFog);
        setColour (juce::TabbedButtonBar::tabTextColourId, kFog);
        setColour (juce::TabbedButtonBar::frontTextColourId, kWhite);
        setColour (juce::TextButton::buttonColourId, kDusk);
        setColour (juce::TextButton::buttonOnColourId, kTeal.darker (0.8f));
        setColour (juce::TextButton::textColourOffId, kWhite);
        setColour (juce::TextButton::textColourOnId, kWhite);
        setColour (juce::AlertWindow::backgroundColourId, kNight);
        setColour (juce::AlertWindow::textColourId, kFog);
        setColour (juce::TextEditor::backgroundColourId, kDusk);
        setColour (juce::TextEditor::textColourId, kFog);
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos,
                           float startAngle, float endAngle, juce::Slider&) override
    {
        auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) w, (float) h).reduced (3.f);
        const float d = juce::jmin (bounds.getWidth(), bounds.getHeight());
        auto rc = bounds.withSizeKeepingCentre (d, d);
        const auto c = rc.getCentre();
        const float angle = startAngle + pos * (endAngle - startAngle);

        g.setColour (kDusk.brighter (0.15f));
        g.fillEllipse (rc);

        // Slim calibrated value arc; bright cap with a subdued metal gradient.
        juce::Path arc;
        arc.addCentredArc (c.x, c.y, d * 0.44f, d * 0.44f, 0.f, startAngle, angle, true);
        g.setColour (kTeal);
        g.strokePath (arc, juce::PathStrokeType (2.3f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        auto cap = rc.reduced (d * 0.18f);
        g.setGradientFill (juce::ColourGradient (kFog.brighter (0.2f), cap.getX(), cap.getY(),
                                                kDusk.brighter (0.25f), cap.getRight(), cap.getBottom(), false));
        g.fillEllipse (cap);
        g.setColour (kNight.withAlpha (0.65f));
        g.drawEllipse (cap, 0.9f);
        const auto tip = c.getPointOnCircumference (cap.getWidth() * 0.42f, angle);
        g.setColour (kNight);
        g.drawLine (c.x, c.y, tip.x, tip.y, 2.2f);
    }

    // Thin-rimmed metal buttons. Active state is unmistakable without neon glow.
    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour&, bool over, bool down) override
    {
        auto r = button.getLocalBounds().toFloat().reduced (1.5f);
        const auto base = button.getToggleState() ? kDusk.brighter (0.40f)
                         : down ? kNight : over ? kDusk.brighter (0.23f) : kDusk;
        g.setGradientFill (juce::ColourGradient (base.brighter (0.15f), r.getX(), r.getY(),
                                                 base.darker (0.2f), r.getX(), r.getBottom(), false));
        g.fillRoundedRectangle (r, 3.f);
        g.setColour (button.getToggleState() ? kTeal.withAlpha (0.8f) : kFog.withAlpha (0.22f));
        g.drawRoundedRectangle (r.reduced (0.5f), 3.f, 1.f);
    }

    // Toggles look like buttons; their on state is steel blue.
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& b, bool over, bool) override
    {
        auto bounds = b.getLocalBounds().toFloat().reduced (2.f);
        const bool on = b.getToggleState();
        g.setColour (on ? kTeal.darker (0.55f) : (over ? kDusk.brighter (0.25f) : kDusk.brighter (0.1f)));
        g.fillRoundedRectangle (bounds, 3.f);
        g.setColour (on ? kTeal.withAlpha (0.8f) : kFog.withAlpha (0.22f));
        g.drawRoundedRectangle (bounds.reduced (0.5f), 3.f, 1.f);
        g.setFont (boldFont (11.f));
        g.setColour (on ? kWhite : kFog);
        g.drawText (b.getButtonText(), bounds.toNearestInt(), juce::Justification::centred, false);
    }

    juce::Font getLabelFont (juce::Label&) override { return boldFont (11.f); }
    juce::Font getComboBoxFont (juce::ComboBox&) override { return boldFont (11.f); }
    juce::Font getPopupMenuFont() override { return boldFont (12.f); }
    juce::Font getTextButtonFont (juce::TextButton&, int) override { return boldFont (11.f); }
};

using APVTS = juce::AudioProcessorValueTreeState;

// Rotary knob with a name above and the value below.
struct Knob : public juce::Component
{
    Knob (APVTS& st, const juce::String& id, const juce::String& name)
    {
        label.setText (name, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setInterceptsMouseClicks (false, false);
        addAndMakeVisible (label);
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 64, 12);
        // Set here, not only in PanelLnF: the text box is built before the editor installs the LnF.
        slider.setColour (juce::Slider::textBoxTextColourId, kFog);
        slider.setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        slider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        addAndMakeVisible (slider);
        att = std::make_unique<APVTS::SliderAttachment> (st, id, slider);
    }
    void resized() override
    {
        auto r = getLocalBounds();
        label.setBounds (r.removeFromTop (12));
        slider.setBounds (r);
    }
    juce::Label label;
    juce::Slider slider;
    std::unique_ptr<APVTS::SliderAttachment> att;
};

// Horizontal slider with a name on the left. Used for module params and route amounts.
struct HSlider : public juce::Component
{
    HSlider (APVTS& st, const juce::String& id, const juce::String& name)
    {
        label.setText (name, juce::dontSendNotification);
        label.setFont (boldFont (10.f));
        addAndMakeVisible (label);
        slider.setSliderStyle (juce::Slider::LinearBar);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        slider.setColour (juce::Slider::trackColourId, kTeal.withAlpha (0.6f));
        slider.setColour (juce::Slider::backgroundColourId, kNight);
        slider.setPopupDisplayEnabled (true, false, nullptr);
        addAndMakeVisible (slider);
        att = std::make_unique<APVTS::SliderAttachment> (st, id, slider);
    }
    void resized() override
    {
        auto r = getLocalBounds();
        label.setBounds (r.removeFromLeft (labelW));
        slider.setBounds (r.reduced (0, 3));
    }
    int labelW = 62;
    juce::Label label;
    juce::Slider slider;
    std::unique_ptr<APVTS::SliderAttachment> att;
};

// ComboBox filled from the choice parameter's own item list.
struct Combo : public juce::ComboBox
{
    Combo (APVTS& st, const juce::String& id)
    {
        if (auto* c = dynamic_cast<juce::AudioParameterChoice*> (st.getParameter (id)))
            addItemList (c->choices, 1);
        att = std::make_unique<APVTS::ComboBoxAttachment> (st, id, *this);
    }
    std::unique_ptr<APVTS::ComboBoxAttachment> att;
};

struct Toggle : public juce::ToggleButton
{
    Toggle (APVTS& st, const juce::String& id, const juce::String& text) : juce::ToggleButton (text)
    {
        att = std::make_unique<APVTS::ButtonAttachment> (st, id, *this);
    }
    std::unique_ptr<APVTS::ButtonAttachment> att;
};

// Bar graph of the first 64 partial levels after one stage, in dB (-60..0).
// refresh() is called by the editor's timer; bars fall back slowly so they read well.
struct SpectrumBars : public juce::Component
{
    SpectrumBars (V4JBAudioProcessor& p, int stageIndex) : proc (p), stage (stageIndex) {}

    void refresh()
    {
        bool changed = false;
        for (int b = 0; b < v4jb::kBars; ++b)
        {
            const float a = proc.tap (stage, b);
            const float h = a > 1.e-6f ? juce::jlimit (0.f, 1.f, (20.f * std::log10 (a) + 60.f) / 60.f) : 0.f;
            const float next = juce::jmax (h, shown[b] - 0.04f);
            changed = changed || std::abs (next - shown[b]) > 1.e-3f;
            shown[b] = next;
        }
        if (changed)
            repaint();
    }

    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();
        g.setColour (kNight);
        g.fillRoundedRectangle (r, 3.f);
        const float bw = r.getWidth() / (float) v4jb::kBars;
        g.setColour (kTeal.withAlpha (0.85f));
        for (int b = 0; b < v4jb::kBars; ++b)
        {
            const float h = shown[b] * (r.getHeight() - 2.f);
            g.fillRect (r.getX() + (float) b * bw, r.getBottom() - 1.f - h, juce::jmax (1.f, bw - 1.f), h);
        }
    }

    V4JBAudioProcessor& proc;
    int stage;
    float shown[v4jb::kBars] {};
};

// Rounded card with a title in the top-left; children lay out below kTitleH.
inline void paintCard (juce::Graphics& g, juce::Rectangle<int> r, const juce::String& title)
{
    auto panel = r.toFloat().reduced (1.f);
    g.setGradientFill (juce::ColourGradient (kDusk.brighter (0.08f), panel.getX(), panel.getY(),
                                             kDusk.darker (0.19f), panel.getX(), panel.getBottom(), false));
    g.fillRoundedRectangle (panel, 5.f);
    g.setColour (kFog.withAlpha (0.23f));
    g.drawRoundedRectangle (panel.reduced (0.5f), 5.f, 1.f);
    auto bar = r.reduced (10, 5).removeFromTop (17);
    g.setColour (kAmber);
    g.setFont (boldFont (11.f));
    g.drawText (title, bar, juce::Justification::centredLeft, false);
    g.setColour (kTeal.withAlpha (0.28f));
    g.drawLine ((float) r.getX() + 10.f, (float) r.getY() + 25.f,
                (float) r.getRight() - 10.f, (float) r.getY() + 25.f, 0.8f);
}

// Lays out components left to right in equal cells.
inline void row (juce::Rectangle<int> r, std::initializer_list<juce::Component*> items)
{
    if (items.size() == 0)
        return;
    const int w = r.getWidth() / (int) items.size();
    for (auto* c : items)
        c->setBounds (r.removeFromLeft (w).reduced (2, 0));
}
} // namespace ui
