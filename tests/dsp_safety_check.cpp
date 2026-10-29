#include "dsp/Engine.h"
#include "dsp/Morph.h"
#include "ui/VersionCompare.h"
#include <iostream>
#include <limits>
#include <memory>

int main(int argc,char** argv)
{
    const std::string which=argc>1?argv[1]:"";
    if(which=="version")
    {
        if(v4jb::versionNewer("999999999999999999999999999999.1","0.0.4")) return 1;
    }
    else if(which=="preroll")
    {
        v4jb::Morph morph;morph.numNodes.store(2);morph.nodeLen[0].store(2);morph.nodeLen[1].store(2);
        morph.nodeX[0].store(0);morph.nodeX[1].store(1);
        morph.advance(v4jb::kPathSync,0,0,1,0,-2.,0.01f,false);
        // Loop length 8 beats: -2 beats == +6 beats, halfway on segment 1 -> 0.
        if(std::abs(morph.outX.load()-.5f)>1e-6f) return 1;
    }
    else if(which=="ppq")
    {
        v4jb::ModuleContext c;c.n=8;c.beats=1e300;c.dt=.01f;
        v4jb::ModuleParams p;p.type=(int)v4jb::ModuleType::Phrase;
        v4jb::ModuleState st;float freq[8]{},amp[8]{1,1,1,1,1,1,1,1};
        v4jb::processModule(p,st,c,freq,amp);
        for(float a:amp)if(!std::isfinite(a))return 1;
    }
    else if(which=="voices")
    {
        auto engine=std::make_unique<v4jb::Engine>();engine->prepare(48000);
        v4jb::EngineParams params;params.env[0].r=.001f;
        float macros[v4jb::kNumMacros];std::fill(std::begin(macros),std::end(macros),.5f);
        engine->setParams(params,macros);engine->noteOn(48,.8f);engine->noteOn(55,.8f);
        float l[64],r[64];engine->render(l,r,64);
        params.poly=1;engine->setParams(params,macros);engine->noteOff(48);engine->noteOff(55);
        for(int i=0;i<v4jb::kMaxVoices;++i)if(engine->voiceHeld(i))return 1;
    }
    else return 2;
    std::cout<<which<<" PASS\n";
}
