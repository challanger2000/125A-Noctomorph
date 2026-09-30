#include "PrototypeAssetBank.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

#if defined(_WIN32) && defined(NOCTOMORPH_HAS_EMBEDDED_ASSETS)
#include <windows.h>
#include "noctomorph_assets_generated.h"
#endif

namespace Noctomorph {
namespace {

std::uint16_t readU16(const unsigned char* p) noexcept {
    return static_cast<std::uint16_t>(
        p[0] | (static_cast<std::uint16_t>(p[1]) << 8));
}

std::uint32_t readU32(const unsigned char* p) noexcept {
    return static_cast<std::uint32_t>(p[0]) |
           (static_cast<std::uint32_t>(p[1]) << 8) |
           (static_cast<std::uint32_t>(p[2]) << 16) |
           (static_cast<std::uint32_t>(p[3]) << 24);
}

void moduleAnchor() noexcept {}

} // namespace

bool PrototypeAssetBank::parsePcm16Wav(
    const unsigned char* bytes,
    std::size_t size,
    OwnedAsset& out,
    bool loop,
    float excitationGain) {

    if (!bytes || size < 44)
        return false;
    if (std::memcmp(bytes, "RIFF", 4) != 0 ||
        std::memcmp(bytes + 8, "WAVE", 4) != 0)
        return false;

    std::uint16_t format = 0;
    std::uint16_t channels = 0;
    std::uint16_t bits = 0;
    std::uint32_t sampleRate = 0;
    const unsigned char* audio = nullptr;
    std::size_t audioBytes = 0;

    std::size_t pos = 12;
    while (pos + 8 <= size) {
        const auto* id = bytes + pos;
        const std::uint32_t chunkSize = readU32(bytes + pos + 4);
        pos += 8;
        if (pos + chunkSize > size)
            return false;

        if (std::memcmp(id, "fmt ", 4) == 0 && chunkSize >= 16) {
            format = readU16(bytes + pos);
            channels = readU16(bytes + pos + 2);
            sampleRate = readU32(bytes + pos + 4);
            bits = readU16(bytes + pos + 14);
        } else if (std::memcmp(id, "data", 4) == 0) {
            audio = bytes + pos;
            audioBytes = chunkSize;
        }

        pos += chunkSize + (chunkSize & 1u);
    }

    if (format != 1 || (channels != 1 && channels != 2) ||
        bits != 16 || sampleRate == 0 || !audio)
        return false;

    const std::size_t bytesPerFrame = static_cast<std::size_t>(channels) * 2u;
    if (audioBytes < bytesPerFrame)
        return false;

    const std::size_t frames = audioBytes / bytesPerFrame;
    out.left.resize(frames);
    if (channels == 2)
        out.right.resize(frames);
    else
        out.right.clear();

    for (std::size_t frame = 0; frame < frames; ++frame) {
        const std::size_t base = frame * bytesPerFrame;
        const auto l = static_cast<std::int16_t>(readU16(audio + base));
        out.left[frame] = static_cast<float>(l / 32768.0f);

        if (channels == 2) {
            const auto r = static_cast<std::int16_t>(
                readU16(audio + base + 2));
            out.right[frame] = static_cast<float>(r / 32768.0f);
        }
    }

    out.clip.left = out.left.data();
    out.clip.right = channels == 2 ? out.right.data() : nullptr;
    out.clip.frames = frames;
    out.clip.sampleRate = static_cast<double>(sampleRate);
    out.clip.loop = loop;
    out.clip.excitationGain = std::clamp(excitationGain, 0.0f, 2.0f);
    out.present = true;
    return true;
}

bool PrototypeAssetBank::load() {
    loaded_ = false;
    for (auto& asset : assets_)
        asset = {};

#if !defined(_WIN32) || !defined(NOCTOMORPH_HAS_EMBEDDED_ASSETS)
    return false;
#else
    try {
        HMODULE module = nullptr;
        if (!GetModuleHandleExW(
                GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&moduleAnchor),
                &module))
            return false;

        for (const auto& meta : kEmbeddedNoctomorphAssets) {
            if (meta.role < 0 || meta.role >= Count)
                return false;

            HRSRC resource = FindResourceW(
                module, MAKEINTRESOURCEW(meta.resourceId), RT_RCDATA);
            if (!resource)
                return false;

            HGLOBAL loaded = LoadResource(module, resource);
            if (!loaded)
                return false;

            const DWORD size = SizeofResource(module, resource);
            const auto* bytes =
                static_cast<const unsigned char*>(LockResource(loaded));
            if (!bytes || size == 0)
                return false;

            auto& asset = assets_[static_cast<std::size_t>(meta.role)];
            if (!parsePcm16Wav(
                    bytes,
                    static_cast<std::size_t>(size),
                    asset,
                    meta.loop,
                    meta.excitationGain))
                return false;
        }

        for (const auto& asset : assets_) {
            if (!asset.present)
                return false;
        }

        loaded_ = true;
        return true;
    } catch (...) {
        for (auto& asset : assets_)
            asset = {};
        loaded_ = false;
        return false;
    }
#endif
}

const noctomorph::Clip* PrototypeAssetBank::clipFor(Role role) const noexcept {
    const auto& asset = assets_[static_cast<std::size_t>(role)];
    return loaded_ && asset.present ? &asset.clip : nullptr;
}

const noctomorph::Clip* PrototypeAssetBank::world() const noexcept {
    return clipFor(World);
}

const noctomorph::Clip* PrototypeAssetBank::texture() const noexcept {
    return clipFor(Texture);
}

const noctomorph::Clip* PrototypeAssetBank::bodyBright() const noexcept {
    return clipFor(BodyBright);
}

const noctomorph::Clip* PrototypeAssetBank::bodyDeep() const noexcept {
    return clipFor(BodyDeep);
}

const noctomorph::Clip* PrototypeAssetBank::event() const noexcept {
    return clipFor(Event);
}

} // namespace Noctomorph
