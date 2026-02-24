#include "adas/signals/SignalValidator.hpp"

namespace adas {
namespace signals {

SignalValidator::SignalValidator(float min_value, float max_value,
                                 float min_confidence, uint64_t timeout_ms)
    : min_value_(min_value), max_value_(max_value),
      min_confidence_(min_confidence), timeout_ms_(timeout_ms) {}

void SignalValidator::validate(Signal<float>& signal, uint64_t current_time_ms) const {
    // Stale data takes highest priority — act on nothing if sensor is silent
    if ((current_time_ms - signal.timestamp_ms) > timeout_ms_) {
        signal.status = SignalStatus::TIMEOUT;
        return;
    }

    // Physically impossible values indicate a sensor fault
    if (signal.value < min_value_ || signal.value > max_value_) {
        signal.status = SignalStatus::OUT_OF_RANGE;
        return;
    }

    // Data is present but the sensor is not confident enough to act on
    if (signal.confidence < min_confidence_) {
        signal.status = SignalStatus::LOW_CONFIDENCE;
        return;
    }

    // All checks passed — safe to consume
    signal.status = SignalStatus::VALID;
}

} // namespace signals
} // namespace adas
