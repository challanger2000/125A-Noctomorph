#include "PluginEditor.h"
#include "ui/VersionCompare.h"

#include <thread>
#include <vector>

using namespace ui;

namespace
{
constexpr int kEditorW = 1220;
constexpr int kEditorH = 800;
constexpr int kPad = 8;
constexpr int kTopH = 64;
constexpr int kKeyboardH = 72;
constexpr int kKeyLow = 36;   // C2
constexpr int kKeyHigh = 84;  // C6
constexpr int kTitleHitW = 244;

int whiteKeysInRange (int low, int high)
{
    int n = 0;
    for (int note = low; note <= high; ++note)
        if (! juce::MidiMessage::isMidiNoteBlack (note))
            ++n;
    return juce::jmax (1, n);
}
} // namespace

// ---------------------------------------------------------------- About

V4JBAudioProcessorEditor::AboutBox::AboutBox()
{
    status.setJustificationType (juce::Justification::centred);
    status.setColour (juce::Label::textColourId, kFog);
    status.setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    addAndMakeVisible (status);

    // ToggleButton is only used for the LnF look here, so it must not latch.
    checkBtn.setClickingTogglesState (false);
    closeBtn.setClickingTogglesState (false);
    checkBtn.onClick = [this] { checkForUpdate(); };
    closeBtn.onClick = [this] { setVisible (false); };
    addAndMakeVisible (checkBtn);
    addAndMakeVisible (closeBtn);

    releasesBtn.setColour (juce::HyperlinkButton::textColourId, kAmber);
    releasesBtn.setVisible (false);
    addAndMakeVisible (releasesBtn);
}

void V4JBAudioProcessorEditor::AboutBox::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black.withAlpha (0.55f));
    auto card = cardBounds().toFloat();
    g.setColour (kNight);
    g.fillRoundedRectangle (card, 6.f);
    g.setColour (kFog.withAlpha (0.35f));
    g.drawRoundedRectangle (card.reduced (0.5f), 6.f, 1.f);

    auto r = cardBounds().reduced (16, 14);
    g.setColour (kFog);
    g.setFont (boldFont (20.f));
    g.drawText ("V4JB", r.removeFromTop (26), juce::Justification::centred, false);
    g.setFont (boldFont (12.f));
    g.drawText ("v" JucePlugin_VersionString, r.removeFromTop (18), juce::Justification::centred, false);
    g.setFont (boldFont (11.f));
    g.setColour (kFog.withAlpha (0.8f));
    g.drawText ("Stupid Systems LLC", r.removeFromTop (16), juce::Justification::centred, false);
    g.drawText ("MIT", r.removeFromTop (16), juce::Justification::centred, false);
}

void V4JBAudioProcessorEditor::AboutBox::resized()
{
    auto r = cardBounds().reduced (16, 14);
    r.removeFromTop (26 + 18 + 16 + 16 + 8);
    checkBtn.setBounds (r.removeFromTop (22).withSizeKeepingCentre (160, 22));
    r.removeFromTop (6);
    status.setBounds (r.removeFromTop (22));
    r.removeFromTop (4);
    releasesBtn.setBounds (r.removeFromTop (20).withSizeKeepingCentre (140, 20));
    closeBtn.setBounds (cardBounds().removeFromBottom (36).reduced (90, 8));
}

void V4JBAudioProcessorEditor::AboutBox::mouseUp (const juce::MouseEvent& e)
{
    if (e.mouseWasClicked() && ! cardBounds().contains (e.getPosition()))
        setVisible (false);
}

juce::Rectangle<int> V4JBAudioProcessorEditor::AboutBox::cardBounds() const
{
    return getLocalBounds().withSizeKeepingCentre (320, 240);
}

void V4JBAudioProcessorEditor::AboutBox::applyResult (const juce::String& text, bool hasUpdate)
{
    fetching.store (false);
    status.setText (text, juce::dontSendNotification);
    releasesBtn.setVisible (hasUpdate);
}

