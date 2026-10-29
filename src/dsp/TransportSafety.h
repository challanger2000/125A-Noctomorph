#pragma once

#include <cmath>

namespace v4jb
{
// Host-reported BPM is not guaranteed to be positive or finite.
// Keep ordinary musical tempi unchanged. This also prevents a near-zero
// positive BPM from overflowing a float in the synced delay calculation.
inline double safeHostBpm (double bpm) noexcept
{
    constexpr double defaultBpm = 120.0;
    return std::isfinite (bpm) && bpm >= 1.0 && bpm <= 1000.0 ? bpm : defaultBpm;
}
} // namespace v4jb
