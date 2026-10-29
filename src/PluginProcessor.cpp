#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WaveAnalyzer.h"
#include "dsp/TransportSafety.h"
#include "WaveState.h"

using namespace v4jb;

namespace
{
inline float ld (const std::atomic<float>* a) { return a->load (std::memory_order_relaxed); }
inline int ldi (const std::atomic<float>* a) { return (int) std::lround (ld (a)); }
inline bool ldb (const std::atomic<float>* a) { return ld (a) > 0.5f; }

bool validMorph (const juce::ValueTree& t)
{
    auto finiteUnit = [] (const juce::String& text)
    {
        const auto start = text.getCharPointer();
        auto end = start;
        const auto value = juce::CharacterFunctions::readDoubleValue (end);
        return end != start && end.isEmpty() && std::isfinite (value) && value >= 0 && value <= 1;
    };
    for (int c = 0; c < 4; ++c)
    {
        const auto key = "c" + juce::String (c);
        if (! t.hasProperty (key)) continue;
        const auto vals = juce::StringArray::fromTokens (t[key].toString(), " ", "");
        if (vals.size() != kNumMacros) return false;
        for (auto& v : vals) if (! finiteUnit (v)) return false;
    }
    auto nodes = juce::StringArray::fromTokens (t["nodes"].toString(), ";", "");
    nodes.removeEmptyStrings();
    if (nodes.size() > kMaxNodes) return false;
    for (auto& node : nodes)
    {
        const auto vals = juce::StringArray::fromTokens (node, " ", "");
        if (vals.size() != 3 || ! finiteUnit (vals[0]) || ! finiteUnit (vals[1])
            || vals[2].length() != 1 || vals[2][0] < '0' || vals[2][0] >= '0' + kNumSegLens) return false;
    }
    return true;
}

juce::String joinFloats (const float* v, int n)
{
    juce::StringArray s;
    for (int i = 0; i < n; ++i)
        s.add (juce::String (v[i], 9, true));
    return s.joinIntoString (" ");
}
} // namespace

V4JBAudioProcessor::V4JBAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMS", createParameterLayout())
{
    auto P = [this] (const juce::String& id)
    {
        auto* a = apvts.getRawParameterValue (id);
        jassert (a != nullptr);  // id typo: every id here must exist in ParameterLayout.h
        return a;
    };
    namespace I = ParamID;
    ptr.level = P (I::level); ptr.pitch = P (I::pitch); ptr.octave = P (I::octave); ptr.glide = P (I::glide);
    ptr.poly = P (I::poly); ptr.economy = P (I::economy); ptr.spread = P (I::spread); ptr.width = P (I::width);
    ptr.spAlgo = P (I::spAlgo); ptr.spDepth = P (I::spDepth); ptr.spTilt = P (I::spTilt);
    ptr.spPartials = P (I::spPartials); ptr.spModDepth = P (I::spModDepth); ptr.spModRate = P (I::spModRate);
    ptr.nzMix = P (I::nzMix); ptr.nzWidth = P (I::nzWidth); ptr.nzLow = P (I::nzLow); ptr.nzHigh = P (I::nzHigh);
    ptr.nzDecay = P (I::nzDecay); ptr.sub1 = P (I::sub1); ptr.sub2 = P (I::sub2);
    for (int i = 0; i < kNumSlots; ++i)
    {
        auto& s = ptr.slot[i];
        s.type = P (I::slot (i, "type")); s.main = P (I::slot (i, "main"));
        for (int k = 0; k < 4; ++k)
            s.p[k] = P (I::slotP (i, k));
        s.es = P (I::slot (i, "es")); s.ea = P (I::slot (i, "ea"));
        s.ls = P (I::slot (i, "ls")); s.la = P (I::slot (i, "la"));
        s.ms = P (I::slot (i, "ms")); s.ma = P (I::slot (i, "ma"));
    }
    static constexpr const char* envKeys[5] = { "a", "d", "s", "r", "vel" };
    for (int i = 0; i < kNumEnvs; ++i)
        for (int k = 0; k < 5; ++k)
            ptr.env[i][k] = P (I::env (i, envKeys[k]));
    static constexpr const char* lfoKeys[4] = { "rate", "sync", "div", "shape" };
    for (int i = 0; i < kNumLfos; ++i)
        for (int k = 0; k < 4; ++k)
            ptr.lfo[i][k] = P (I::lfo (i, lfoKeys[k]));
    static constexpr const char* waveKeys[3] = { "start", "end", "speed" };
    for (int w = 0; w < 2; ++w)
        for (int k = 0; k < 3; ++k)
            ptr.wave[w][k] = P (I::wave (w, waveKeys[k]));
    ptr.waveBal = P (I::waBalance);
    ptr.drOn = P (I::drOn); ptr.drAmt = P (I::drAmt); ptr.drMix = P (I::drMix);
    ptr.mdOn = P (I::mdOn); ptr.mdType = P (I::mdType); ptr.mdRate = P (I::mdRate);
    ptr.mdDepth = P (I::mdDepth); ptr.mdMix = P (I::mdMix);
    ptr.dlOn = P (I::dlOn); ptr.dlSync = P (I::dlSync); ptr.dlTime = P (I::dlTime); ptr.dlDiv = P (I::dlDiv);
    ptr.dlFb = P (I::dlFb); ptr.dlMix = P (I::dlMix);
    ptr.rvOn = P (I::rvOn); ptr.rvSize = P (I::rvSize); ptr.rvDamp = P (I::rvDamp); ptr.rvMix = P (I::rvMix);
    for (int m = 0; m < kNumMacros; ++m)
    {
        ptr.macro[m] = P (I::macro (m));
        ptr.lock[m] = P (I::lock (m));
    }
    ptr.mx = P (I::morphX); ptr.my = P (I::morphY); ptr.mmode = P (I::morphMode);
    ptr.mtime = P (I::morphTime); ptr.msnap = P (I::morphSnap); ptr.muser = P (I::userSlot);

    startTimer (500);  // frees retired wave data
}

