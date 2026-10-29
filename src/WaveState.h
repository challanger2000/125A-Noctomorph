#pragma once
#include "dsp/Wave.h"
#include <JuceHeader.h>

// Persist the actual immutable analysis consumed by the synth, not a machine-
// specific path. Little-endian floats via JUCE streams; bounded to analyzer limits.
namespace WaveState
{
inline void append(juce::ValueTree& tree, int slot, const v4jb::WaveData& wave)
{
    juce::MemoryOutputStream bytes;
    for (float gain : wave.gain) bytes.writeFloat(gain);
    juce::ValueTree data("WAVE_DATA");
    data.setProperty("slot",slot,nullptr);
    data.setProperty("frames",wave.numFrames,nullptr);
    data.setProperty("seconds",wave.seconds,nullptr);
    data.setProperty("gains",bytes.getMemoryBlock().toBase64Encoding(),nullptr);
    tree.appendChild(data,nullptr);
}
inline std::shared_ptr<v4jb::WaveData> decode(const juce::ValueTree& data)
{
    const int frames=data["frames"];
    const float seconds=data["seconds"];
    if (frames<1 || frames>2048 || !std::isfinite(seconds) || seconds<=0 || seconds>120) return {};
    const auto encoded=data["gains"].toString();
    if (encoded.length()>3000000) return {};
    juce::MemoryBlock bytes;
    if (!bytes.fromBase64Encoding(encoded) || bytes.getSize()!=(size_t)frames*v4jb::WaveData::kBands*sizeof(float)) return {};
    auto wave=std::make_shared<v4jb::WaveData>();
    wave->numFrames=frames;wave->seconds=seconds;wave->gain.resize((size_t)frames*v4jb::WaveData::kBands);
    juce::MemoryInputStream input(bytes,false);
    for (auto& gain:wave->gain) { gain=input.readFloat();if (!std::isfinite(gain) || gain<0 || gain>1) return {}; }
    return wave;
}
}
