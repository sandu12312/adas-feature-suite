#pragma once

namespace adas {
namespace signals {

// Represents all possible validity states of a sensor signal.
// Every Signal<T> carries one of these states so that ADAS features
// can decide whether to act on the data or enter a safe fallback state.
enum class SignalStatus {
    VALID,           // Data is fresh, in-range and above confidence threshold
    TIMEOUT,         // No new data received within the allowed time window
    OUT_OF_RANGE,    // Value exceeds the physical limits defined for this signal
    LOW_CONFIDENCE,  // Sensor is reporting data but with insufficient certainty
    INITIALIZING     // System just started; no measurement has been taken yet
};

} // namespace signals
} // namespace adas
