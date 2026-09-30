#pragma once

#include "base/source/fstreamer.h"
#include <cmath>

namespace Noctomorph {

constexpr Steinberg::int32 kStateMagic = 0x31434F4E; // "NOC1"
constexpr Steinberg::int32 kStateVersion = 2;
constexpr Steinberg::int32 kLegacyStateVersion = 1;
constexpr int kLegacyStateValueCount = 10;
constexpr int kStateValueCount = 11;
constexpr float kLegacyMotionDefault = 0.35f;

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
    if (!s.readInt32(magic) || magic != kStateMagic)
        return false;
    if (!s.readInt32(version))
        return false;

    for (float& v : values)
        v = 0.0f;
    values[10] = kLegacyMotionDefault;

    const int count =
        version == kLegacyStateVersion ? kLegacyStateValueCount :
        version == kStateVersion ? kStateValueCount : 0;
    if (count == 0)
        return false;

    for (int i = 0; i < count; ++i) {
        if (!s.readFloat(values[i]) ||
            !std::isfinite(values[i]) ||
            values[i] < 0.f || values[i] > 1.f)
            return false;
    }
    return true;
}

} // namespace Noctomorph
