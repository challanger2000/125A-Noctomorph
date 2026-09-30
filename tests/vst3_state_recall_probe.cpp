#include "base/source/fstreamer.h"
#include "public.sdk/source/common/memorystream.h"
#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst/hosting/module.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"

#include <array>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace VST3::Hosting;

namespace {

constexpr int32 kMagic = 0x31434F4E; // NOC1
constexpr int32 kVersion = 1;
constexpr std::array<ParamID,10> kIds{
    3000,3001,3002,3003,3004,3005,3006,3007,3008,3009
};

bool nearly(double a,double b){ return std::fabs(a-b)<1.0e-6; }

bool writeState(MemoryStream& ms,int32 magic,int32 version,
                const std::array<float,10>& values){
    IBStreamer s(&ms,kLittleEndian);
    if(!s.writeInt32(magic)||!s.writeInt32(version)) return false;
    for(float v:values) if(!s.writeFloat(v)) return false;
    ms.seek(0,IBStream::kIBSeekSet,nullptr);
    return true;
}

bool readState(IComponent* component,std::array<float,10>& values){
    MemoryStream ms;
    if(component->getState(&ms)!=kResultTrue) return false;
    if(ms.getSize()!=48) return false;
    ms.seek(0,IBStream::kIBSeekSet,nullptr);
    IBStreamer s(&ms,kLittleEndian);
    int32 magic=0,version=0;
    if(!s.readInt32(magic)||magic!=kMagic) return false;
    if(!s.readInt32(version)||version!=kVersion) return false;
    for(float& v:values)
        if(!s.readFloat(v)||!std::isfinite(v)||v<0.f||v>1.f) return false;
    return true;
}

bool same(const std::array<float,10>& a,const std::array<float,10>& b){
    for(size_t i=0;i<a.size();++i) if(!nearly(a[i],b[i])) return false;
    return true;
}

int run(const std::string& path){
    std::string error;
    auto module=Module::create(path,error);
    if(!module){ std::cerr<<"[FAIL] load: "<<error<<"\n"; return 1; }

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

        IAudioProcessor* audio{};
        if(component->queryInterface(IAudioProcessor::iid,
            reinterpret_cast<void**>(&audio))!=kResultTrue || !audio){
            component->terminate();
            continue;
        }
        audio->release();

        const std::array<float,10> expectedDefaults{
            0.0f,0.50f,0.35f,0.25f,0.35f,0.25f,0.35f,0.18f,0.35f,0.50f
        };
        std::array<float,10> defaults{};
        if(!readState(component.get(),defaults) || !same(defaults,expectedDefaults)){
            std::cerr<<"[FAIL] default state mismatch\n"; return 2;
        }

        TUID controllerCid{};
        if(component->getControllerClassId(controllerCid)!=kResultTrue)
            return 3;
        auto controller=factory.createInstance<IEditController>(VST3::UID(controllerCid));
        if(!controller || controller->initialize(host)!=kResultOk)
            return 4;

        const std::array<float,10> controllerValues{
            0.8f,0.11f,0.22f,0.33f,0.44f,0.55f,0.66f,0.27f,0.78f,0.89f
        };
        MemoryStream controllerState;
        if(!writeState(controllerState,kMagic,kVersion,controllerValues) ||
           controller->setComponentState(&controllerState)!=kResultTrue)
            return 5;

        for(size_t i=0;i<kIds.size();++i){
            const double actual=controller->getParamNormalized(kIds[i]);
            if(!nearly(actual,controllerValues[i])){
                std::cerr<<"[FAIL] controller param "<<kIds[i]
                         <<" expected "<<controllerValues[i]
                         <<" got "<<actual<<"\n";
                return 6;
            }
        }
        controller->terminate();
        controller.reset();

        const std::array<float,10> custom{
            0.6f,0.12f,0.23f,0.34f,0.45f,0.56f,0.67f,0.28f,0.79f,0.90f
        };
        MemoryStream customState;
        if(!writeState(customState,kMagic,kVersion,custom) ||
           component->setState(&customState)!=kResultTrue)
            return 7;

        std::array<float,10> roundtrip{};
        if(!readState(component.get(),roundtrip) || !same(roundtrip,custom)){
            std::cerr<<"[FAIL] state roundtrip mismatch\n"; return 8;
        }

        auto expectRejectWithoutMutation=[&](int32 magic,int32 version,
                                              std::array<float,10> values,
                                              const char* label)->bool{
            MemoryStream invalid;
            if(!writeState(invalid,magic,version,values)) return false;
            if(component->setState(&invalid)==kResultTrue){
                std::cerr<<"[FAIL] "<<label<<" accepted\n"; return false;
            }
            std::array<float,10> after{};
            if(!readState(component.get(),after) || !same(after,custom)){
                std::cerr<<"[FAIL] "<<label<<" mutated valid state\n"; return false;
            }
            return true;
        };

        auto nanValues=custom;
        nanValues[4]=std::numeric_limits<float>::quiet_NaN();
        if(!expectRejectWithoutMutation(kMagic,kVersion,nanValues,"NaN"))
            return 9;

        auto rangeValues=custom;
        rangeValues[8]=1.25f;
        if(!expectRejectWithoutMutation(kMagic,kVersion,rangeValues,"out-of-range"))
            return 10;

        if(!expectRejectWithoutMutation(0x12345678,kVersion,custom,"bad magic"))
            return 11;
        if(!expectRejectWithoutMutation(kMagic,99,custom,"bad version"))
            return 12;

        if(component->terminate()!=kResultOk) return 13;
        std::cout<<"Noctomorph state/recall contract PASS\n";
        return 0;
    }

    std::cerr<<"[FAIL] no audio processor class\n";
    return 14;
}

} // namespace

int main(int argc,char** argv){
    if(argc!=2) return 64;
    return run(argv[1]);
}
