#pragma once

#include "NoctomorphCore.h"

#include <array>
#include <cstddef>
#include <vector>

namespace Noctomorph {

class PrototypeAssetBank {
public:
    bool load();
    bool loaded() const noexcept { return loaded_; }

    const noctomorph::Clip* world() const noexcept;
    const noctomorph::Clip* texture() const noexcept;
    const noctomorph::Clip* bodyBright() const noexcept;
    const noctomorph::Clip* bodyDeep() const noexcept;
    const noctomorph::Clip* event() const noexcept;

private:
    enum Role : int {
        World = 0,
        Texture = 1,
        BodyBright = 2,
        BodyDeep = 3,
        Event = 4,
        Count = 5
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

    const noctomorph::Clip* clipFor(Role role) const noexcept;

    std::array<OwnedAsset, Count> assets_ {};
    bool loaded_ = false;
};

} // namespace Noctomorph