V4JBAudioProcessor::~V4JBAudioProcessor()
{
    stopTimer();
    analysisPool.removeAllJobs (true, 10000);
}

bool V4JBAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono())
        return false;
    return layouts.getMainInputChannelSet().isDisabled();
}

void V4JBAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sr = sampleRate;
    engine->prepare (sampleRate);
    fxBlock = juce::jmax (samplesPerBlock, 512);
    monoScratch.assign ((size_t) juce::jmax (samplesPerBlock, 8192), 0.f);

    const juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32) fxBlock, 2 };
    chorus.prepare (spec);
    phaser.prepare (spec);
    reverb.prepare (spec);
    limiter.prepare (spec);
    limiter.setThreshold (-0.5f);
    limiter.setRelease (80.f);
    delay.setMaximumDelayInSamples ((int) (sampleRate * 2.6));  // 1 bar at 92 bpm, or 2000 ms
    delay.prepare (spec);
    delaySamples.reset (sampleRate, 0.08);
    delaySamples.setCurrentAndTargetValue ((float) (sampleRate * 0.375));
    delayWasOn = false;
    reverbParams = {};
}

void V4JBAudioProcessor::pushKey (int note, bool on) noexcept
{
    const auto scope = keyFifo.write (1);
    if (scope.blockSize1 > 0)
        keyEvents[scope.startIndex1] = { (juce::uint8) juce::jlimit (0, 127, note),
                                         (juce::uint8) (on ? 1 : 0) };
    else
        keyOverflow.store (true, std::memory_order_release);
}

void V4JBAudioProcessor::drainKeys() noexcept
{
    // Consume panic BEFORE taking the read snapshot. A concurrent overflow then
    // remains pending for the next callback instead of clearing a newer panic.
    const bool panic = keyOverflow.exchange (false, std::memory_order_acquire);
    const auto scope = keyFifo.read (keyFifo.getNumReady());
    auto apply = [this] (int i)
    {
        const auto e = keyEvents[i];
        if (e.on != 0)
            engine->noteOn ((int) e.note, 0.8f);
        else
            engine->noteOff ((int) e.note);
    };
    if (panic)
    {
        engine->allNotesOff();
        return; // discard this potentially incomplete batch (RAII commits read)
    }
    for (int i = 0; i < scope.blockSize1; ++i)
        apply (scope.startIndex1 + i);
    for (int i = 0; i < scope.blockSize2; ++i)
        apply (scope.startIndex2 + i);
}

