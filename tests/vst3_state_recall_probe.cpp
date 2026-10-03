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
#include <string>

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace VST3::Hosting;

namespace {

constexpr int32 kMagic = 0x31434F4E;
constexpr int32 kVersion = 3;
constexpr int32 kLegacyVersion2 = 2;
constexpr std::array<ParamID,2> kIds{3000,3001};

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

bool readCurrentState(IComponent* component,std::array<float,2>& values){
    MemoryStream ms;
    if(component->getState(&ms)!=kResultTrue) return false;
    if(ms.getSize()!=16) return false;
    ms.seek(0,IBStream::kIBSeekSet,nullptr);
    IBStreamer s(&ms,kLittleEndian);
    int32 magic=0,version=0;
    if(!s.readInt32(magic)||magic!=kMagic) return false;
    if(!s.readInt32(version)||version!=kVersion) return false;
    for(float& v:values)
        if(!s.readFloat(v)||!std::isfinite(v)||v<0.f||v>1.f) return false;
    return true;
}

bool same(const std::array<float,2>& a,const std::array<float,2>& b){
    return nearly(a[0],b[0]) && nearly(a[1],b[1]);
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

        const std::array<float,2> expectedDefaults{0.173f,0.50f};
        std::array<float,2> defaults{};
        if(!readCurrentState(component.get(),defaults) || !same(defaults,expectedDefaults)){
            std::cerr<<"[FAIL] default state mismatch\n"; return 2;
        }

        TUID controllerCid{};
        if(component->getControllerClassId(controllerCid)!=kResultTrue)
            return 3;
        auto controller=factory.createInstance<IEditController>(VST3::UID(controllerCid));
        if(!controller || controller->initialize(host)!=kResultOk)
            return 4;

        const std::array<float,2> controllerValues{0.81f,0.67f};
        MemoryStream controllerState;
        if(!writeState(controllerState,kMagic,kVersion,controllerValues) ||
           controller->setComponentState(&controllerState)!=kResultTrue)
            return 5;
        // Random/DNA is retained only for state compatibility and is no
        // longer exposed by the one-knob controller.
        if(controller->getParameterObject(kIds[0]) != nullptr) return 6;
        const double intensityActual=controller->getParamNormalized(kIds[1]);
        if(!nearly(intensityActual,controllerValues[1])) return 6;
        controller->terminate();
        controller.reset();

        const std::array<float,2> custom{0.63f,0.74f};
        MemoryStream customState;
        if(!writeState(customState,kMagic,kVersion,custom) ||
           component->setState(&customState)!=kResultTrue)
            return 7;
        std::array<float,2> roundtrip{};
        if(!readCurrentState(component.get(),roundtrip) || !same(roundtrip,custom))
            return 8;

        const std::array<float,11> legacyV2{
            0.41f,0.58f,0.2f,0.3f,0.4f,0.5f,0.6f,0.0f,0.4f,0.5f,0.3f
        };
        MemoryStream legacyState;
        if(!writeState(legacyState,kMagic,kLegacyVersion2,legacyV2) ||
           component->setState(&legacyState)!=kResultTrue)
            return 9;
        std::array<float,2> migrated{};
        const std::array<float,2> expectedMigrated{0.41f,0.58f};
        if(!readCurrentState(component.get(),migrated) || !same(migrated,expectedMigrated))
            return 10;

        MemoryStream restoreCustom;
        if(!writeState(restoreCustom,kMagic,kVersion,custom) ||
           component->setState(&restoreCustom)!=kResultTrue)
            return 11;

        if(component->terminate()!=kResultOk) return 16;
        std::cout<<"Noctomorph state/recall contract PASS incl. v2->v3 macro migration\n";
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
