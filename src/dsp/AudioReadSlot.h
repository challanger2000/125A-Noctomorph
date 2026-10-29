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
// A racing publication retains the last safely protected wave until next block.
// No retry/spin. At most two distinct retired objects can remain protected.
template<class T> class AudioReadSlot
{
public:
    const T* acquire() noexcept
    {
        const auto* candidate = live.load();
        speculative.store(candidate);
        if (candidate == live.load()) currentRead.store(candidate);
        speculative.store(nullptr);
        return currentRead.load();
    }
    void release() noexcept { currentRead.store(nullptr); speculative.store(nullptr); }
    void publish(std::shared_ptr<T> next)
    {
        live.store(next.get());
        if (owned) retired.push_back(std::move(owned));
        owned = std::move(next);
        collect();
    }
    void collect()
    {
        // Scan in the opposite order to the reader's handoff. If speculative
        // was already cleared, its pointer has been installed in currentRead.
        const auto* speculativeObject = speculative.load();
        const auto* protectedObject = currentRead.load();
        retired.erase(std::remove_if(retired.begin(),retired.end(),
            [protectedObject,speculativeObject](const auto& p)
            {return p.get()!=protectedObject && p.get()!=speculativeObject;}),retired.end());
    }
    std::shared_ptr<T> current() const { return owned; }
private:
    static_assert(std::atomic<const T*>::is_always_lock_free, "Audio pointers must be lock free");
    std::atomic<const T*> live{nullptr}, currentRead{nullptr}, speculative{nullptr};
    std::shared_ptr<T> owned;
    std::vector<std::shared_ptr<T>> retired;
};
}
