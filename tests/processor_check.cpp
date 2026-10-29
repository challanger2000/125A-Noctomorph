#include <JuceHeader.h>
#include <BinaryData.h>
// Access to private processor/editor internals is granted explicitly by
// test-only friend declarations; never rewrite C++ access specifiers on MSVC.
#include "PluginEditor.h"
#include "WaveAnalyzer.h"
#include <iostream>
#include <limits>
#include <thread>
#include <cstdlib>
#include <set>
#include <fstream>
static thread_local bool measureNew=false;
static thread_local size_t audioNews=0;
#if AUDIT_WRAP_ALLOCATIONS
extern "C" void* __real_malloc(size_t);
extern "C" void* __real_calloc(size_t,size_t);
extern "C" void* __real_realloc(void*,size_t);
extern "C" void* __wrap_malloc(size_t n) {if(measureNew)++audioNews;return __real_malloc(n);}
extern "C" void* __wrap_calloc(size_t n,size_t s) {if(measureNew)++audioNews;return __real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,size_t n) {if(measureNew)++audioNews;return __real_realloc(p,n);}
#endif
void* operator new(size_t n) { if(measureNew) ++audioNews; if(auto* p=std::malloc(n?n:1))return p;throw std::bad_alloc(); }
void* operator new[](size_t n) {return ::operator new(n);}
void operator delete(void* p) noexcept {std::free(p);}
void operator delete[](void* p) noexcept {std::free(p);}
void operator delete(void* p,size_t) noexcept {std::free(p);}
void operator delete[](void* p,size_t) noexcept {std::free(p);}

static int failures = 0;
#define CHECK(x) do { if (!(x)) { std::cerr << "FAIL " << __LINE__ << ": " << #x << '\n'; ++failures; } } while(false)
using P = V4JBAudioProcessor;
static void set(P& p, const juce::String& id, float v) { auto* q=p.apvts.getParameter(id); CHECK(q); if(q) q->setValueNotifyingHost(q->convertTo0to1(v)); }
static void init(P& p, int channels=2, double sr=48000)
{
    p.setPlayConfigDetails(0,channels,sr,128);
    for (auto id : {ParamID::drOn,ParamID::mdOn,ParamID::dlOn,ParamID::rvOn}) set(p,id,0);
    set(p,ParamID::level,1); set(p,ParamID::env(0,"a"),0.001f); set(p,ParamID::env(0,"r"),0.001f);
    p.prepareToPlay(sr,128);
    juce::AudioBuffer<float> b(channels,128); juce::MidiBuffer m; p.processBlock(b,m);
}
static bool finite(const juce::AudioBuffer<float>& b) { for(int c=0;c<b.getNumChannels();++c) for(int i=0;i<b.getNumSamples();++i) if(!std::isfinite(b.getSample(c,i))) return false; return true; }
static void pump(int ms=10) { juce::MessageManager::getInstance()->runDispatchLoopUntil(ms); }

