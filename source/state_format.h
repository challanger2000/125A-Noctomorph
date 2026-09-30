#pragma once

#include "base/source/fstreamer.h"
#include <cmath>

namespace Noctomorph {

constexpr Steinberg::int32 kStateMagic = 0x31434F4E; // "NOC1"
constexpr Steinberg::int32 kStateVersion = 1;
constexpr int kStateValueCount = 10;

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
    if (!s.readInt32(version) || version != kStateVersion)
        return false;

    for (float& v : values) {
        if (!s.readFloat(v) || !std::isfinite(v) || v < 0.f || v > 1.f)
            return false;
    }
    return true;
}

} // namespace Noctomorph
