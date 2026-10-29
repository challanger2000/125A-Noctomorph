#include "../src/dsp/TransportSafety.h"

#include <cmath>
#include <iostream>
#include <limits>

static int fail (const char* msg)
{
    std::cerr << "FAIL: " << msg << '\n';
    return 1;
}

int main()
{
    using v4jb::safeHostBpm;
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    for (double invalid : {0.0, -1.0, -120.0, nan, inf, -inf, 1e-300, 1000.001})
        if (safeHostBpm (invalid) != 120.0)
            return fail ("invalid BPM must fall back to 120");
    for (double valid : {1.0, 20.0, 44.5, 92.0, 120.0, 200.0, 999.0, 1000.0})
        if (safeHostBpm (valid) != valid)
            return fail ("valid musical BPM changed");
    for (double raw : {0.0, -1.0, nan, inf, -inf, 1e-300, 1.0, 20.0, 120.0, 1000.0})
    {
        const double bpm = safeHostBpm (raw);
        const float syncedSeconds = 4.f * 60.f / static_cast<float> (bpm);
        if (! std::isfinite (syncedSeconds) || syncedSeconds <= 0.f)
            return fail ("sync delay seconds must stay finite and positive");
    }
    std::cout << "transport_safety_check OK\n";
}