EngineParams V4JBAudioProcessor::readParams() const
{
    EngineParams p;
    auto& s = p.spec;
    s.algo = ldi (ptr.spAlgo); s.depth = ld (ptr.spDepth); s.tilt = ld (ptr.spTilt);
    s.partials = ld (ptr.spPartials); s.modDepth = ld (ptr.spModDepth); s.modRate = ld (ptr.spModRate);
    s.noiseMix = ld (ptr.nzMix); s.noiseWidth = ld (ptr.nzWidth); s.noiseLow = ld (ptr.nzLow);
    s.noiseHigh = ld (ptr.nzHigh); s.noiseDecay = ld (ptr.nzDecay); s.sub1 = ld (ptr.sub1); s.sub2 = ld (ptr.sub2);
    for (int i = 0; i < kNumSlots; ++i)
    {
        const auto& q = ptr.slot[i];
        auto& sl = p.slot[i];
        sl.mod.type = ldi (q.type);
        sl.mod.main = ld (q.main);
        for (int k = 0; k < 4; ++k)
            sl.mod.p[k] = ld (q.p[k]);
        sl.envSel = ldi (q.es); sl.envAmt = ld (q.ea);
        sl.lfoSel = ldi (q.ls); sl.lfoAmt = ld (q.la);
        sl.midiSel = ldi (q.ms); sl.midiAmt = ld (q.ma);
    }
    for (int i = 0; i < kNumEnvs; ++i)
        p.env[i] = { ld (ptr.env[i][0]), ld (ptr.env[i][1]), ld (ptr.env[i][2]), ld (ptr.env[i][3]), ld (ptr.env[i][4]) };
    for (int i = 0; i < kNumLfos; ++i)
        p.lfo[i] = { ld (ptr.lfo[i][0]), ldb (ptr.lfo[i][1]), ldi (ptr.lfo[i][2]), ldi (ptr.lfo[i][3]) };
    for (int w = 0; w < 2; ++w)
    {
        p.wave.start[w] = ld (ptr.wave[w][0]);
        p.wave.end[w] = ld (ptr.wave[w][1]);
        p.wave.speed[w] = ld (ptr.wave[w][2]);
    }
    p.wave.balance = ld (ptr.waveBal);
    p.pitch = ld (ptr.pitch);
    p.octave = ldi (ptr.octave);
    p.glide = ld (ptr.glide);
    p.poly = ldi (ptr.poly);
    p.economy = ldb (ptr.economy);
    p.spread = ld (ptr.spread);
    p.userSlot = ldi (ptr.muser);
    return p;
}

void V4JBAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    const int n = buffer.getNumSamples();

    // Transport: host tempo always, host beat position only while playing.
    double bpm = 120.0, ppq = 0.0;
    bool playing = false;
    if (auto* ph = getPlayHead())
        if (auto pos = ph->getPosition())
        {
            if (auto b = pos->getBpm())
                bpm = *b;
            if (pos->getIsPlaying())
                if (auto q = pos->getPpqPosition())
                {
                    ppq = *q;
                    playing = true;
                }
        }
    bpm = v4jb::safeHostBpm (bpm);
    engine->setTransport (bpm, playing && std::isfinite (ppq), ppq);

    // Morph pad -> macros -> engine parameters.
    float offsets[kNumMacros], macros[kNumMacros];
    bool locks[kNumMacros];
    for (int m = 0; m < kNumMacros; ++m)
    {
        offsets[m] = ld (ptr.macro[m]);
        locks[m] = ldb (ptr.lock[m]);
    }
    morph.advance (ldi (ptr.mmode), ld (ptr.mx), ld (ptr.my), ld (ptr.mtime), ldi (ptr.msnap),
                   engine->beatClock(), (float) (n / sr), engine->takeNoteStarted());
    morph.macros (offsets, locks, macros);
    engine->setParams (readParams(), macros);
    engine->setWaves (waveSlots[0].acquire(), waveSlots[1].acquire());
    struct WaveReadEnd
    {
        V4JBAudioProcessor& p;
        ~WaveReadEnd() { p.engine->setWaves (nullptr, nullptr); }
    } waveReadEnd { *this };
    drainKeys();

    if (buffer.getNumChannels() == 0) { midi.clear(); return; }

    auto* L = buffer.getWritePointer (0);
    const bool stereo = buffer.getNumChannels() > 1;
    auto event = midi.begin();
    int off = 0;
    do
    {
        const int m = juce::jmin (n - off, (int) monoScratch.size());
        auto* left = L + off;
        auto* R = stereo ? buffer.getWritePointer (1) + off : monoScratch.data();

        // Render up to each MIDI event, apply it, carry on: notes land on their exact sample.
        int pos = 0;
        while (event != midi.end())
        {
            const auto meta = *event;
            const int at = juce::jlimit (pos, n - off, meta.samplePosition - off);
            if (at > m) break;
            ++event;
            if (at > pos) engine->render (left + pos, R + pos, at - pos);
            pos = at;

            // Unsupported long messages (SysEx) must not allocate MidiMessage
            // storage on the audio thread merely to be ignored below.
            if (meta.numBytes > 3) continue;
            const auto msg = meta.getMessage();
            if (msg.isNoteOn())
                engine->noteOn (msg.getNoteNumber(), msg.getFloatVelocity());
            else if (msg.isNoteOff())
                engine->noteOff (msg.getNoteNumber());
            else if (msg.isAllNotesOff() || msg.isAllSoundOff())
                engine->allNotesOff();
            else if (msg.isPitchWheel())
                engine->setPitchBend ((float) (msg.getPitchWheelValue() - 8192) / 8192.f * 2.f);  // +-2 semitones
            else if (msg.isControllerOfType (1))
                engine->setModWheel ((float) msg.getControllerValue() / 127.f);
            else if (msg.isChannelPressure())
                engine->setAftertouch ((float) msg.getChannelPressureValue() / 127.f);
            else if (msg.isAftertouch())
                engine->setAftertouch ((float) msg.getAfterTouchValue() / 127.f);
        }
        if (m > pos) engine->render (left + pos, R + pos, m - pos);

        processEffects (left, R, m, macros, bpm);

        if (! stereo)
        {
            for (int i = 0; i < m; ++i)
                left[i] = 0.5f * (left[i] + R[i]);
        }
        off += m;
    } while (off < n);
    midi.clear();

    const float lv = ld (ptr.level);
    buffer.applyGainRamp (0, n, lastLevel, lv);
    lastLevel = lv;
}

