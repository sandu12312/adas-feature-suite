#include "adas/features/AebFeature.hpp"
#include <variant>

namespace adas {
namespace features {

void AebFeature::onEvent(events::EventType type, const events::EventData& data) {
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

void AebFeature::execute(VehicleState& state,
                          diagnostics::DTCManager& dtc,
                          uint64_t current_time_ms) {
    // Cannot act without valid speed or low-confidence radar
    if (!speed_valid_ || radar_confidence_ < kMinConfidence) {
        dtc.report(diagnostics::DTC::AEB_SENSOR_FAULT,
                   diagnostics::Severity::WARNING,
                   "AEB: sensor not ready", current_time_ms);
        return;
    }

    float closing_speed = ego_speed_mps_ - target_speed_mps_;
    if (closing_speed <= 0.0f) return;  // target moving away or faster — no risk

    float ttc = distance_m_ / closing_speed;

    if (ttc < kFullBrakeTtc) {
        state.brake_requested = true;
        state.brake_intensity = 1.0f;
        dtc.report(diagnostics::DTC::AEB_ACTIVATED,
                   diagnostics::Severity::INFO,
                   "AEB: full emergency brake", current_time_ms);
    } else if (ttc < kPartialBrakeTtc) {
        state.brake_requested = true;
        state.brake_intensity = (kPartialBrakeTtc - ttc) / (kPartialBrakeTtc - kFullBrakeTtc);
    }
}

} // namespace features
} // namespace adas