void V4JBAudioProcessorEditor::AboutBox::checkForUpdate()
{
    if (fetching.exchange (true))
        return;
    status.setText ("Checking...", juce::dontSendNotification);
    releasesBtn.setVisible (false);
    // Network on a detached thread; the result hops back to the message thread
    // and is dropped if the editor closed meanwhile (SafePointer goes null).
    juce::Component::SafePointer<AboutBox> safe (this);
    std::thread ([safe]
    {
        juce::String text;
        bool hasUpdate = false;
        juce::URL url ("https://api.github.com/repos/iamtew/V4JB/tags");
        const auto opts = juce::URL::InputStreamOptions (juce::URL::ParameterHandling::inAddress)
                              .withExtraHeaders ("User-Agent: V4JB\nAccept: application/vnd.github+json")
                              .withConnectionTimeoutMs (8000);
        if (auto stream = url.createInputStream (opts))
        {
            const auto parsed = juce::JSON::parse (stream->readEntireStreamAsString());
            if (auto* arr = parsed.getArray())
            {
                std::vector<std::string> names;
                names.reserve ((size_t) arr->size());
                for (const auto& v : *arr)
                    if (auto* obj = v.getDynamicObject())
                        names.push_back (obj->getProperty ("name").toString().toStdString());
                const auto newest = v4jb::newestNewerTag (names, JucePlugin_VersionString);
                if (newest.empty())
                    text = "You are up to date.";
                else
                {
                    text = "Newer version available: " + juce::String (newest);
                    hasUpdate = true;
                }
            }
            else
                text = "Could not reach GitHub.";
        }
        else
            text = "Could not reach GitHub.";

        juce::MessageManager::callAsync ([safe, text, hasUpdate]
        {
            if (safe != nullptr)
                safe->applyResult (text, hasUpdate);
        });
    }).detach();
}

// ---------------------------------------------------------------- Editor

V4JBAudioProcessorEditor::V4JBAudioProcessorEditor (V4JBAudioProcessor& p)
    : juce::AudioProcessorEditor (p), proc (p)
{
    setLookAndFeel (&panelLnF);
    setOpaque (true);
    // Master 125A logo kept as the exact source SVG, not a reconstructed font mark.
    logo = juce::Drawable::createFromSVGString (
        juce::String::fromUTF8 (BinaryData::logo_svg, BinaryData::logo_svgSize));

    presetBtn.onClick = [this] { showPresetMenu(); };
    presetBtn.setTooltip ("Presets");
    prevBtn.onClick = [this] { stepPreset (-1); };
    nextBtn.onClick = [this] { stepPreset (1); };
    saveBtn.onClick = [this] { savePresetDialog(); };
    randBtn.onClick = [this] { showRandomMenu(); };
    randBtn.setTooltip ("Generate a sound or vary the current module configuration");
    economy.setTooltip ("Economy: at most 128 partials per voice, much lighter on CPU");
    for (juce::Component* c : { (juce::Component*) &presetBtn, (juce::Component*) &prevBtn, (juce::Component*) &nextBtn,
                                (juce::Component*) &saveBtn, (juce::Component*) &randBtn, (juce::Component*) &economy,
                                (juce::Component*) &poly, (juce::Component*) &octave, (juce::Component*) &pitch,
                                (juce::Component*) &glide, (juce::Component*) &volume })
        content.addAndMakeVisible (c);

    const auto tabColour = kNight.brighter (0.05f);
    tabs.addTab ("SOUND", tabColour, &editPage, false);
    tabs.addTab ("MODULATION", tabColour, &modPage, false);
    tabs.addTab ("MORPH", tabColour, &morphPage, false);
    tabs.addTab ("WAVES", tabColour, &wavePage, false);
    tabs.setTabBarDepth (28);
    tabs.setOutline (0);
    content.addAndMakeVisible (tabs);
    content.addChildComponent (aboutBox);

    // VST3 stays keyboard-free. The host already has keys; this strip is for Standalone.
    const auto captureDir = juce::SystemStats::getEnvironmentVariable ("V4JB_CAPTURE_GUI_DIR", {});
    // Test mode renders the VST3-sized editor without the Standalone-only keys.
    if (proc.wrapperType == juce::AudioProcessor::wrapperType_Standalone && captureDir.isEmpty())
    {
        keyboard = std::make_unique<juce::MidiKeyboardComponent> (
            keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard);
        keyboard->setAvailableRange (kKeyLow, kKeyHigh);
        keyboard->setScrollButtonsVisible (false);
        // Mouse only. The component's QWERTY map stays off while it never takes focus.
        keyboard->setWantsKeyboardFocus (false);
        keyboard->setMouseClickGrabsKeyboardFocus (false);
        keyboard->setColour (juce::MidiKeyboardComponent::whiteNoteColourId, kFog);
        keyboard->setColour (juce::MidiKeyboardComponent::blackNoteColourId, kNight);
        keyboard->setColour (juce::MidiKeyboardComponent::keySeparatorLineColourId, kDusk);
        keyboard->setColour (juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, kTeal.withAlpha (0.35f));
        keyboard->setColour (juce::MidiKeyboardComponent::keyDownOverlayColourId, kTeal.withAlpha (0.65f));
        keyboard->setColour (juce::MidiKeyboardComponent::textLabelColourId, kNight);
        keyboard->setColour (juce::MidiKeyboardComponent::shadowColourId, juce::Colours::transparentBlack);
        keyboardState.addListener (this);
        content.addAndMakeVisible (*keyboard);
    }

    baseW = kEditorW;
    baseH = kEditorH + (keyboard != nullptr ? kKeyboardH : 0);
    content.setSize (baseW, baseH);
    addAndMakeVisible (content);

    // Drag the corner to scale (40%..150%), aspect ratio locked. The scale is saved with the session.
    // Read it first: setResizeLimits clamps the still-empty editor and resized() overwrites uiScale.
    const float s = juce::jlimit (0.4f, 1.5f, proc.uiScale);
    setResizable (true, true);
    setResizeLimits (baseW * 2 / 5, baseH * 2 / 5, baseW * 3 / 2, baseH * 3 / 2);
    getConstrainer()->setFixedAspectRatio ((double) baseW / (double) baseH);
    setSize (juce::roundToInt ((float) baseW * (captureDir.isNotEmpty() ? 1.f : s)),
             juce::roundToInt ((float) baseH * (captureDir.isNotEmpty() ? 1.f : s)));
    startTimerHz (30);
}

