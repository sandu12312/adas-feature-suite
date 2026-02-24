#pragma once

#include <cstdint>
#include "adas/signals/SignalStatus.hpp"

namespace adas {
namespace signals {

// Generic wrapper for any sensor measurement.
// Bundles the raw value together with metadata needed for validation
// and safe consumption by ADAS features.
//
// Usage:
//   Signal<float> radar_dist;
//   radar_dist.value        = 25.3f;
//   radar_dist.timestamp_ms = 1000;
//   radar_dist.status       = SignalStatus::VALID;
//   radar_dist.confidence   = 0.95f;
template <typename T>
struct Signal {
    T            value        = T{};                       // Measured sensor value
    uint64_t     timestamp_ms = 0;                         // Acquisition time in milliseconds
    SignalStatus status       = SignalStatus::INITIALIZING; // Current validity state
    float        confidence   = 0.0f;                      // Sensor certainty [0.0 – 1.0]
};

} // namespace signals
} // namespace adas
