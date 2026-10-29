#pragma once
#include <atomic>
#include <memory>
#include <vector>
#include <algorithm>

namespace v4jb
{
// One audio reader; publishers/collectors are externally serialized off audio.
// All pointer operations are seq_cst: publish -> scan and protect -> validate
// cannot both miss one another. Never dereference the speculative first load.
// A concurrent publication may cause one block to use no wave; no retry/spin.
template<class T> class AudioReadSlot
{
public:
    const T* acquire() noexcept
    {
        const auto* candidate = live.load();
        hazard.store(candidate);
        if (candidate == live.load()) return candidate;
        hazard.store(nullptr);
        return nullptr;
    }
    void release() noexcept { hazard.store(nullptr); }
    void publish(std::shared_ptr<T> next)
    {
        live.store(next.get());
        if (owned) retired.push_back(std::move(owned));
        owned = std::move(next);
        collect();
    }
    void collect()
    {
        const auto* protectedObject = hazard.load();
        retired.erase(std::remove_if(retired.begin(),retired.end(),
            [protectedObject](const auto& p){return p.get()!=protectedObject;}),retired.end());
    }
    std::shared_ptr<T> current() const { return owned; }
private:
    static_assert(std::atomic<const T*>::is_always_lock_free, "Audio pointers must be lock free");
    std::atomic<const T*> live{nullptr}, hazard{nullptr};
    std::shared_ptr<T> owned;
    std::vector<std::shared_ptr<T>> retired;
};
}