// Slot 12: drive, chorus/phaser, delay, reverb, then width and the limiter.
// The FX Mix macro shifts every effect's mix, the Width macro the stereo width.
void V4JBAudioProcessor::processEffects (float* L, float* R, int n, const float* macros, double bpm)
{
    const float mixOff = macros[8] - 0.5f;
    const float width = juce::jlimit (0.f, 2.f, ld (ptr.width) + (macros[9] - 0.5f));

    if (ldb (ptr.drOn))
    {
        const float g = 1.f + ld (ptr.drAmt) * 20.f, norm = 1.f / std::tanh (g);
        const float mix = clamp01 (ld (ptr.drMix) + mixOff);
        for (int i = 0; i < n; ++i)
        {
            L[i] += mix * (std::tanh (g * L[i]) * norm - L[i]);
            R[i] += mix * (std::tanh (g * R[i]) * norm - R[i]);
        }
    }

    const bool dlOn = ldb (ptr.dlOn);
    if (dlOn && ! delayWasOn)
        delay.reset();  // no stale echoes from the last time it was on
    delayWasOn = dlOn;
    if (dlOn)
    {
        const float secs = ldb (ptr.dlSync) ? kDelayDivBeats[juce::jlimit (0, kNumDelayDivs - 1, ldi (ptr.dlDiv))] * 60.f / (float) bpm
                                            : ld (ptr.dlTime) * 0.001f;
        delaySamples.setTargetValue (juce::jlimit (1.f, (float) delay.getMaximumDelayInSamples() - 1.f, secs * (float) sr));
    }

    // juce::dsp effects keep per-block buffers sized at prepare, so feed them in fxBlock chunks.
    for (int off = 0; off < n; off += fxBlock)
    {
        const int len = juce::jmin (fxBlock, n - off);
        float* ch[2] = { L + off, R + off };
        juce::dsp::AudioBlock<float> block (ch, 2, (size_t) len);
        juce::dsp::ProcessContextReplacing<float> ctx (block);

        if (ldb (ptr.mdOn))
        {
            const float mix = clamp01 (ld (ptr.mdMix) + mixOff), depth = ld (ptr.mdDepth), rate = ld (ptr.mdRate);
            if (ldi (ptr.mdType) == 0)
            {
                chorus.setRate (rate);
                chorus.setDepth (depth);
                chorus.setCentreDelay (7.f + depth * 10.f);
                chorus.setFeedback (0.2f);
                chorus.setMix (mix);
                chorus.process (ctx);
            }
            else
            {
                phaser.setRate (rate);
                phaser.setDepth (depth);
                phaser.setCentreFrequency (800.f);
                phaser.setFeedback (0.5f);
                phaser.setMix (mix);
                phaser.process (ctx);
            }
        }

        if (dlOn)
        {
            // Ping-pong: each side's echo feeds the other side.
            const float fb = ld (ptr.dlFb), mix = clamp01 (ld (ptr.dlMix) + mixOff);
            float* l = ch[0];
            float* r = ch[1];
            for (int i = 0; i < len; ++i)
            {
                const float t = delaySamples.getNextValue();
                const float dl = delay.popSample (0, t), dr = delay.popSample (1, t);
                delay.pushSample (0, l[i] + dr * fb);
                delay.pushSample (1, r[i] + dl * fb);
                l[i] = (1.f - mix) * l[i] + mix * dl;
                r[i] = (1.f - mix) * r[i] + mix * dr;
            }
        }

        if (ldb (ptr.rvOn))
        {
            juce::Reverb::Parameters rp;
            const float mix = clamp01 (ld (ptr.rvMix) + mixOff);
            rp.roomSize = ld (ptr.rvSize);
            rp.damping = ld (ptr.rvDamp);
            rp.wetLevel = mix;
            // juce::Reverb internally multiplies dryLevel by 2. Keep unity
            // dry at 0%, and no direct signal at 100%; retain its wet scaling.
            rp.dryLevel = 0.5f * (1.f - mix);
            rp.width = 1.f;
            if (rp.roomSize != reverbParams.roomSize || rp.damping != reverbParams.damping
                || rp.wetLevel != reverbParams.wetLevel || rp.dryLevel != reverbParams.dryLevel)
            {
                reverbParams = rp;
                reverb.setParameters (rp);
            }
            reverb.process (ctx);
        }

        // Mid/side width: 0 = mono, 1 = as is, 2 = extra wide.
        for (int i = 0; i < len; ++i)
        {
            const float mid = 0.5f * (ch[0][i] + ch[1][i]), side = 0.5f * (ch[0][i] - ch[1][i]) * width;
            ch[0][i] = mid + side;
            ch[1][i] = mid - side;
        }
        limiter.process (ctx);
    }
}