static void blocks()
{
    for(double sr : {44100.,48000.,96000.}) for(int n : {0,1,16,127,128,511,8192,8193,16384,32769})
    {
        P mono,stereo; init(mono,1,sr); init(stereo,2,sr);
        juce::AudioBuffer<float> a(1,n), b(2,n); juce::MidiBuffer ma,mb;
        ma.addEvent(juce::MidiMessage::noteOn(1,60,0.8f),0); mb=ma;
        mono.processBlock(a,ma); stereo.processBlock(b,mb);
        CHECK(finite(a)&&finite(b));
        for(int i=0;i<n;++i) if(std::abs(a.getSample(0,i)-0.5f*(b.getSample(0,i)+b.getSample(1,i)))>1e-6f) { CHECK(false && "mono/stereo PCM mismatch"); break; }
        if(n>8192) CHECK(a.getMagnitude(0,n-512,512)>1e-5f);
    }
}
void keys()
{
    P p; init(p);
    // An accepted on followed by a rejected off used to hang forever.
    p.pushKey(60,true);
    for(int i=0;i<31;++i) p.pushKey(61+i%12,true);
    p.pushKey(60,false);
    for(int note=0;note<128;++note) p.pushKey(note,false);
    juce::AudioBuffer<float> b(2,512); juce::MidiBuffer m;
    for(int i=0;i<100;++i) p.processBlock(b,m);
    for(int i=0;i<v4jb::kMaxVoices;++i) CHECK(!p.engine->voiceHeld(i));
    CHECK(b.getMagnitude(0,512)<1e-5f);
}
void wet()
{
    const float neutral[v4jb::kNumMacros]={.5f,.5f,.5f,.5f,.5f,.5f,.5f,.5f,.5f,.5f,.5f};
    P dry;init(dry);std::vector<float> dl(8192,0),dr(8192,0);
    dry.processEffects(dl.data(),dr.data(),8192,neutral,120);dl[0]=dr[0]=.1f;dry.processEffects(dl.data(),dr.data(),8192,neutral,120);
    const float direct=dl[0];
    for(bool rv : {false,true}) for(float mix : {0.f,0.25f,0.5f,0.75f,1.f})
    {
        P p; init(p); set(p,rv?ParamID::rvOn:ParamID::dlOn,1); set(p,rv?ParamID::rvMix:ParamID::dlMix,mix);
        set(p,ParamID::dlSync,0); set(p,ParamID::dlTime,100);
        std::vector<float> l(8192,0),r(8192,0);
        p.processEffects(l.data(),r.data(),8192,neutral,120); // settle JUCE ramps
        l[0]=r[0]=0.1f;
        p.processEffects(l.data(),r.data(),8192,neutral,120);
        std::cout<<(rv?"reverb":"delay")<<" mix="<<mix<<" direct="<<l[0]<<'\n';
        CHECK(std::abs(l[0]-direct*(1.f-mix))<1e-6f);
        if(mix==1) CHECK(std::abs(l[0])<1e-7f && std::abs(r[0])<1e-7f);
        if(mix==0) CHECK(l[0]>0.05f);
        if(mix>0) CHECK(juce::FloatVectorOperations::findMinAndMax(l.data()+1,8191).getLength()>1e-5f);
    }
}
struct Head : juce::AudioPlayHead { double bpm=120,ppq=0; juce::Optional<PositionInfo> getPosition() const override { PositionInfo p;p.setBpm(bpm);p.setPpqPosition(ppq);p.setIsPlaying(true);return p;} };
void tempo()
{
    for(double bpm : {0.,-120.,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN(),1e-300,120.})
    {
        P p; init(p); Head h; h.bpm=bpm;p.setPlayHead(&h);set(p,ParamID::dlOn,1);
        juce::AudioBuffer<float> b(2,512); juce::MidiBuffer m;m.addEvent(juce::MidiMessage::noteOn(1,60,.8f),0);
        p.processBlock(b,m);CHECK(finite(b));CHECK(std::isfinite(p.engine->beatClock()));p.setPlayHead(nullptr);
    }
}
void state()
{
    P a,b; init(a); init(b);
    for(auto* q:a.getParameters()) q->setValueNotifyingHost(q->isBoolean()?1.f:0.37f);
    for(int c=0;c<4;++c)for(int m=0;m<v4jb::kNumMacros;++m)a.morph.corner[c][m].store(0.12345678f);
    a.morph.corner[1][0].store(1e-12f);
    juce::MemoryBlock s;a.getStateInformation(s);b.setStateInformation(s.getData(),(int)s.getSize());
    for(int i=0;i<a.getParameters().size();++i) { auto* ap=a.getParameters()[i];auto* bp=b.getParameters()[i]; if(std::abs(ap->getValue()-bp->getValue())>=1e-6f) std::cerr<<ap->getName(100)<<" before="<<ap->getValue()<<" after="<<bp->getValue()<<"\n";CHECK(std::abs(ap->getValue()-bp->getValue())<1e-6f); }
    CHECK(std::abs(a.morph.corner[0][0].load()-b.morph.corner[0][0].load())<1e-7f);
    CHECK(a.morph.corner[1][0].load()==b.morph.corner[1][0].load());
    // Fresh processor must restore the analysed audio even if its source has gone.
    auto wd=std::make_shared<v4jb::WaveData>();wd->numFrames=2;wd->seconds=1;wd->gain.assign(512,.12345f);
    a.publishWave(0,wd,"/tmp/noctomorph-removed-source.wav");a.getStateInformation(s);b.setStateInformation(s.getData(),(int)s.getSize());
    auto restored=b.getWave(0);CHECK(restored && restored->gain==wd->gain);
    auto xml=a.createStateXml();auto* payload=xml->getChildByName("WAVE_DATA");
    if(payload)
    {
        auto previous=b.getWave(0);auto value=b.apvts.getRawParameterValue(ParamID::spTilt)->load();
        payload->setAttribute("gains","corrupt");
        xml->getChildByName("PARAM")->setAttribute("value",0.99);
        b.applyStateXml(*xml,false);CHECK(b.getWave(0)==previous);CHECK(b.apvts.getRawParameterValue(ParamID::spTilt)->load()==value);
    }
    set(a,ParamID::lock(0),1);set(a,ParamID::macro(0),.2f);
    juce::AudioBuffer<float> scratch(2,128);juce::MidiBuffer midi;a.processBlock(scratch,midi);
    const float macro=a.morph.outMacro[0].load();a.storeCorner(0);a.processBlock(scratch,midi);CHECK(std::abs(macro-a.morph.outMacro[0].load())<1e-6f);
    P source,dest;init(source);init(dest);
    set(source,ParamID::slot(0,"type"),(float)v4jb::ModuleType::Wave);set(source,ParamID::slot(0,"main"),1);
    source.publishWave(0,wd,"/tmp/noctomorph-deleted.wav");source.getStateInformation(s);dest.setStateInformation(s.getData(),(int)s.getSize());
    juce::AudioBuffer<float> l(2,4096),r(2,4096);juce::MidiBuffer lm,rm;
    lm.addEvent(juce::MidiMessage::noteOn(1,60,.8f),0);rm=lm;
    source.processBlock(l,lm);dest.processBlock(r,rm);CHECK(l.getMagnitude(0,4096)>1e-5f);
    for(int ch=0;ch<2;++ch)for(int i=0;i<4096;++i)if(std::abs(l.getSample(ch,i)-r.getSample(ch,i))>1e-6f){CHECK(false&&"WAV recall PCM differs");break;}
    auto file=juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("noctomorph-invalid",".v4jbmorph");
    CHECK(file.replaceWithText("<MORPH nodes=\"0.5\"/>"));CHECK(!a.loadMorphFile(file));CHECK(file.deleteFile());
}
void midi()
{
    P p;init(p);set(p,ParamID::spModDepth,0);set(p,ParamID::nzMix,0);
    juce::AudioBuffer<float>b(2,512);juce::MidiBuffer m;
    m.addEvent(juce::MidiMessage::noteOn(1,69,.8f),100);p.processBlock(b,m);
    CHECK(b.getMagnitude(0,100)==0);CHECK(b.getMagnitude(164,348)>1e-5f);
    CHECK(std::abs(p.engine->voiceFreq(0,0)-440)<.01f);
    m.addEvent(juce::MidiMessage::pitchWheel(1,16383),0);m.addEvent(juce::MidiMessage::controllerEvent(1,1,127),1);
    m.addEvent(juce::MidiMessage::channelPressureChange(1,64),2);p.processBlock(b,m);
    CHECK(std::abs(p.engine->voiceFreq(0,0)-440*std::exp2((8191.f/8192.f*2.f)/12.f))<.02f);
    CHECK(p.engine->wheel==1);CHECK(std::abs(p.engine->touch-64.f/127.f)<1e-6f);
    m.addEvent(juce::MidiMessage::noteOff(1,69),0);for(int i=0;i<20;++i)p.processBlock(b,m);
    CHECK(b.getMagnitude(0,512)<1e-5f);
}
void waves()
{
    P p;init(p);auto old=std::make_shared<v4jb::WaveData>();old->numFrames=1;old->seconds=1;old->gain.assign(256,.5f);
    std::weak_ptr<v4jb::WaveData> sentinel=old;p.publishWave(0,old,"old");old.reset();
#if __has_include("dsp/AudioReadSlot.h")
    const auto* reading=p.waveSlots[0].acquire();
#else
    const auto* reading=p.waveLive[0].load();
#endif
    for(int i=0;i<1000;++i)
    {
        auto fresh=std::make_shared<v4jb::WaveData>();fresh->numFrames=1;fresh->seconds=1;fresh->gain.assign(256,.4f);p.publishWave(0,fresh,"new");
#if !__has_include("dsp/AudioReadSlot.h")
        for(auto& r:p.retired) r.time=juce::Time::getMillisecondCounter()-11000;
#endif
        p.timerCallback();
    }
    CHECK(!sentinel.expired());if(!sentinel.expired())CHECK(reading->gain[0]==.5f);
#if __has_include("dsp/AudioReadSlot.h")
    p.waveSlots[0].release();p.timerCallback();CHECK(sentinel.expired());
    // The expired weak pointer proves the old wave was actually reclaimed,
    // without accessing private AudioReadSlot implementation details.
#endif
    const auto prev=p.getWave(0);const int ver=p.waveVersion.load();p.loadWave(0,"/tmp/noctomorph-does-not-exist.wav");
    for(int i=0;i<100 && p.waveVersion.load()==ver;++i)pump(20);
    CHECK(p.waveVersion.load()>ver);CHECK(p.getWave(0)==prev);
    const auto file=juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("noctomorph-float",".wav");
    for(bool corrupt:{false,true})
    {
        juce::MemoryOutputStream wav;
        wav.write("RIFF",4);wav.writeInt(36+8192);wav.write("WAVEfmt ",8);wav.writeInt(16);wav.writeShort(3);wav.writeShort(1);
        wav.writeInt(48000);wav.writeInt(192000);wav.writeShort(4);wav.writeShort(32);wav.write("data",4);wav.writeInt(8192);
        for(int i=0;i<2048;++i)wav.writeFloat(i==1024?(corrupt?std::numeric_limits<float>::quiet_NaN():.5f):0.f);
        CHECK(file.replaceWithData(wav.getData(),wav.getDataSize()));
        CHECK((WaveAnalyzer::analyse(file.getFullPathName())==nullptr)==corrupt);
    }
    CHECK(file.deleteFile());
}
static void realtime()
{
    P p;init(p);juce::AudioBuffer<float> b(2,32769);juce::MidiBuffer m;
    const unsigned char sysex[]={0x7d,1,2,3,4,5,6,7};m.addEvent(juce::MidiMessage::createSysExMessage(sysex,8),0);
    m.addEvent(juce::MidiMessage::noteOn(1,60,.8f),1);
    audioNews=0;measureNew=true;p.processBlock(b,m);measureNew=false;
    CHECK(audioNews==0);CHECK(finite(b));std::cout<<"C++ new in processBlock: "<<audioNews<<'\n';
}
static void presets()
{
    P p;init(p);CHECK(p.getFactoryPresets().size()==494);
    juce::AudioBuffer<float> b(2,128);juce::MidiBuffer m;
    for(const auto& preset:p.getFactoryPresets())
    {
        auto xml=Presets::parse(preset);CHECK(xml);if(!xml)continue;
        for(auto* param:xml->getChildWithTagNameIterator("PARAM"))CHECK(p.apvts.getParameter(param->getStringAttribute("id")));
        p.loadPreset(preset);CHECK(p.currentPresetName==preset.name);
        m.addEvent(juce::MidiMessage::noteOn(1,60,.8f),0);p.processBlock(b,m);CHECK(finite(b));
        m.addEvent(juce::MidiMessage::allNotesOff(1),0);p.processBlock(b,m);
    }
    std::cout<<"494 factory presets parsed, applied, and short MIDI rendered\n";
}
static void save_failure()
{
    P p; init(p);
    p.currentPresetName = "Previous";
    p.currentPresetCategory = "Old";
    const auto dir = Presets::userDir();
    CHECK(dir.createDirectory());
    const juce::String base = "125A-QA-" + juce::Uuid().toString();
    const auto blocked = dir.getChildFile(base + ".xml");
    CHECK(blocked.createDirectory()); // saving to an existing directory must fail
    CHECK(!p.saveUserPreset(base, "New Category"));
    CHECK(p.currentPresetName == "Previous");
    CHECK(p.currentPresetCategory == "Old");
    CHECK(blocked.isDirectory());
    CHECK(blocked.deleteRecursively());

    const juce::String successName = "125A-QA-" + juce::Uuid().toString();
    const auto written = dir.getChildFile(successName + ".xml");
    CHECK(p.saveUserPreset(successName, "User"));
    CHECK(p.currentPresetName == successName);
    CHECK(p.currentPresetCategory == "User");
    CHECK(written.existsAsFile());
    CHECK(written.deleteFile());
}

