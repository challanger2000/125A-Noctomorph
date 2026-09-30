#include "NoctomorphCore.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace {

void writeU16(std::ofstream& out, std::uint16_t v) {
    out.put(static_cast<char>(v & 0xFF));
    out.put(static_cast<char>((v >> 8) & 0xFF));
}

void writeU32(std::ofstream& out, std::uint32_t v) {
    out.put(static_cast<char>(v & 0xFF));
    out.put(static_cast<char>((v >> 8) & 0xFF));
    out.put(static_cast<char>((v >> 16) & 0xFF));
    out.put(static_cast<char>((v >> 24) & 0xFF));
}

void writeWav(
    const char* path,
    const std::vector<float>& left,
    const std::vector<float>& right,
    std::uint32_t sampleRate) {

    const std::uint32_t frames = static_cast<std::uint32_t>(std::min(left.size(), right.size()));
    const std::uint16_t channels = 2;
    const std::uint16_t bits = 16;
    const std::uint32_t bytesPerSample = bits / 8;
    const std::uint32_t dataSize = frames * channels * bytesPerSample;

    std::ofstream out(path, std::ios::binary);
    out.write("RIFF", 4);
    writeU32(out, 36 + dataSize);
    out.write("WAVE", 4);
    out.write("fmt ", 4);
    writeU32(out, 16);
    writeU16(out, 1);
    writeU16(out, channels);
    writeU32(out, sampleRate);
    writeU32(out, sampleRate * channels * bytesPerSample);
    writeU16(out, channels * bytesPerSample);
    writeU16(out, bits);
    out.write("data", 4);
    writeU32(out, dataSize);

    for (std::uint32_t i = 0; i < frames; ++i) {
        for (float x : {left[i], right[i]}) {
            x = std::clamp(x, -1.0f, 1.0f);
            const auto s = static_cast<std::int16_t>(std::lround(x * 32767.0f));
            writeU16(out, static_cast<std::uint16_t>(s));
        }
    }
}

} // namespace

int main(int argc, char** argv) {
    const char* output = argc > 1 ? argv[1] : "noctomorph-smoke.wav";

    constexpr double sampleRate = 48000.0;
    constexpr double seconds = 30.0;
    const std::size_t frames = static_cast<std::size_t>(sampleRate * seconds);

    auto engine = std::make_unique<noctomorph::Engine>();
    engine->prepare(sampleRate);
    engine->reset(0x125A4E4F43544F4DULL);

    noctomorph::Parameters p;
    p.foundation = 0.58f;
    p.world = 0.0f;   // no external asset in the smoke renderer
    p.texture = 0.30f;
    p.body = 0.55f;
    p.tension = 0.42f;
    p.evolve = 0.72f;
    p.events = 0.38f;
    p.space = 0.48f;
    p.output = 0.50f;

    engine->setParameters(p);
    engine->setArchetype(noctomorph::Archetype::Nocturne);
    engine->noteOn(36, 0.9f);

    std::vector<float> left(frames);
    std::vector<float> right(frames);

    constexpr std::size_t block = 257;
    std::size_t offset = 0;
    while (offset < frames) {
        const auto n = std::min(block, frames - offset);
        engine->process(left.data() + offset, right.data() + offset, n);
        offset += n;
    }

    writeWav(output, left, right, static_cast<std::uint32_t>(sampleRate));
    std::cout << "Rendered " << output
              << " | events=" << engine->eventCount()
              << " | seconds=" << seconds << "\n";
    return 0;
}
