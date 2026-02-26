#pragma once

#include "adas/features/IAdasFeature.hpp"

namespace adas {
namespace features {

// Lane Keep Assist — applies a proportional steering correction when the vehicle
// drifts more than a set threshold from the lane centre.
class LkaFeature : public IAdasFeature {
public:
    void onEvent(events::EventType type, const events::EventData& data) override;
    void execute(VehicleState& state,
                 diagnostics::DTCManager& dtc,
                 uint64_t current_time_ms) override;
    const char* name() const override { return "LKA"; }

private:
    float lateral_deviation_m_ = 0.0f;
    float confidence_          = 0.0f;

    static constexpr float kDeviationThreshold = 0.3f;   // metres
    static constexpr float kSteeringGain       = 0.5f;   // rad/m
    static constexpr float kMaxSteering        = 0.5f;   // rad
    static constexpr float kMinConfidence      = 0.6f;
};

} // namespace features
} // namespace adas
