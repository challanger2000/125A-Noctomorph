#include "NoctomorphCore.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
struct RawAudio { std::uint32_t sr=0; std::vector<float> l,r; };

template <class T> bool readValue(std::ifstream& in,T& v){ return static_cast<bool>(in.read(reinterpret_cast<char*>(&v),sizeof(T))); }

bool loadRaw(const char* path, RawAudio& a){
    std::ifstream in(path,std::ios::binary); if(!in) return false;
    char magic[8]{}; if(!in.read(magic,8) || std::string(magic,8)!="NOMORAW1") return false;
    std::uint32_t channels=0; std::uint64_t frames=0;
    if(!readValue(in,a.sr)||!readValue(in,channels)||!readValue(in,frames)||channels!=2||frames==0) return false;
    if(frames>100000000ULL) return false;
    a.l.resize(static_cast<std::size_t>(frames)); a.r.resize(static_cast<std::size_t>(frames));
    if(!in.read(reinterpret_cast<char*>(a.l.data()),static_cast<std::streamsize>(a.l.size()*sizeof(float)))) return false;
    if(!in.read(reinterpret_cast<char*>(a.r.data()),static_cast<std::streamsize>(a.r.size()*sizeof(float)))) return false;
    return true;
}
void u16(std::ofstream& o,std::uint16_t v){o.put(char(v&255));o.put(char((v>>8)&255));}
void u32(std::ofstream& o,std::uint32_t v){for(int i=0;i<4;++i)o.put(char((v>>(8*i))&255));}
void wav(const char* path,const std::vector<float>& l,const std::vector<float>& r,std::uint32_t sr){
    const auto frames=static_cast<std::uint32_t>(std::min(l.size(),r.size())); const std::uint32_t data=frames*4;
    std::ofstream o(path,std::ios::binary); o.write("RIFF",4);u32(o,36+data);o.write("WAVEfmt ",8);u32(o,16);u16(o,1);u16(o,2);u32(o,sr);u32(o,sr*4);u16(o,4);u16(o,16);o.write("data",4);u32(o,data);
    for(std::uint32_t i=0;i<frames;++i)for(float x:{l[i],r[i]}){x=std::clamp(x,-1.0f,1.0f);u16(o,static_cast<std::uint16_t>(static_cast<std::int16_t>(std::lround(x*32767.0f))));}
}
}