// ---------------------------------------------------------------- morph

juce::ValueTree V4JBAudioProcessor::morphTree (bool points, bool path) const
{
    juce::ValueTree t ("MORPH");
    if (points)
        for (int c = 0; c < 4; ++c)
        {
            float v[kNumMacros];
            for (int m = 0; m < kNumMacros; ++m)
                v[m] = morph.corner[c][m].load();
            t.setProperty ("c" + juce::String (c), joinFloats (v, kNumMacros), nullptr);
        }
    if (path)
    {
        juce::StringArray nodes;
        for (int i = 0; i < morph.numNodes.load(); ++i)
            nodes.add (juce::String (morph.nodeX[i].load(), 9, true) + " " + juce::String (morph.nodeY[i].load(), 9, true)
                       + " " + juce::String (morph.nodeLen[i].load()));
        t.setProperty ("nodes", nodes.joinIntoString (";"), nullptr);
    }
    return t;
}

void V4JBAudioProcessor::applyMorphTree (const juce::ValueTree& t, bool resetMissing)
{
    for (int c = 0; c < 4; ++c)
    {
        const auto key = "c" + juce::String (c);
        if (t.hasProperty (key))
        {
            auto vals = juce::StringArray::fromTokens (t[key].toString(), " ", "");
            for (int m = 0; m < kNumMacros; ++m)
                morph.corner[c][m].store (m < vals.size() ? clamp01 (vals[m].getFloatValue()) : 0.5f);
        }
        else if (resetMissing)
            for (int m = 0; m < kNumMacros; ++m)
                morph.corner[c][m].store (0.5f);
    }
    if (t.hasProperty ("nodes") || resetMissing)
    {
        auto nodes = juce::StringArray::fromTokens (t["nodes"].toString(), ";", "");
        nodes.removeEmptyStrings();
        const int count = juce::jmin (nodes.size(), kMaxNodes);
        morph.numNodes.store (0);  // audio sees no path while nodes change
        for (int i = 0; i < count; ++i)
        {
            auto f = juce::StringArray::fromTokens (nodes[i], " ", "");
            morph.nodeX[i].store (clamp01 (f[0].getFloatValue()));
            morph.nodeY[i].store (clamp01 (f[1].getFloatValue()));
            morph.nodeLen[i].store (juce::jlimit (0, kNumSegLens - 1, f[2].getIntValue()));
        }
        morph.numNodes.store (count);
    }
}

void V4JBAudioProcessor::storeCorner (int c)
{
    // Store what you hear, then park the puck on that corner with offsets at zero,
    // so the sound does not jump.
    for (int m = 0; m < kNumMacros; ++m)
        morph.corner[c][m].store (morph.outMacro[m].load());
    for (int m = 0; m < kNumMacros; ++m)
        if (! ldb (ptr.lock[m])) setParam (ParamID::macro (m), 0.f);
    recallCorner (c);
}

void V4JBAudioProcessor::recallCorner (int c)
{
    setParam (ParamID::morphX, (c == 1 || c == 3) ? 1.f : 0.f);
    setParam (ParamID::morphY, (c >= 2) ? 1.f : 0.f);
}

bool V4JBAudioProcessor::saveMorphFile (const juce::File& f, bool points)
{
    auto t = morphTree (points, ! points);
    if (auto xml = t.createXml())
        return xml->writeTo (f);
    return false;
}

bool V4JBAudioProcessor::loadMorphFile (const juce::File& f)
{
    auto xml = juce::parseXML (f);
    if (xml == nullptr || ! xml->hasTagName ("MORPH"))
        return false;
    const auto tree = juce::ValueTree::fromXml (*xml);
    if (! validMorph (tree)) return false;
    applyMorphTree (tree, false);  // a points file leaves the path alone, and vice versa
    return true;
}

// ---------------------------------------------------------------- waves

