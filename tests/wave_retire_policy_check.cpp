#include "../src/dsp/WaveRetirePolicy.h"

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

int main()
{
    using v4jb::canRetireWaveAfterBlocks;
    const auto fail = [] (const char* msg) { std::cerr << "FAIL: " << msg << '\n'; return 1; };
    for (std::int64_t count : {-100, -1, 0, 1})
        if (canRetireWaveAfterBlocks (count, 0))
            return fail ("retired before two completed blocks");
    if (!canRetireWaveAfterBlocks (2, 0) || !canRetireWaveAfterBlocks (400, 398))
        return fail ("never retired after two blocks");
    if (canRetireWaveAfterBlocks (400, 399) || canRetireWaveAfterBlocks (0, 100))
        return fail ("retired prematurely");

    // Deterministic stalled-callback interleaving. Test synchronisation belongs
    // in the test only, never in the realtime audio callback.
    auto old = std::make_shared<int> (123456);
    const std::weak_ptr<int> sentinel = old;
    auto replacement = std::make_shared<int> (77);
    std::atomic<int*> live {old.get()};
    std::atomic<int> stage {0};
    std::atomic<std::int64_t> completed {0};

    std::thread callback ([&]
    {
        int* inFlight = live.load (std::memory_order_seq_cst);
        stage.store (1, std::memory_order_release);
        while (stage.load (std::memory_order_acquire) != 2)
            std::this_thread::yield();
        if (*inFlight != 123456)
            std::abort();
        completed.fetch_add (1, std::memory_order_release);
    });
    while (stage.load (std::memory_order_acquire) != 1)
        std::this_thread::yield();
    live.store (replacement.get(), std::memory_order_seq_cst);
    std::vector<std::shared_ptr<int>> retired {old};
    old.reset();
    if (canRetireWaveAfterBlocks (completed.load(), 0) || sentinel.expired())
        std::abort();
    stage.store (2, std::memory_order_release);
    callback.join();
    if (canRetireWaveAfterBlocks (completed.load(), 0))
        return fail ("one completed block is insufficient");
    completed.fetch_add (1, std::memory_order_release);
    if (!canRetireWaveAfterBlocks (completed.load(), 0))
        return fail ("two completed blocks should permit retirement");
    retired.clear();
    if (!sentinel.expired())
        return fail ("retired wave not reclaimed");
    std::cout << "wave_retire_policy_check OK\n";
}
