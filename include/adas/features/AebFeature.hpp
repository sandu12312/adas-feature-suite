#pragma once

#include "adas/features/IAdasFeature.hpp"

namespace adas {
namespace features {

// Automatic Emergency Braking — triggers full or partial brake when
// the Time-To-Collision (TTC) with the vehicle ahead drops below safe thresholds.
class AebFeature : public IAdasFeature {
public:
    void onEvent(events::EventType type, const events::EventData& data) override;
    void execute(VehicleState& state,
                 diagnostics::DTCManager& dtc,
                 uint64_t current_time_ms) override;
    const char* name() const override { return "AEB"; }

private:
    float distance_m_       = 999.0f;
    float target_speed_mps_ = 0.0f;
    float ego_speed_mps_    = 0.0f;
    float radar_confidence_ = 0.0f;
    bool  speed_valid_      = false;

    static constexpr float kFullBrakeTtc    = 1.5f;  // seconds
    static constexpr float kPartialBrakeTtc = 3.0f;  // seconds
    static constexpr float kMinConfidence   = 0.6f;
};

} // namespace features
} // namespace adas
