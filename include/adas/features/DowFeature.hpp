#pragma once

#include "adas/features/IAdasFeature.hpp"

namespace adas {
namespace features {

// Door Open Warning — alerts the driver when a vehicle is approaching
// from behind while a door is open (e.g. when parked on a roadside).
class DowFeature : public IAdasFeature {
public:
    void onEvent(events::EventType type, const events::EventData& data) override;
    void execute(VehicleState& state,
                 diagnostics::DTCManager& dtc,
                 uint64_t current_time_ms) override;
    const char* name() const override { return "DOW"; }

private:
    float distance_m_       = 999.0f;
    float target_speed_mps_ = 0.0f;
    float radar_confidence_ = 0.0f;
    bool  door_open_        = false;

    static constexpr float kWarningDistance = 15.0f;   // metres
    static constexpr float kMinTargetSpeed  = 0.5f;    // m/s — ignore stationary objects
    static constexpr float kMinConfidence   = 0.6f;
};

} // namespace features
} // namespace adas
