#pragma once

#include <cstdint>

namespace v4jb
{
// Never retire audio data by elapsed wall time: the audio callback can be
// suspended indefinitely with a raw pointer to the previous WaveData.
// A completed-block count is the only reclamation signal in this protocol.
// One in-flight callback may have observed the old pointer at publication.
inline bool canRetireWaveAfterBlocks (std::int64_t completedBlocks,
                                      std::int64_t blockAtSwap) noexcept
{
    return completedBlocks >= blockAtSwap && completedBlocks - blockAtSwap >= 2;
}
} // namespace v4jb
