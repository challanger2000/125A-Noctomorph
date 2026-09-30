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
    if(argc<6){std::cerr<<"usage: render_real_sources WORLD.raw TEXTURE.raw EVENT.raw seconds output.wav\n";return 2;}
    RawAudio w,t,e; if(!loadRaw(argv[1],w)||!loadRaw(argv[2],t)||!loadRaw(argv[3],e)){std::cerr<<"raw load failed\n";return 3;}
    const double sr=48000.0; const double seconds=std::clamp(std::stod(argv[4]),1.0,600.0); const std::size_t frames=static_cast<std::size_t>(sr*seconds);
    noctomorph::Clip wc{w.l.data(),w.r.data(),w.l.size(),double(w.sr),true};
    noctomorph::Clip tc{t.l.data(),t.r.data(),t.l.size(),double(t.sr),true};
    noctomorph::Clip ec{e.l.data(),e.r.data(),e.l.size(),double(e.sr),false};
    auto engine=std::make_unique<noctomorph::Engine>(); engine->prepare(sr); engine->reset(0x125A5245414C5352ULL);
    noctomorph::Parameters p; p.foundation=.42f;p.world=.52f;p.texture=.34f;p.body=.52f;p.tension=.44f;p.evolve=.72f;p.events=.22f;p.space=.46f;p.output=.50f;
    engine->setParameters(p); engine->setArchetype(noctomorph::Archetype::Industrial); engine->setWorldClip(&wc);engine->setTextureClip(&tc);engine->setEventClip(&ec);engine->noteOn(36,.9f);
    std::vector<float> l(frames),r(frames); constexpr std::size_t block=257; std::size_t off=0; float peak=0.0f; long double energy=0.0;
    while(off<frames){auto n=std::min(block,frames-off);engine->process(l.data()+off,r.data()+off,n);for(std::size_t i=off;i<off+n;++i){peak=std::max(peak,std::max(std::fabs(l[i]),std::fabs(r[i])));energy+=(long double)l[i]*l[i]+(long double)r[i]*r[i];}off+=n;}
    wav(argv[5],l,r,48000); const double rms=std::sqrt(double(energy/(2.0L*frames)));
    std::cout<<"real-source render seconds="<<seconds<<" peak="<<peak<<" rms="<<rms<<" events="<<engine->eventCount()<<"\n";
    return (!std::isfinite(peak)||!std::isfinite(rms)||peak>.892f||rms<1e-6)?4:0;
}