V4JBAudioProcessorEditor::~V4JBAudioProcessorEditor()
{
    stopTimer();
    if (keyboard != nullptr)
    {
        keyboardState.allNotesOff (1);  // note-offs reach the engine before the keys are destroyed
        keyboardState.removeListener (this);
    }
    setLookAndFeel (nullptr);
}

void V4JBAudioProcessorEditor::handleNoteOn (juce::MidiKeyboardState*, int, int note, float)
{
    proc.pushKey (note, true);
}

void V4JBAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState*, int, int note, float)
{
    proc.pushKey (note, false);
}

// 30 Hz: only the visible page pulls spectra and meters.
void V4JBAudioProcessorEditor::timerCallback()
{
    if (proc.currentPresetCategory.isNotEmpty())
        presetBtn.setButtonText (proc.currentPresetCategory + "  ·  " + proc.currentPresetName);
    else
        presetBtn.setButtonText (proc.currentPresetName);
    presetBtn.setTooltip (proc.currentPresetCategory.isNotEmpty()
                              ? proc.currentPresetCategory + " / " + proc.currentPresetName
                              : proc.currentPresetName);
    switch (tabs.getCurrentTabIndex())
    {
        case 0: editPage.refresh(); break;
        case 2: morphPage.refresh(); break;
        case 3: wavePage.refresh(); break;
        default: break;
    }

    const auto outDir = juce::SystemStats::getEnvironmentVariable ("V4JB_CAPTURE_GUI_DIR", {});
    if (outDir.isNotEmpty() && ciCaptureStep < 4)
    {
        static const char* const names[] = { "01-sound-modules-01-05.png",
                                            "02-sound-modules-06-10.png",
                                            "03-morph.png", "04-modulation.png" };
        const auto folder = juce::File (outDir);
        folder.createDirectory();
        const auto pngFile = folder.getChildFile (names[ciCaptureStep]);
        const auto pendingFile = folder.getChildFile (juce::String (names[ciCaptureStep]) + ".partial");
        juce::PNGImageFormat png;
        const auto image = createComponentSnapshot (getLocalBounds(), true);
        if (image.isValid())
        {
            if (auto stream = pendingFile.createOutputStream())
            {
                const bool written = png.writeImageToStream (image, *stream);
                stream->flush();
                stream.reset(); // close before making the final .png visible to CI
                if (written)
                    pendingFile.moveFileTo (pngFile);
            }
        }
        ++ciCaptureStep;
        if (ciCaptureStep == 1) editPage.setBank (1);
        else if (ciCaptureStep == 2) tabs.setCurrentTabIndex (2);
        else if (ciCaptureStep == 3) tabs.setCurrentTabIndex (1);
    }
}