void V4JBAudioProcessor::loadWave (int w, const juce::String& source)
{
    const juce::ScopedLock lock (waveMutex);
    const int gen = ++waveGen[w];
    juce::WeakReference<V4JBAudioProcessor> weak (this);
    analysisPool.addJob ([weak, w, gen, source]
    {
        auto data = WaveAnalyzer::analyse (source);
        juce::MessageManager::callAsync ([weak, w, gen, source, data]
        {
            if (auto* self = weak.get())
            {
                const juce::ScopedLock guard (self->waveMutex);
                if (self->waveGen[w] != gen) return;
                if (data != nullptr) self->publishWave (w, data, source);
                else { self->waveError[w] = "Could not load audio: " + source; ++self->waveVersion; }
            }
        });
    });
}

void V4JBAudioProcessor::clearWave (int w)
{
    const juce::ScopedLock lock (waveMutex);
    ++waveGen[w];
    publishWave (w, nullptr, {});
}

void V4JBAudioProcessor::publishWave (int w, std::shared_ptr<WaveData> data, const juce::String& source)
{
    const juce::ScopedLock lock (waveMutex);
    waveSlots[w].publish (std::move (data));
    waveSource[w] = source;
    waveError[w].clear();
    ++waveVersion;
}

void V4JBAudioProcessor::timerCallback()
{
    const juce::ScopedLock lock (waveMutex);
    for (auto& slot : waveSlots) slot.collect();
}

// ---------------------------------------------------------------- state and presets

void V4JBAudioProcessor::setParam (const juce::String& id, float realValue)
{
    if (auto* p = apvts.getParameter (id))
        p->setValueNotifyingHost (p->convertTo0to1 (realValue));
}

void V4JBAudioProcessor::resetParamsToDefaults()
{
    for (auto* p : getParameters())
        if (auto* rp = dynamic_cast<juce::RangedAudioParameter*> (p))
            rp->setValueNotifyingHost (rp->getDefaultValue());
}

std::unique_ptr<juce::XmlElement> V4JBAudioProcessor::createStateXml()
{
    const juce::ScopedLock lock (waveMutex);
    auto state = apvts.copyState();
    for (int i = state.getNumChildren(); --i >= 0;)
        if (state.getChild (i).hasType ("WAVE_DATA")) state.removeChild (i, nullptr);
    state.setProperty ("waveDataVersion", 1, nullptr);
    for (int w = 0; w < 2; ++w)
        if (auto wave = waveSlots[w].current()) WaveState::append (state, w, *wave);
    state.removeChild (state.getChildWithName ("MORPH"), nullptr);
    state.appendChild (morphTree (true, true), nullptr);
    state.setProperty ("waveA", waveSource[0], nullptr);
    state.setProperty ("waveB", waveSource[1], nullptr);
    state.setProperty ("presetName", currentPresetName, nullptr);
    state.setProperty ("category", currentPresetCategory, nullptr);
    state.setProperty ("uiScale", uiScale.load(), nullptr);
    return state.createXml();
}

bool V4JBAudioProcessor::applyStateXml (const juce::XmlElement& xml, bool fromPreset)
{
    const juce::ScopedLock lock (waveMutex);
    auto tree = juce::ValueTree::fromXml (xml);
    if (! tree.isValid() || (int) tree.getProperty ("waveDataVersion", 0) > 1) return false;
    if (! validMorph (tree.getChildWithName ("MORPH"))) return false;
    if (! std::isfinite (xml.getDoubleAttribute ("uiScale", uiScale.load()))) return false;
    std::shared_ptr<WaveData> restored[2];
    bool embedded[2] {};
    // Validate ALL resources before touching parameters or previously valid sound.
    for (auto child : tree)
        if (child.hasType ("WAVE_DATA"))
        {
            const int w = child.getProperty ("slot", -1);
            if (w < 0 || w > 1 || embedded[w]) return false;
            embedded[w] = true;
            restored[w] = WaveState::decode (child);
            if (! restored[w]) { waveError[w] = "Invalid embedded wave data; state not loaded"; ++waveVersion; return false; }
        }
    for (int w = 0; w < 2; ++w)
        if (! embedded[w] && xml.hasAttribute (w == 0 ? "waveA" : "waveB"))
        {
            const auto src = xml.getStringAttribute (w == 0 ? "waveA" : "waveB");
            if (src.isNotEmpty())
            {
                restored[w] = WaveAnalyzer::analyse (src);
                if (! restored[w]) { waveError[w] = "Missing or invalid wave; state not loaded: " + src; ++waveVersion; return false; }
            }
        }
    for (auto* e : xml.getChildWithTagNameIterator ("PARAM"))
    {
        const auto value = e->getDoubleAttribute ("value");
        if (! std::isfinite (value)) return false;
        if (auto* parameter = apvts.getParameter (e->getStringAttribute ("id")))
        {
            const auto& range = parameter->getNormalisableRange();
            const float realValue = (float) value;
            if (! std::isfinite (realValue) || realValue < range.start || realValue > range.end) return false;
        }
    }
    if (fromPreset)
    {
        // Factory presets may be partial: start from defaults, then apply what is listed.
        resetParamsToDefaults();
        for (auto* e : xml.getChildWithTagNameIterator ("PARAM"))
        {
            jassert (apvts.getParameter (e->getStringAttribute ("id")) != nullptr);  // typo in a preset id
            setParam (e->getStringAttribute ("id"), (float) e->getDoubleAttribute ("value"));
        }
    }
    else
    {
        auto parameters = tree.createCopy();
        for (int i = parameters.getNumChildren(); --i >= 0;)
            if (parameters.getChild (i).hasType ("WAVE_DATA")) parameters.removeChild (i, nullptr);
        apvts.replaceState (parameters);
        uiScale = juce::jlimit (0.4f, 1.5f, (float) xml.getDoubleAttribute ("uiScale", uiScale.load()));  // sessions only, presets keep your size
    }

    applyMorphTree (tree.getChildWithName ("MORPH"), true);
    for (int w = 0; w < 2; ++w)
    {
        const auto key = w == 0 ? "waveA" : "waveB";
        if (! xml.hasAttribute (key) && ! embedded[w])
            continue;  // presets without waveA/waveB keep whatever waves are loaded
        const auto src = xml.getStringAttribute (key);
        ++waveGen[w]; // stale worker completions cannot overwrite recalled state
        publishWave (w, restored[w], src);
    }
    currentPresetName = xml.getStringAttribute ("presetName", currentPresetName);
    if (xml.hasAttribute ("category"))
        currentPresetCategory = xml.getStringAttribute ("category");
    else
    {
        currentPresetCategory.clear();
        for (const auto& e : factoryPresets)
            if (e.name == currentPresetName)
            {
                currentPresetCategory = e.category;
                break;
            }
    }
    return true;
}

