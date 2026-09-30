#include "base/source/fstreamer.h"
#include "public.sdk/source/common/memorystream.h"
#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst/hosting/module.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
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

constexpr int32 kMagic = 0x31434F4E;
constexpr int32 kVersion = 2;
constexpr int32 kLegacyVersion = 1;
constexpr std::array<ParamID,11> kIds{
    3000,3001,3002,3003,3004,3005,3006,3007,3008,3009,3010
};

bool nearly(double a,double b){ return std::fabs(a-b)<1.0e-6; }

template <size_t N>
bool writeState(MemoryStream& ms,int32 magic,int32 version,
                const std::array<float,N>& values){
    IBStreamer s(&ms,kLittleEndian);
    if(!s.writeInt32(magic)||!s.writeInt32(version)) return false;
    for(float v:values) if(!s.writeFloat(v)) return false;
    ms.seek(0,IBStream::kIBSeekSet,nullptr);
    return true;
}

bool readCurrentState(IComponent* component,std::array<float,11>& values){
    MemoryStream ms;
    if(component->getState(&ms)!=kResultTrue) return false;
    if(ms.getSize()!=52) return false;
    ms.seek(0,IBStream::kIBSeekSet,nullptr);
    IBStreamer s(&ms,kLittleEndian);
    int32 magic=0,version=0;
    if(!s.readInt32(magic)||magic!=kMagic) return false;
    if(!s.readInt32(version)||version!=kVersion) return false;
    for(float& v:values)
        if(!s.readFloat(v)||!std::isfinite(v)||v<0.f||v>1.f) return false;
    return true;
}

template <size_t N>
bool samePrefix(const std::array<float,11>& current,
                const std::array<float,N>& expected){
    for(size_t i=0;i<N;++i)
        if(!nearly(current[i],expected[i])) return false;
    return true;
}

bool same(const std::array<float,11>& a,const std::array<float,11>& b){
    return samePrefix(a,b);
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

        const std::array<float,11> expectedDefaults{
            0.0f,0.30f,0.42f,0.34f,0.30f,0.32f,0.45f,0.00f,0.45f,0.50f,0.45f
        };
        std::array<float,11> defaults{};
        if(!readCurrentState(component.get(),defaults) || !same(defaults,expectedDefaults)){
            std::cerr<<"[FAIL] default state mismatch\n"; return 2;
        }

        TUID controllerCid{};
        if(component->getControllerClassId(controllerCid)!=kResultTrue)
            return 3;
        auto controller=factory.createInstance<IEditController>(VST3::UID(controllerCid));
        if(!controller || controller->initialize(host)!=kResultOk)
            return 4;

        const std::array<float,11> controllerValues{
            0.8f,0.11f,0.22f,0.33f,0.44f,0.55f,0.66f,0.27f,0.78f,0.89f,0.41f
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

        const std::array<float,11> custom{
            0.6f,0.12f,0.23f,0.34f,0.45f,0.56f,0.67f,0.28f,0.79f,0.90f,0.42f
        };
        MemoryStream customState;
        if(!writeState(customState,kMagic,kVersion,custom) ||
           component->setState(&customState)!=kResultTrue)
            return 7;

        std::array<float,11> roundtrip{};
        if(!readCurrentState(component.get(),roundtrip) || !same(roundtrip,custom)){
            std::cerr<<"[FAIL] state roundtrip mismatch\n"; return 8;
        }

        const std::array<float,10> legacy{
            0.4f,0.15f,0.25f,0.35f,0.45f,0.55f,0.65f,0.20f,0.75f,0.85f
        };
        MemoryStream legacyState;
        if(!writeState(legacyState,kMagic,kLegacyVersion,legacy) ||
           component->setState(&legacyState)!=kResultTrue)
            return 9;

        std::array<float,11> migrated{};
        if(!readCurrentState(component.get(),migrated) ||
           !samePrefix(migrated,legacy) || !nearly(migrated[10],0.35)){
            std::cerr<<"[FAIL] legacy v1 migration mismatch\n"; return 10;
        }

        MemoryStream restoreCustom;
        if(!writeState(restoreCustom,kMagic,kVersion,custom) ||
           component->setState(&restoreCustom)!=kResultTrue)
            return 11;

        auto expectRejectWithoutMutation=[&](int32 magic,int32 version,
                                              std::array<float,11> values,
                                              const char* label)->bool{
            MemoryStream invalid;
            if(!writeState(invalid,magic,version,values)) return false;
            if(component->setState(&invalid)==kResultTrue){
                std::cerr<<"[FAIL] "<<label<<" accepted\n"; return false;
            }
            std::array<float,11> after{};
            if(!readCurrentState(component.get(),after) || !same(after,custom)){
                std::cerr<<"[FAIL] "<<label<<" mutated valid state\n"; return false;
            }
            return true;
        };

        auto nanValues=custom;
        nanValues[4]=std::numeric_limits<float>::quiet_NaN();
        if(!expectRejectWithoutMutation(kMagic,kVersion,nanValues,"NaN"))
            return 12;

        auto rangeValues=custom;
        rangeValues[10]=1.25f;
        if(!expectRejectWithoutMutation(kMagic,kVersion,rangeValues,"out-of-range"))
            return 13;

        if(!expectRejectWithoutMutation(0x12345678,kVersion,custom,"bad magic"))
            return 14;
        if(!expectRejectWithoutMutation(kMagic,99,custom,"bad version"))
            return 15;

        if(component->terminate()!=kResultOk) return 16;
        std::cout<<"Noctomorph state/recall contract PASS incl. v1->v2 MOTION migration\n";
        return 0;
    }

    std::cerr<<"[FAIL] no audio processor class\n";
    return 17;
}

} // namespace

int main(int argc,char** argv){
    if(argc!=2) return 64;
    return run(argv[1]);
}