void V4JBAudioProcessorEditor::refreshPresetList()
{
    presetList = proc.getFactoryPresets();
    presetList.addArray (Presets::user());
}

void V4JBAudioProcessorEditor::showPresetMenu()
{
    refreshPresetList();

    // Group by category, factory first; menu ids are presetList index + 1.
    juce::PopupMenu menu;
    juce::StringArray categories;
    for (auto& e : presetList)
        categories.addIfNotAlreadyThere (e.category);
    for (auto& cat : categories)
    {
        juce::PopupMenu sub;
        for (int i = 0; i < presetList.size(); ++i)
            if (presetList[i].category == cat)
                sub.addItem (i + 1, presetList[i].name, true, presetList[i].name == proc.currentPresetName);
        menu.addSubMenu (cat, sub);
    }
    menu.addSeparator();
    menu.addItem (-1, "Save preset...");
    menu.addItem (-2, "Open user preset folder");

    juce::Component::SafePointer<V4JBAudioProcessorEditor> safe (this);
    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&presetBtn), [safe] (int id)
    {
        if (safe == nullptr || id == 0)
            return;
        if (id == -1)
            safe->savePresetDialog();
        else if (id == -2)
        {
            Presets::userDir().createDirectory();
            Presets::userDir().startAsProcess();
        }
        else if (juce::isPositiveAndBelow (id - 1, safe->presetList.size()))
        {
            safe->presetIndex = id - 1;
            safe->proc.loadPreset (safe->presetList[id - 1]);
        }
    });
}

void V4JBAudioProcessorEditor::stepPreset (int delta)
{
    refreshPresetList();
    if (presetList.isEmpty())
        return;

    juce::Array<int> group;
    for (int i = 0; i < presetList.size(); ++i)
        if (presetList[i].category == proc.currentPresetCategory)
            group.add (i);
    if (group.isEmpty())  // Random / unknown name: walk the whole list
        for (int i = 0; i < presetList.size(); ++i)
            group.add (i);

    int pos = 0;
    for (int g = 0; g < group.size(); ++g)
        if (presetList[group[g]].name == proc.currentPresetName)
        {
            pos = g;
            break;
        }
    pos = (pos + delta + group.size()) % group.size();
    presetIndex = group[pos];
    proc.loadPreset (presetList[presetIndex]);
}

void V4JBAudioProcessorEditor::savePresetDialog()
{
    auto* w = new juce::AlertWindow ("Save preset", "Saved to Documents/V4JB/Presets", juce::MessageBoxIconType::NoIcon);
    w->addTextEditor ("name", proc.currentPresetName, "Name");
    w->addTextEditor ("category", "User", "Category");
    w->addButton ("Save", 1, juce::KeyPress (juce::KeyPress::returnKey));
    w->addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey));
    juce::Component::SafePointer<V4JBAudioProcessorEditor> safe (this);
    // deleteWhenDismissed: the window is deleted after this callback runs.
    w->enterModalState (true, juce::ModalCallbackFunction::create ([safe, w] (int result)
    {
        if (safe != nullptr && result == 1)
            safe->proc.saveUserPreset (w->getTextEditorContents ("name"), w->getTextEditorContents ("category"));
    }), true);
}

void V4JBAudioProcessorEditor::showRandomMenu()
{
    juce::PopupMenu menu;
    menu.addItem (1, "New sound (new module chain)");
    menu.addItem (2, "Vary values (keep modules)");
    juce::Component::SafePointer<V4JBAudioProcessorEditor> safe (this);
    menu.showMenuAsync (juce::PopupMenu::Options().withTargetComponent (&randBtn), [safe] (int id)
    {
        if (safe != nullptr && id > 0)
            safe->proc.randomize (id == 1);
    });
}

