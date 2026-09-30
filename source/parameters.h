#pragma once
#include "pluginterfaces/vst/vsttypes.h"

namespace Noctomorph {

enum ParamIds : Steinberg::Vst::ParamID {
    kArchetype  = 3000,
    kFoundation = 3001,
    kWorld      = 3002,
    kTexture    = 3003,
    kBody       = 3004,
    kTension    = 3005,
    kEvolve     = 3006,
    kEvents     = 3007,
    kSpace      = 3008,
    kOutput     = 3009
};

inline constexpr int kParamCount = 10;

} // namespace Noctomorph