int main(int argc,char** argv){
    if(argc<6){
        std::cerr<<"usage (legacy): render_real_sources WORLD.raw TEXTURE.raw EVENT.raw seconds output.wav [profile]\n"
                 <<"usage (body):   render_real_sources WORLD.raw TEXTURE.raw BODY.raw EVENT.raw seconds output.wav [profile]\n";
        return 2;
    }

    auto isNumber = [](const char* text) {
        if (!text || !*text) return false;
        char* end = nullptr;
        std::strtod(text, &end);
        return end && *end == '\0';
    };

    const bool bodyLayout = argc >= 7 && !isNumber(argv[4]);

    const char* worldPath = argv[1];
    const char* texturePath = argv[2];
    const char* bodyPath = bodyLayout ? argv[3] : argv[3];
    const char* eventPath = bodyLayout ? argv[4] : argv[3];
    const char* secondsText = bodyLayout ? argv[5] : argv[4];
    const char* outputPath = bodyLayout ? argv[6] : argv[5];
    const char* profileText =
        bodyLayout ? (argc >= 8 ? argv[7] : "industrial")
                   : (argc >= 7 ? argv[6] : "industrial");

    RawAudio w,t,b,e;
    if(!loadRaw(worldPath,w)||!loadRaw(texturePath,t)||!loadRaw(eventPath,e)){
        std::cerr<<"raw load failed\n";
        return 3;
    }

    if (bodyLayout) {
        if (!loadRaw(bodyPath,b)) {
            std::cerr<<"body raw load failed\n";
            return 3;
        }
    } else {
        b = e;
    }

    const double sr=48000.0;
    const double seconds=std::clamp(std::stod(secondsText),1.0,600.0);
    const std::size_t frames=static_cast<std::size_t>(sr*seconds);
    noctomorph::Clip wc{w.l.data(),w.r.data(),w.l.size(),double(w.sr),true};
    noctomorph::Clip tc{t.l.data(),t.r.data(),t.l.size(),double(t.sr),true};
    noctomorph::Clip bc{b.l.data(),b.r.data(),b.l.size(),double(b.sr),false};
    noctomorph::Clip ec{e.l.data(),e.r.data(),e.l.size(),double(e.sr),false};
    auto engine=std::make_unique<noctomorph::Engine>(); engine->prepare(sr); engine->reset(0x125A5245414C5352ULL);
    noctomorph::Parameters p;
    noctomorph::Archetype archetype = noctomorph::Archetype::Industrial;
    std::string profile = profileText;

    if (profile == "source-only") {
        p.foundation=.0f; p.world=.74f; p.texture=.58f; p.body=.0f;
        p.tension=.0f; p.evolve=.25f; p.events=.0f; p.space=.0f; p.output=.48f;
    } else if (profile == "foundation-only") {
        p.foundation=.55f; p.world=.0f; p.texture=.0f; p.body=.0f;
        p.tension=.48f; p.evolve=.72f; p.events=.0f; p.space=.0f; p.output=.48f;
    } else if (profile == "body-only") {
        p.foundation=.0f; p.world=.0f; p.texture=.0f; p.body=.55f;
        p.tension=.48f; p.evolve=.72f; p.events=.18f; p.space=.0f; p.output=.48f;
    } else if (profile == "world-texture-only") {
        p.foundation=.0f; p.world=.74f; p.texture=.58f; p.body=.0f;
        p.tension=.0f; p.evolve=.72f; p.events=.0f; p.space=.0f; p.output=.48f;
    } else if (profile == "ruins") {
        archetype = noctomorph::Archetype::Ruins;
        p.foundation=.14f; p.world=.84f; p.texture=.50f; p.body=.34f;
        p.tension=.40f; p.evolve=.58f; p.events=.10f; p.space=.16f; p.output=.48f;
    } else if (profile == "nocturne") {
        archetype = noctomorph::Archetype::Nocturne;
        p.foundation=.20f; p.world=.70f; p.texture=.48f; p.body=.30f;
        p.tension=.52f; p.evolve=.68f; p.events=.08f; p.space=.20f; p.output=.48f;
    } else if (profile == "abyss") {
        archetype = noctomorph::Archetype::Abyss;
        p.foundation=.28f; p.world=.58f; p.texture=.40f; p.body=.50f;
        p.tension=.72f; p.evolve=.82f; p.events=.12f; p.space=.24f; p.output=.46f;
    } else if (profile == "wasteland") {
        archetype = noctomorph::Archetype::Wasteland;
        p.foundation=.18f; p.world=.78f; p.texture=.64f; p.body=.26f;
        p.tension=.46f; p.evolve=.74f; p.events=.14f; p.space=.22f; p.output=.48f;
    } else if (profile == "void") {
        archetype = noctomorph::Archetype::Void;
        p.foundation=.42f; p.world=.24f; p.texture=.18f; p.body=.52f;
        p.tension=.64f; p.evolve=.54f; p.events=.04f; p.space=.44f; p.output=.46f;
    } else {
        p.foundation=.22f; p.world=.74f; p.texture=.58f; p.body=.30f;
        p.tension=.48f; p.evolve=.72f; p.events=.18f; p.space=.42f; p.output=.48f;
    }

    // Calibrate source excitation, not the modal-bank output.
    // Deep gong-like exciters inject far more low modal energy than
    // broadband transient exciters such as glass.
    if (profile == "abyss" || profile == "void")
        bc.excitationGain = 0.075f;
    else
        bc.excitationGain = 1.0f;

    engine->setParameters(p);
    engine->setArchetype(archetype);
    engine->setWorldClip(&wc);
    engine->setTextureClip(&tc);
    engine->setEventClip(&ec);
    if (profile != "source-only")
        engine->setBodyExciterClip(&bc);
    engine->noteOn(36,.9f);
    std::vector<float> l(frames),r(frames); constexpr std::size_t block=257; std::size_t off=0; float peak=0.0f; long double energy=0.0;
    while(off<frames){auto n=std::min(block,frames-off);engine->process(l.data()+off,r.data()+off,n);for(std::size_t i=off;i<off+n;++i){peak=std::max(peak,std::max(std::fabs(l[i]),std::fabs(r[i])));energy+=(long double)l[i]*l[i]+(long double)r[i]*r[i];}off+=n;}
    wav(argv[6],l,r,48000); const double rms=std::sqrt(double(energy/(2.0L*frames)));
    std::cout<<"real-source render profile="<<profile<<" seconds="<<seconds<<" peak="<<peak<<" rms="<<rms<<" events="<<engine->eventCount()<<"\n";
    return (!std::isfinite(peak)||!std::isfinite(rms)||peak>.892f||rms<1e-6)?4:0;
}
