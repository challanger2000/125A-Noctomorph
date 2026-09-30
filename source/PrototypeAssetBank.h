#pragma once

#include "NoctomorphCore.h"

#include <array>
#include <cstddef>
#include <vector>

namespace Noctomorph {

class PrototypeAssetBank {
public:
    struct ScenePool {
        std::array<const noctomorph::Clip*, 3> world {};
        std::array<const noctomorph::Clip*, 3> texture {};
        std::array<const noctomorph::Clip*, 4> body {};
        std::array<const noctomorph::Clip*, 6> event {};
        std::size_t worldCount = 0;
        std::size_t textureCount = 0;
        std::size_t bodyCount = 0;
        std::size_t eventCount = 0;
    };

    bool load();
    bool loaded() const noexcept { return loaded_; }

    ScenePool sceneFor(noctomorph::Archetype archetype) const noexcept;

private:
    enum AssetId : int {
        WorldPaper = 0,
        WorldEccentric,
        WorldFence,
        TexturePacking,
        TextureSaw,
        TextureBrush,
        WorldWind,
        WorldAmbient,
        TextureChoir,
        BodyGlass,
        BodyGong,
        EventMetalDoor,
        EventCabinet,
        EventThud,
        EventPeters,
        Count
    };

    struct OwnedAsset {
        std::vector<float> left;
        std::vector<float> right;
        noctomorph::Clip clip {};
        bool present = false;
    };

    bool parsePcm16Wav(
        const unsigned char* bytes,
        std::size_t size,
        OwnedAsset& out,
        bool loop,
        float excitationGain);

    const noctomorph::Clip* clipFor(AssetId id) const noexcept;

    std::array<OwnedAsset, Count> assets_ {};
    bool loaded_ = false;
};

} // namespace Noctomorph
