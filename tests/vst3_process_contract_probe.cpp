#include "public.sdk/source/vst/hosting/module.h"
#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/hosting/parameterchanges.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/vstspeaker.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace VST3::Hosting;

namespace {

void trace(const std::string& s){ std::cout<<"[process-probe] "<<s<<std::endl; }
int fail(int code,const std::string& s){
    std::cerr<<"[process-probe] FAIL "<<code<<": "<<s<<std::endl;
    return code;
}

bool finiteBuffers(const std::vector<float>& l,const std::vector<float>& r){
    for(float v:l) if(!std::isfinite(v)) return false;
    for(float v:r) if(!std::isfinite(v)) return false;
    return true;
}

bool addParam(ParameterChanges& changes,ParamID id,int32 offset,ParamValue value){
    int32 qi=0,pi=0;
    auto* q=changes.addParameterData(id,qi);
    return q && q->addPoint(offset,value,pi)==kResultTrue;
}

Event noteEvent(bool on,int32 offset,int32 noteId,int16 pitch=48){
    Event e{};
    e.busIndex=0;
    e.sampleOffset=offset;
    e.ppqPosition=0.0;
    e.flags=Event::kIsLive;
    if(on){
        e.type=Event::kNoteOnEvent;
        e.noteOn.channel=0;
        e.noteOn.pitch=pitch;
        e.noteOn.tuning=0.0f;
        e.noteOn.velocity=0.8f;
        e.noteOn.length=0;
        e.noteOn.noteId=noteId;
    } else {
        e.type=Event::kNoteOffEvent;
        e.noteOff.channel=0;
        e.noteOff.pitch=pitch;
        e.noteOff.tuning=0.0f;
        e.noteOff.velocity=0.0f;
        e.noteOff.noteId=noteId;
    }
    return e;
}

bool runBlock(IComponent* component,IAudioProcessor* processor,
              ProcessModes mode,double sampleRate,int32 blockSize){
    ProcessSetup setup{};
    setup.processMode=mode;
    setup.symbolicSampleSize=kSample32;
    setup.maxSamplesPerBlock=blockSize;
    setup.sampleRate=sampleRate;
    if(processor->setupProcessing(setup)!=kResultTrue) return false;
    if(component->setActive(true)!=kResultTrue) return false;
    if(processor->setProcessing(true)!=kResultTrue){
        component->setActive(false);
        return false;
    }

    std::vector<float> left(static_cast<size_t>(blockSize),0.0f);
    std::vector<float> right(static_cast<size_t>(blockSize),0.0f);
    Sample32* channels[2]{left.data(),right.data()};
    AudioBusBuffers out{};
    out.numChannels=2;
    out.silenceFlags=0;
    out.channelBuffers32=channels;

    EventList events(8);
    auto on=noteEvent(true,0,12501);
    if(events.addEvent(on)!=kResultTrue) return false;

    ParameterChanges params(12);
    if(!addParam(params,3006,0,0.10)) return false; // EVOLVE
    if(blockSize>1 && !addParam(params,3006,blockSize-1,0.90)) return false;
    if(!addParam(params,3009,0,0.80)) return false; // OUTPUT
    if(!addParam(params,3005,0,std::numeric_limits<double>::quiet_NaN()))
        return false; // TENSION robustness
    if(!addParam(params,3010,0,0.75)) return false; // MOTION
    if(!addParam(params,3000,0,0.80)) return false; // ABYSS archetype

    ProcessData data{};
    data.processMode=mode;
    data.symbolicSampleSize=kSample32;
    data.numSamples=blockSize;
    data.numInputs=0;
    data.numOutputs=1;
    data.inputs=nullptr;
    data.outputs=&out;
    data.inputEvents=&events;
    data.inputParameterChanges=&params;

    if(processor->process(data)!=kResultOk || !finiteBuffers(left,right))
        return false;

    float energy=0.0f;
    for(size_t i=0;i<left.size();++i)
        energy+=std::fabs(left[i])+std::fabs(right[i]);
    if(blockSize>=64 && energy<=0.0f){
        std::cerr<<"[FAIL] NoteOn produced no output\n";
        return false;
    }

    std::fill(left.begin(),left.end(),0.0f);
    std::fill(right.begin(),right.end(),0.0f);
    EventList offs(4);
    auto off=noteEvent(false,0,12501);
    if(offs.addEvent(off)!=kResultTrue) return false;
    data.inputEvents=&offs;
    data.inputParameterChanges=nullptr;
    if(processor->process(data)!=kResultOk || !finiteBuffers(left,right))
        return false;

    ParameterChanges flush(2);
    if(!addParam(flush,3002,0,0.50)) return false;
    ProcessData zero{};
    zero.processMode=mode;
    zero.symbolicSampleSize=kSample32;
    zero.numSamples=0;
    zero.inputParameterChanges=&flush;
    if(processor->process(zero)!=kResultOk)
        return false;

    const bool stopOk=processor->setProcessing(false)==kResultTrue;
    const bool inactiveOk=component->setActive(false)==kResultTrue;
    return stopOk && inactiveOk;
}


double renderAssetOnly(
    IComponent* component,
    IAudioProcessor* processor,
    ParamID roleParam,
    double seconds) {

    ProcessSetup setup{};
    setup.processMode = kRealtime;
    setup.symbolicSampleSize = kSample32;
    setup.maxSamplesPerBlock = 256;
    setup.sampleRate = 48000.0;
    if (processor->setupProcessing(setup) != kResultTrue)
        return -1.0;
    if (component->setActive(true) != kResultTrue)
        return -1.0;
    if (processor->setProcessing(true) != kResultTrue) {
        component->setActive(false);
        return -1.0;
    }

    std::vector<float> left(256, 0.0f);
    std::vector<float> right(256, 0.0f);
    Sample32* channels[2] {left.data(), right.data()};
    AudioBusBuffers out{};
    out.numChannels = 2;
    out.channelBuffers32 = channels;

    ParameterChanges initial(16);
    const std::array<std::pair<ParamID, ParamValue>, 11> values {{
        {3000, 0.4}, // INDUSTRIAL
        {3001, 0.0}, // FOUNDATION
        {3002, roleParam == 3002 ? 1.0 : 0.0}, // WORLD
        {3003, roleParam == 3003 ? 1.0 : 0.0}, // TEXTURE
        {3004, 0.0}, // BODY
        {3005, 0.0}, // TENSION
        {3006, 0.35}, // EVOLVE
        {3007, roleParam == 3007 ? 1.0 : 0.0}, // EVENTS
        {3008, 0.0}, // SPACE
        {3009, 0.5}, // OUTPUT
        {3010, 0.35}  // MOTION
    }};
    for (const auto& [id, value] : values) {
        if (!addParam(initial, id, 0, value)) {
            processor->setProcessing(false);
            component->setActive(false);
            return -1.0;
        }
    }

    EventList onEvents(2);
    auto on = noteEvent(true, 0, 7001);
    if (onEvents.addEvent(on) != kResultTrue) {
        processor->setProcessing(false);
        component->setActive(false);
        return -1.0;
    }

    ProcessData data{};
    data.processMode = kRealtime;
    data.symbolicSampleSize = kSample32;
    data.numInputs = 0;
    data.numOutputs = 1;
    data.inputs = nullptr;
    data.outputs = &out;
    data.inputEvents = &onEvents;
    data.inputParameterChanges = &initial;

    const std::size_t totalFrames =
        static_cast<std::size_t>(std::llround(seconds * 48000.0));
    std::size_t rendered = 0;
    long double energy = 0.0;

    while (rendered < totalFrames) {
        const int32 n = static_cast<int32>(std::min<std::size_t>(
            left.size(), totalFrames - rendered));
        data.numSamples = n;

        if (rendered != 0) {
            data.inputEvents = nullptr;
            data.inputParameterChanges = nullptr;
        }

        std::fill(left.begin(), left.end(), 0.0f);
        std::fill(right.begin(), right.end(), 0.0f);

        if (processor->process(data) != kResultOk ||
            !finiteBuffers(left, right)) {
            processor->setProcessing(false);
            component->setActive(false);
            return -1.0;
        }

        for (int32 i = 0; i < n; ++i) {
            energy +=
                static_cast<long double>(left[static_cast<std::size_t>(i)]) *
                left[static_cast<std::size_t>(i)];
            energy +=
                static_cast<long double>(right[static_cast<std::size_t>(i)]) *
                right[static_cast<std::size_t>(i)];
        }
        rendered += static_cast<std::size_t>(n);
    }

    processor->setProcessing(false);
    component->setActive(false);

    return std::sqrt(static_cast<double>(
        energy / static_cast<long double>(2 * std::max<std::size_t>(1, totalFrames))));
}

int run(const std::string& path){
    trace("start: "+path);
    std::string error;
    auto module=Module::create(path,error);
    if(!module) return fail(1,"module load: "+error);

    HostApplication hostApplication;
    FUnknown* host=&hostApplication;
    auto factory=module->getFactory();
    factory.setHostContext(host);

    for(const auto& info:factory.classInfos()){
        auto component=factory.createInstance<IComponent>(info.ID());
        if(!component) continue;
        if(component->initialize(host)!=kResultOk){
            component->terminate();
            continue;
        }

        IAudioProcessor* processor{};
        if(component->queryInterface(
            IAudioProcessor::iid,reinterpret_cast<void**>(&processor))!=kResultTrue ||
            !processor){
            component->terminate();
            continue;
        }

        if(processor->canProcessSampleSize(kSample32)!=kResultTrue)
            return fail(2,"32-bit unsupported");
        if(processor->canProcessSampleSize(kSample64)==kResultTrue)
            return fail(3,"unexpected 64-bit support");

        if(component->getBusCount(kAudio,kInput)!=0 ||
           component->getBusCount(kAudio,kOutput)!=1 ||
           component->getBusCount(kEvent,kInput)!=1)
            return fail(4,"bus topology");

        SpeakerArrangement stereo=SpeakerArr::kStereo;
        if(processor->setBusArrangements(nullptr,0,&stereo,1)!=kResultTrue)
            return fail(5,"stereo arrangement");

        component->activateBus(kAudio,kOutput,0,true);
        component->activateBus(kEvent,kInput,0,true);

        for(auto mode:{kRealtime,kOffline}){
            for(double sr:{44100.0,48000.0,96000.0,192000.0}){
                for(int32 block:{1,16,64,257,1024}){
                    trace("matrix mode="+std::to_string(static_cast<int>(mode))+
                          " sr="+std::to_string(static_cast<int>(sr))+
                          " block="+std::to_string(block));
                    if(!runBlock(component.get(),processor,mode,sr,block)){
                        processor->release();
                        component->terminate();
                        return fail(6,"process matrix");
                    }
                }
            }
        }

        const double worldRms = renderAssetOnly(
            component.get(), processor, 3002, 3.0);
        const double textureRms = renderAssetOnly(
            component.get(), processor, 3003, 3.0);
        const double eventRms = renderAssetOnly(
            component.get(), processor, 3007, 12.0);

        trace("asset WORLD-only rms=" + std::to_string(worldRms));
        trace("asset TEXTURE-only rms=" + std::to_string(textureRms));
        trace("asset EVENT-only rms=" + std::to_string(eventRms));

        if (!(worldRms > 1.0e-5))
            return fail(7, "embedded WORLD asset not active");
        if (!(textureRms > 1.0e-5))
            return fail(8, "embedded TEXTURE asset not active");
        if (!(eventRms > 1.0e-7))
            return fail(9, "embedded EVENT asset not active");

        ProcessSetup finalSetup{};
        finalSetup.processMode=kRealtime;
        finalSetup.symbolicSampleSize=kSample32;
        finalSetup.maxSamplesPerBlock=128;
        finalSetup.sampleRate=48000.0;
        if(processor->setupProcessing(finalSetup)!=kResultTrue)
            return fail(10,"final setup");

        for(int i=0;i<8;++i){
            if(component->setActive(true)!=kResultTrue)
                return fail(11,"setActive true");
            if(processor->setProcessing(true)!=kResultTrue)
                return fail(12,"setProcessing true");
            if(processor->setProcessing(false)!=kResultTrue)
                return fail(13,"setProcessing false");
            if(component->setActive(false)!=kResultTrue)
                return fail(14,"setActive false");
        }

        processor->release();
        if(component->terminate()!=kResultOk)
            return fail(15,"terminate");

        std::cout
            <<"Noctomorph VST3 process contract PASS: realtime/offline, "
            <<"4 rates, 5 block sizes, MIDI, automation, NaN, zero-flush, "
            <<"activate/deactivate, MOTION, embedded WORLD/TEXTURE/EVENT assets\n";
        return 0;
    }

    return fail(16,"no audio processor class");
}

} // namespace

int main(int argc,char** argv){
    if(argc!=2) return 64;
    return run(argv[1]);
}
