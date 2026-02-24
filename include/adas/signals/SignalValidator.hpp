#pragma once

#include <cstdint>
#include "adas/signals/Signal.hpp"

namespace adas {
namespace signals {

// Validates a Signal<float> against configurable physical limits and timing constraints.
// Each instance is configured once for a specific signal type (e.g. radar distance, ego speed).
// Writes the resulting SignalStatus back into the signal in-place.
class SignalValidator {
public:
    // Configures the validator with the physical and timing limits for one signal type.
    SignalValidator(float min_value, float max_value,
                   float min_confidence, uint64_t timeout_ms);

    // Evaluates the signal and sets signal.status to the appropriate SignalStatus.
    // Checks are applied in order of severity: TIMEOUT > OUT_OF_RANGE > LOW_CONFIDENCE > VALID.
    void validate(Signal<float>& signal, uint64_t current_time_ms) const;

private:
    float    min_value_;       // Lower bound of the physically valid range
    float    max_value_;       // Upper bound of the physically valid range
    float    min_confidence_;  // Minimum acceptable sensor confidence [0.0 – 1.0]
    uint64_t timeout_ms_;      // Maximum age of a signal before it is considered stale
};

} // namespace signals
} // namespace adas