void V4JBAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = createStateXml())
        copyXmlToBinary (*xml, destData);
}

void V4JBAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (data == nullptr || sizeInBytes <= 0 || sizeInBytes > 16 * 1024 * 1024) return;
    // Hosts can hand us anything. Only accept our own tree.
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            applyStateXml (*xml, false);
}

void V4JBAudioProcessor::setCurrentProgram (int index)
{
    if (juce::isPositiveAndBelow (index, factoryPresets.size()))
    {
        currentProgram = index;
        loadPreset (factoryPresets.getReference (index));
    }
}

const juce::String V4JBAudioProcessor::getProgramName (int index)
{
    return juce::isPositiveAndBelow (index, factoryPresets.size()) ? factoryPresets[index].name : juce::String ("Init");
}

void V4JBAudioProcessor::loadPreset (const PresetEntry& e)
{
    const juce::ScopedLock lock (waveMutex);
    if (auto xml = Presets::parse (e))
        if (xml->hasTagName (apvts.state.getType()))
        {
            if (! applyStateXml (*xml, true)) return;
            currentPresetName = e.name;
            currentPresetCategory = e.category;
        }
}

bool V4JBAudioProcessor::saveUserPreset (const juce::String& name, const juce::String& category)
{
    const juce::ScopedLock lock (waveMutex);
    const auto clean = juce::File::createLegalFileName (name.trim());
    if (clean.isEmpty())
        return false;
    currentPresetName = name.trim();
    currentPresetCategory = category.isNotEmpty() ? category : juce::String ("User");
    auto xml = createStateXml();
    xml->setAttribute ("presetName", currentPresetName);
    xml->setAttribute ("category", currentPresetCategory);
    const auto dir = Presets::userDir();
    return dir.createDirectory() && xml->writeTo (dir.getChildFile (clean + ".xml"));
}

