#pragma once

#include "base/source/fstreamer.h"
#include <cmath>

namespace Noctomorph {

constexpr Steinberg::int32 kStateMagic = 0x31434F4E; // "NOC1"
constexpr Steinberg::int32 kStateVersion = 3;
constexpr Steinberg::int32 kLegacyStateVersion1 = 1;
constexpr Steinberg::int32 kLegacyStateVersion2 = 2;
constexpr int kStateValueCount = 2; // RANDOM DNA, INTENSITY

inline bool writeState(
    Steinberg::IBStreamer& s,
    const float (&values)[kStateValueCount]) {
    if (!s.writeInt32(kStateMagic) || !s.writeInt32(kStateVersion))
        return false;
    for (float v : values) {
        if (!std::isfinite(v) || v < 0.f || v > 1.f || !s.writeFloat(v))
            return false;
    }
    return true;
}

inline bool readState(
    Steinberg::IBStreamer& s,
    float (&values)[kStateValueCount]) {
    Steinberg::int32 magic = 0;
    Steinberg::int32 version = 0;
    if (!s.readInt32(magic) || magic != kStateMagic || !s.readInt32(version))
        return false;

    const int count =
        version == kStateVersion ? 2 :
        version == kLegacyStateVersion2 ? 11 :
        version == kLegacyStateVersion1 ? 10 : 0;
    if (count == 0)
        return false;

    for (int i = 0; i < count; ++i) {
        float v = 0.0f;
        if (!s.readFloat(v) || !std::isfinite(v) || v < 0.f || v > 1.f)
            return false;
        if (i < kStateValueCount)
            values[i] = v;
    }
    return true;
}

} // namespace Noctomorph