void V4JBAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (kNight);  // covers the sub-pixel edge the scaled content may leave
}

void V4JBAudioProcessorEditor::resized()
{
    if (baseW == 0)
        return;  // called before the constructor finished
    const float s = (float) getWidth() / (float) baseW;
    content.setTransform (juce::AffineTransform::scale (s));
    proc.uiScale = s;
}

void V4JBAudioProcessorEditor::paintContent (juce::Graphics& g)
{
    g.fillAll (kNight);

    const auto header = content.getLocalBounds().removeFromTop (kTopH).toFloat();
    g.setGradientFill (juce::ColourGradient (kDusk.brighter (0.09f), 0.f, 0.f,
                                             kNight.brighter (0.07f), 0.f, header.getBottom(), false));
    g.fillRect (header);
    g.setColour (kFog.withAlpha (0.21f));
    g.drawLine (8.f, header.getBottom() - 1.f, header.getRight() - 8.f,
                header.getBottom() - 1.f, 1.f);

    auto title = content.getLocalBounds().reduced (kPad).removeFromTop (kTopH - kPad);
    if (logo != nullptr)
    {
        auto logoR = title.removeFromLeft (100).reduced (2);
        logo->drawWithin (g, logoR.toFloat(), juce::RectanglePlacement::centred, 1.f);
        title.removeFromLeft (8);
    }
    auto nameCol = title.removeFromLeft (124).withSizeKeepingCentre (124, 42);
    g.setColour (kWhite);
    g.setFont (boldFont (17.f));
    g.drawText ("SPECTRAL", nameCol.removeFromTop (24), juce::Justification::centredLeft, false);
    g.setFont (boldFont (9.f));
    g.setColour (kFog.withAlpha (0.85f));
    g.drawText ("V4JB ENGINE  /  " JucePlugin_VersionString, nameCol, juce::Justification::centredLeft, false);

    g.setColour (kFog.withAlpha (0.30f));
    g.drawRoundedRectangle (content.getLocalBounds().toFloat().reduced (1.5f), 3.f, 1.f);
}

void V4JBAudioProcessorEditor::layoutContent()
{
    auto r = content.getLocalBounds().reduced (kPad);
    auto top = r.removeFromTop (kTopH - kPad);
    top.removeFromLeft (100 + 8 + 124 + 10);

    volume.setBounds (top.removeFromRight (70));
    for (auto* k : { &glide, &pitch, &octave, &poly })
        k->setBounds (top.removeFromRight (62));
    economy.setBounds (top.removeFromRight (84).withSizeKeepingCentre (76, 26));

    auto presets = top.withSizeKeepingCentre (top.getWidth(), 28);
    prevBtn.setBounds (presets.removeFromLeft (28).reduced (1));
    presetBtn.setBounds (presets.removeFromLeft (260).reduced (1));
    nextBtn.setBounds (presets.removeFromLeft (28).reduced (1));
    presets.removeFromLeft (8);
    saveBtn.setBounds (presets.removeFromLeft (60).reduced (1));
    presets.removeFromLeft (8);
    randBtn.setBounds (presets.removeFromLeft (100).reduced (1));

    if (keyboard != nullptr)
    {
        auto keys = r.removeFromBottom (kKeyboardH);
        keyboard->setBounds (keys);
        keyboard->setKeyWidth ((float) keys.getWidth() / (float) whiteKeysInRange (kKeyLow, kKeyHigh));
        r.removeFromBottom (kPad);
    }
    tabs.setBounds (r);
    aboutBox.setBounds (content.getLocalBounds());
}

juce::Rectangle<int> V4JBAudioProcessorEditor::titleHitBounds() const
{
    return { kPad, kPad, kTitleHitW, kTopH - kPad };
}

void V4JBAudioProcessorEditor::contentMouseUp (const juce::MouseEvent& e)
{
    if (e.mouseWasClicked() && titleHitBounds().contains (e.getPosition()))
    {
        aboutBox.setVisible (true);
        aboutBox.toFront (true);
    }
}
