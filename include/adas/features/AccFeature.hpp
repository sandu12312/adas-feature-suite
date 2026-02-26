#pragma once

#include "adas/features/IAdasFeature.hpp"

namespace adas {
namespace features {

// Adaptive Cruise Control — maintains the driver's set speed and automatically
// adjusts to keep a safe gap behind the vehicle ahead.
class AccFeature : public IAdasFeature {
public:
    // set_speed_mps: desired cruising speed (default 120 km/h = 33.33 m/s)
    explicit AccFeature(float set_speed_mps = 33.33f);

    void onEvent(events::EventType type, const events::EventData& data) override;
    void execute(VehicleState& state,
                 diagnostics::DTCManager& dtc,
                 uint64_t current_time_ms) override;
    const char* name() const override { return "ACC"; }

private:
    float set_speed_mps_;
    float ego_speed_mps_    = 0.0f;
    float distance_m_       = 999.0f;
    float target_speed_mps_ = 0.0f;
    float radar_confidence_ = 0.0f;
    bool  speed_valid_      = false;

    static constexpr float kMinGap        = 30.0f;   // metres
    static constexpr float kFollowRange   = 150.0f;  // metres — beyond this, free cruise
    static constexpr float kMaxAccel      = 2.0f;    // m/s²
    static constexpr float kMaxDecel      = 3.0f;    // m/s²
    static constexpr float kMinConfidence = 0.6f;
};

} // namespace features
} // namespace adas