// Smart randomizer: random, but inside ranges that tend to sound like music.
// newModules = also pick a new module chain; otherwise only the values move.
void V4JBAudioProcessor::randomize (bool newModules)
{
    const juce::ScopedLock lock (waveMutex);
    juce::Random r;
    auto between = [&r] (float lo, float hi) { return lo + (hi - lo) * r.nextFloat(); };
    auto chance = [&r] (float p) { return r.nextFloat() < p; };
    using M = ModuleType;
    namespace I = ParamID;

    if (newModules)
    {
        // A curated pool. Wave modules only when a wave is loaded; at most one Double Wave.
        std::vector<M> pool { M::FiveSines, M::OddEven, M::Sync, M::Adder, M::DiscreteAdder, M::EveryNth,
                              M::Lowpass, M::Highpass, M::Bandpass, M::Notch, M::MovingFilter, M::Decay, M::Hype,
                              M::Modulator, M::Detuned, M::Blur, M::Noise, M::Octaver, M::SecondTone,
                              M::Phrase, M::RandomDrops, M::Tail, M::Clip, M::HarmonicAdd, M::RangeEnhance };
        if (getWave (0) != nullptr)
            pool.push_back (M::Wave);
        if (getWave (0) != nullptr || getWave (1) != nullptr)
            pool.push_back (M::DoubleWave);
        const int active = 3 + r.nextInt (4);
        bool doubleUsed = false;
        for (int i = 0; i < kNumSlots; ++i)
        {
            M t = M::Off;
            if (i < active)
            {
                t = (i == 0 && chance (0.6f)) ? (chance (0.5f) ? M::Lowpass : M::Bandpass)
                                              : pool[(size_t) r.nextInt ((int) pool.size())];
                if (t == M::DoubleWave && doubleUsed)
                    t = M::Hype;
                doubleUsed = doubleUsed || t == M::DoubleWave;
            }
            setParam (I::slot (i, "type"), (float) (int) t);
        }
    }

    for (int i = 0; i < kNumSlots; ++i)
    {
        setParam (I::slot (i, "main"), between (0.2f, 0.85f));
        for (int k = 0; k < 4; ++k)
            setParam (I::slotP (i, k), between (0.2f, 0.8f));
        const bool lfoRoute = chance (0.3f);
        setParam (I::slot (i, "ls"), lfoRoute ? (float) (1 + r.nextInt (3)) : 0.f);
        setParam (I::slot (i, "la"), lfoRoute ? between (-0.3f, 0.3f) : 0.f);
        const bool envRoute = chance (0.2f);
        setParam (I::slot (i, "es"), envRoute ? (float) (2 + r.nextInt (3)) : 0.f);
        setParam (I::slot (i, "ea"), envRoute ? between (-0.4f, 0.4f) : 0.f);
    }

    const bool distort = chance (0.4f);
    setParam (I::spAlgo, (float) r.nextInt (kNumAlgos));
    setParam (I::spDepth, distort ? between (0.2f, 0.8f) : 0.f);
    setParam (I::spTilt, between (0.6f, 1.8f));
    setParam (I::spPartials, (float) (64 + r.nextInt (449)));
    setParam (I::spModDepth, between (0.f, 0.5f));
    setParam (I::spModRate, between (0.1f, 0.6f));
    setParam (I::nzMix, chance (0.5f) ? between (0.f, 0.5f) : 0.f);
    setParam (I::nzWidth, between (0.2f, 0.9f));
    setParam (I::nzDecay, chance (0.3f) ? between (0.3f, 0.9f) : 0.f);
    setParam (I::sub1, chance (0.4f) ? between (0.f, 0.5f) : 0.f);
    setParam (I::sub2, chance (0.2f) ? between (0.f, 0.3f) : 0.f);

    const bool pad = chance (0.6f);
    setParam (I::env (0, "a"), pad ? between (0.2f, 2.5f) : between (0.002f, 0.05f));
    setParam (I::env (0, "d"), between (0.2f, 4.f));
    setParam (I::env (0, "s"), pad ? between (0.5f, 1.f) : between (0.f, 0.6f));
    setParam (I::env (0, "r"), pad ? between (1.f, 6.f) : between (0.2f, 1.5f));
    for (int i = 1; i < kNumEnvs; ++i)
    {
        setParam (I::env (i, "a"), between (0.005f, 3.f));
        setParam (I::env (i, "d"), between (0.1f, 4.f));
        setParam (I::env (i, "s"), between (0.f, 1.f));
        setParam (I::env (i, "r"), between (0.1f, 4.f));
    }
    for (int i = 0; i < kNumLfos; ++i)
    {
        setParam (I::lfo (i, "rate"), between (0.03f, 3.f));
        setParam (I::lfo (i, "shape"), (float) r.nextInt (kNumLfoShapes));
    }

    setParam (I::mdOn, chance (0.6f) ? 1.f : 0.f);
    setParam (I::mdType, (float) r.nextInt (2));
    setParam (I::mdMix, between (0.1f, 0.5f));
    setParam (I::dlOn, chance (0.4f) ? 1.f : 0.f);
    setParam (I::dlFb, between (0.2f, 0.6f));
    setParam (I::dlMix, between (0.1f, 0.35f));
    setParam (I::rvOn, chance (0.85f) ? 1.f : 0.f);
    setParam (I::rvSize, between (0.4f, 0.95f));
    setParam (I::rvMix, between (0.15f, 0.5f));

    for (int c = 0; c < 4; ++c)
        for (int m = 0; m < kNumMacros; ++m)
            morph.corner[c][m].store (between (0.2f, 0.8f));

    currentPresetName = newModules ? "Random" : "Random (varied)";
    currentPresetCategory.clear();  // not a real group; arrows fall back to the full list
}

juce::AudioProcessorEditor* V4JBAudioProcessor::createEditor()
{
    return new V4JBAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new V4JBAudioProcessor();
}
