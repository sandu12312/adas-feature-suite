#include "adas/features/AccFeature.hpp"
#include <algorithm>
#include <variant>

namespace adas {
namespace features {

AccFeature::AccFeature(float set_speed_mps) : set_speed_mps_(set_speed_mps) {}

void AccFeature::onEvent(events::EventType type, const events::EventData& data) {
    if (type == events::EventType::RADAR_UPDATE) {
        const auto& r   = std::get<events::RadarData>(data);
        distance_m_       = r.distance_m;
        target_speed_mps_ = r.target_speed_mps;
        radar_confidence_ = r.confidence;
    } else if (type == events::EventType::SPEED_UPDATE) {
        ego_speed_mps_ = std::get<events::SpeedData>(data).speed_mps;
        speed_valid_   = true;
    }
}

void AccFeature::execute(VehicleState& state,
                          diagnostics::DTCManager& dtc,
                          uint64_t current_time_ms) {
    if (!speed_valid_) {
        dtc.report(diagnostics::DTC::ACC_SENSOR_FAULT,
                   diagnostics::Severity::WARNING,
                   "ACC: speed signal not ready", current_time_ms);
        return;
    }

    float desired_speed = set_speed_mps_;

    // A vehicle is detected ahead within following range
    if (radar_confidence_ >= kMinConfidence && distance_m_ < kFollowRange) {
        float gap_error = distance_m_ - kMinGap;

        if (gap_error < 0.0f) {
            // Too close — slow down to match target speed
            desired_speed = std::max(0.0f, target_speed_mps_);
        } else {
            // In following range — proportional approach to set speed
            desired_speed = std::min(set_speed_mps_,
                                     target_speed_mps_ + gap_error * 0.5f);
        }
    }

    float raw_accel = (desired_speed - ego_speed_mps_) * 0.3f;
    state.ego_acceleration = std::clamp(raw_accel, -kMaxDecel, kMaxAccel);
}

} // namespace features
} // namespace adas
