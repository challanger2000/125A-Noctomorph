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
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace VST3::Hosting;

namespace {

bool addParam(
    ParameterChanges& changes,
    ParamID id,
    int32 offset,
    ParamValue value) {

    int32 queueIndex = 0;
    int32 pointIndex = 0;
    auto* queue = changes.addParameterData(id, queueIndex);
    return queue &&
        queue->addPoint(offset, value, pointIndex) == kResultTrue;
}

Event noteOnEvent(int32 offset, int32 noteId, int16 pitch = 36) {
    Event e {};
    e.busIndex = 0;
    e.sampleOffset = offset;
    e.ppqPosition = 0.0;
    e.flags = Event::kIsLive;
    e.type = Event::kNoteOnEvent;
    e.noteOn.channel = 0;
    e.noteOn.pitch = pitch;
    e.noteOn.tuning = 0.0f;
    e.noteOn.velocity = 0.90f;
    e.noteOn.length = 0;
    e.noteOn.noteId = noteId;
    return e;
}

void writeU16(std::ofstream& out, std::uint16_t value) {
    const char b[2] {
        static_cast<char>(value & 0xFFu),
        static_cast<char>((value >> 8) & 0xFFu)
    };
    out.write(b, 2);
}

void writeU32(std::ofstream& out, std::uint32_t value) {
    const char b[4] {
        static_cast<char>(value & 0xFFu),
        static_cast<char>((value >> 8) & 0xFFu),
        static_cast<char>((value >> 16) & 0xFFu),
        static_cast<char>((value >> 24) & 0xFFu)
    };
    out.write(b, 4);
}

bool writePcm16Stereo(
    const std::filesystem::path& path,
    const std::vector<float>& left,
    const std::vector<float>& right,
    int sampleRate) {

    if (left.size() != right.size() || left.empty())
        return false;

    const std::uint64_t dataBytes64 =
        static_cast<std::uint64_t>(left.size()) * 2u * 2u;
    if (dataBytes64 > 0xFFFFFFFFu - 44u)
        return false;

    const auto dataBytes = static_cast<std::uint32_t>(dataBytes64);
    std::ofstream out(path, std::ios::binary);
    if (!out)
        return false;

    out.write("RIFF", 4);
    writeU32(out, 36u + dataBytes);
    out.write("WAVE", 4);
    out.write("fmt ", 4);
    writeU32(out, 16);
    writeU16(out, 1); // PCM
    writeU16(out, 2);
    writeU32(out, static_cast<std::uint32_t>(sampleRate));
    writeU32(out, static_cast<std::uint32_t>(sampleRate * 4));
    writeU16(out, 4);
    writeU16(out, 16);
    out.write("data", 4);
    writeU32(out, dataBytes);

    auto quantize = [](float x) -> std::int16_t {
        x = std::clamp(std::isfinite(x) ? x : 0.0f, -1.0f, 1.0f);
        const long q = std::lround(x * 32767.0f);
        return static_cast<std::int16_t>(
            std::clamp<long>(q, -32768, 32767));
    };

    for (std::size_t i = 0; i < left.size(); ++i) {
        const auto l = static_cast<std::uint16_t>(quantize(left[i]));
        const auto r = static_cast<std::uint16_t>(quantize(right[i]));
        writeU16(out, l);
        writeU16(out, r);
    }
    return static_cast<bool>(out);
}

struct Scene {
    const char* name;
    double archetype;
};

constexpr std::array<Scene, 6> kScenes {{
    {"void",       0.0},
    {"ruins",      0.2},
    {"industrial", 0.4},
    {"wasteland",  0.6},
    {"abyss",      0.8},
    {"nocturne",   1.0}
}};

bool renderScene(
    IComponent* component,
    IAudioProcessor* processor,
    const Scene& scene,
    const std::filesystem::path& output,
    double seconds) {

    constexpr double sampleRate = 48000.0;
    constexpr int32 blockSize = 257;

    ProcessSetup setup {};
    setup.processMode = kOffline;
    setup.symbolicSampleSize = kSample32;
    setup.maxSamplesPerBlock = blockSize;
    setup.sampleRate = sampleRate;

    if (processor->setupProcessing(setup) != kResultTrue ||
        component->setActive(true) != kResultTrue ||
        processor->setProcessing(true) != kResultTrue)
        return false;

    const std::size_t totalFrames =
        static_cast<std::size_t>(std::llround(seconds * sampleRate));
    std::vector<float> allL(totalFrames, 0.0f);
    std::vector<float> allR(totalFrames, 0.0f);
    std::vector<float> blockL(blockSize, 0.0f);
    std::vector<float> blockR(blockSize, 0.0f);

    Sample32* channels[2] {blockL.data(), blockR.data()};
    AudioBusBuffers out {};
    out.numChannels = 2;
    out.channelBuffers32 = channels;

    ParameterChanges initial(16);
    const std::array<std::pair<ParamID, ParamValue>, 11> values {{
        {3000, scene.archetype}, // ARCHETYPE
        {3001, 0.30}, // FOUNDATION
        {3002, 0.42}, // WORLD
        {3003, 0.34}, // TEXTURE
        {3004, 0.30}, // BODY
        {3005, 0.32}, // TENSION
        {3006, 0.45}, // EVOLVE
        {3007, 0.00}, // EVENTS: deliberately OFF
        {3008, 0.45}, // SPACE
        {3009, 0.50}, // OUTPUT
        {3010, 0.45}  // MOTION
    }};
    for (const auto& [id, value] : values) {
        if (!addParam(initial, id, 0, value))
            return false;
    }

    EventList noteOn(2);
    auto e = noteOnEvent(0, 9001);
    if (noteOn.addEvent(e) != kResultTrue)
        return false;

    ProcessData data {};
    data.processMode = kOffline;
    data.symbolicSampleSize = kSample32;
    data.numInputs = 0;
    data.numOutputs = 1;
    data.outputs = &out;
    data.inputParameterChanges = &initial;
    data.inputEvents = &noteOn;

    std::size_t done = 0;
    float peak = 0.0f;
    long double energy = 0.0;

    while (done < totalFrames) {
        const int32 n = static_cast<int32>(
            std::min<std::size_t>(blockSize, totalFrames - done));
        data.numSamples = n;

        if (done != 0) {
            data.inputParameterChanges = nullptr;
            data.inputEvents = nullptr;
        }

        std::fill(blockL.begin(), blockL.end(), 0.0f);
        std::fill(blockR.begin(), blockR.end(), 0.0f);

        if (processor->process(data) != kResultOk)
            return false;

        for (int32 i = 0; i < n; ++i) {
            const float l = blockL[static_cast<std::size_t>(i)];
            const float r = blockR[static_cast<std::size_t>(i)];
            if (!std::isfinite(l) || !std::isfinite(r))
                return false;

            allL[done + static_cast<std::size_t>(i)] = l;
            allR[done + static_cast<std::size_t>(i)] = r;
            peak = std::max(peak, std::max(std::fabs(l), std::fabs(r)));
            energy += static_cast<long double>(l) * l +
                      static_cast<long double>(r) * r;
        }
        done += static_cast<std::size_t>(n);
    }

    const double rms = std::sqrt(static_cast<double>(
        energy / static_cast<long double>(2 * totalFrames)));

    const bool stopOk =
        processor->setProcessing(false) == kResultTrue &&
        component->setActive(false) == kResultTrue;

    if (!stopOk || peak <= 1.0e-6f || rms <= 1.0e-7)
        return false;

    std::cout
        << scene.name
        << " seconds=" << seconds
        << " peak=" << peak
        << " rms=" << rms
        << " events_default=0"
        << "\n";

    return writePcm16Stereo(
        output, allL, allR, static_cast<int>(sampleRate));
}

int run(
    const std::string& pluginPath,
    const std::filesystem::path& outputDir,
    double seconds) {

    std::filesystem::create_directories(outputDir);

    std::string error;
    auto module = Module::create(pluginPath, error);
    if (!module) {
        std::cerr << "load failed: " << error << "\n";
        return 1;
    }

    HostApplication hostApplication;
    FUnknown* host = &hostApplication;
    auto factory = module->getFactory();
    factory.setHostContext(host);

    for (const auto& info : factory.classInfos()) {
        auto component = factory.createInstance<IComponent>(info.ID());
        if (!component)
            continue;
        if (component->initialize(host) != kResultOk) {
            component->terminate();
            continue;
        }

        IAudioProcessor* processor {};
        if (component->queryInterface(
                IAudioProcessor::iid,
                reinterpret_cast<void**>(&processor)) != kResultTrue ||
            !processor) {
            component->terminate();
            continue;
        }

        SpeakerArrangement stereo = SpeakerArr::kStereo;
        if (processor->setBusArrangements(
                nullptr, 0, &stereo, 1) != kResultTrue) {
            processor->release();
            component->terminate();
            return 2;
        }

        component->activateBus(kAudio, kOutput, 0, true);
        component->activateBus(kEvent, kInput, 0, true);

        for (const auto& scene : kScenes) {
            const auto output =
                outputDir / (std::string(scene.name) + "-default-120s.wav");
            if (!renderScene(
                    component.get(), processor, scene, output, seconds)) {
                std::cerr << "render failed: " << scene.name << "\n";
                processor->release();
                component->terminate();
                return 3;
            }
        }

        processor->release();
        component->terminate();
        std::cout << "Noctomorph embedded VST3 scene render PASS\n";
        return 0;
    }

    return 4;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 3 || argc > 4) {
        std::cerr
            << "usage: vst3_scene_render_probe PLUGIN.vst3 OUTPUT_DIR [seconds]\n";
        return 64;
    }

    const double seconds =
        argc == 4 ? std::max(10.0, std::stod(argv[3])) : 120.0;
    return run(argv[1], argv[2], seconds);
}