void gui()
{
    P p;init(p);auto before=p.apvts.copyState().toXmlString();
    for(int run=0;run<3;++run)
    {
        auto ed=std::unique_ptr<V4JBAudioProcessorEditor>(new V4JBAudioProcessorEditor(p));
        CHECK(p.apvts.copyState().toXmlString()==before);
        ed->tabs.setCurrentTabIndex(1);ed->timerCallback();
        for(int i=0;i<10;++i){ed->editPage.slotButtons[i]->onClick();CHECK(ed->editPage.activeSlot==i);}
        for(int i=0;i<5;++i){ed->editPage.fx.pageButtons[i]->onClick();CHECK(ed->editPage.fx.activeEffect==i);}
        auto& k=ed->playPage.tilt.slider;k.setValue(2.,juce::sendNotificationSync);CHECK(std::abs(p.apvts.getRawParameterValue(ParamID::spTilt)->load()-2.f)<1e-6f);
        set(p,ParamID::spTilt,1.2f);pump();CHECK(std::abs(k.getValue()-1.2)<1e-5);
        // A recalled/cleared path invalidates a selection in the still-open editor.
        ed->morphPage.pad.selected=3; p.morph.numNodes.store(0);ed->morphPage.refresh();CHECK(ed->morphPage.pad.selected==-1);
        auto& pad=ed->playPage.morphPad;
        const auto point=pad.toScreen(.2f,.8f);const auto now=juce::Time::getCurrentTime();
        juce::MouseEvent click(juce::Desktop::getInstance().getMainMouseSource(),point,juce::ModifierKeys::leftButtonModifier,1,0,0,0,0,&pad,&pad,now,point,now,1,false);
        pad.mouseDown(click);pad.mouseUp(click);
        CHECK(std::abs(p.apvts.getRawParameterValue(ParamID::morphX)->load()-.2f)<1e-5f);CHECK(std::abs(p.apvts.getRawParameterValue(ParamID::morphY)->load()-.8f)<1e-5f);
        set(p,ParamID::morphX,.5f);set(p,ParamID::morphY,.5f);
        // Every parameter widget, including hidden ADVANCED pages, is driven
        // in both directions through its real JUCE attachment.
        std::set<juce::String> ids;
        std::set<juce::Component*> visited;
        std::ofstream inventory("control-inventory.tsv");inventory<<"parameter\twidget\tattachment\tPCM_per_control\tDAW_mouse\n";
        std::function<void(juce::Component&)> visit=[&](juce::Component& c)
        {
            if (!visited.insert(&c).second) return;
            auto id=c.getComponentID();
            if(id.isNotEmpty())
            {
                auto* param=p.apvts.getParameter(id);CHECK(param);if(!param)return;
                const float before=param->getValue();
                if(auto* s=dynamic_cast<juce::Slider*>(&c))
                {
                    s->setValue(s->getMinimum()+.63*(s->getMaximum()-s->getMinimum()),juce::sendNotificationSync);
                    CHECK(std::abs(p.apvts.getRawParameterValue(id)->load()-(float)s->getValue())<1e-4f);
                    param->setValueNotifyingHost(param->getDefaultValue());pump();
                    CHECK(std::abs(s->getValue()-p.apvts.getRawParameterValue(id)->load())<1e-4f);
                    inventory<<id<<"\tslider\tPASS\tNOT RUN\tNOT RUN\n";
                }
                else if(auto* combo=dynamic_cast<juce::ComboBox*>(&c))
                {
                    combo->setSelectedItemIndex((combo->getSelectedItemIndex()+1)%combo->getNumItems(),juce::sendNotificationSync);
                    CHECK((int)p.apvts.getRawParameterValue(id)->load()==combo->getSelectedItemIndex());
                    inventory<<id<<"\tchoice\tPASS\tNOT RUN\tNOT RUN\n";
                }
                else if(auto* button=dynamic_cast<juce::ToggleButton*>(&c))
                {
                    button->setToggleState(!button->getToggleState(),juce::sendNotificationSync);
                    CHECK((p.apvts.getRawParameterValue(id)->load()>.5f)==button->getToggleState());
                    inventory<<id<<"\ttoggle\tPASS\tNOT RUN\tNOT RUN\n";
                }
                ids.insert(id);param->setValueNotifyingHost(before);pump();
            }
            for(auto* child:c.getChildren())visit(*child);
        };
        visit(*ed);
        // JUCE detaches non-selected tab content from the component tree.
        visit(ed->playPage);visit(ed->editPage);visit(ed->modPage);visit(ed->morphPage);visit(ed->wavePage);
        // XY are custom mouse-driven parameters, tested separately below.
        for(auto* param:p.getParameters())
        {
            auto* ranged=dynamic_cast<juce::RangedAudioParameter*>(param);CHECK(ranged);
            if(ranged && ranged->paramID!=ParamID::morphX && ranged->paramID!=ParamID::morphY)CHECK(ids.count(ranged->paramID)>0);
        }
        std::cout<<"GUI bound parameter IDs: "<<ids.size()<<"\n";
    }
}
int main(int argc,char**argv)
{
    juce::ScopedJuceInitialiser_GUI juceInit;
    const std::string which=argc>1?argv[1]:"";
    if(which=="blocks") blocks();else if(which=="keys")keys();else if(which=="wet")wet();else if(which=="tempo")tempo();else if(which=="state")state();else if(which=="gui")gui();else if(which=="waves")waves();else if(which=="realtime")realtime();else if(which=="presets")presets();else if(which=="save")save_failure();else if(which=="midi")midi();else return 2;
    std::cout<<which<<": "<<(failures?"FAIL":"PASS")<<" ("<<failures<<" failed checks)\n";
    return failures?1:0;
}
