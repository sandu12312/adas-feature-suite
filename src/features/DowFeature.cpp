#include "adas/features/DowFeature.hpp"
#include <variant>

namespace adas {
namespace features {

void DowFeature::onEvent(events::EventType type, const events::EventData& data) {
    if (type == events::EventType::RADAR_UPDATE) {
        const auto& r   = std::get<events::RadarData>(data);
        distance_m_       = r.distance_m;
        target_speed_mps_ = r.target_speed_mps;
        radar_confidence_ = r.confidence;
    } else if (type == events::EventType::DOOR_UPDATE) {
        door_open_ = std::get<events::DoorData>(data).is_open;
    }
}

void DowFeature::execute(VehicleState& state,
                          diagnostics::DTCManager& dtc,
                          uint64_t current_time_ms) {
    state.dow_warning = false;

    if (!door_open_) return;

    if (radar_confidence_ < kMinConfidence) {
        dtc.report(diagnostics::DTC::DOW_SENSOR_FAULT,
                   diagnostics::Severity::WARNING,
                   "DOW: radar not reliable with door open", current_time_ms);
        return;
    }

    if (distance_m_ < kWarningDistance && target_speed_mps_ > kMinTargetSpeed) {
        state.dow_warning = true;
        dtc.report(diagnostics::DTC::DOW_WARNING_ACTIVE,
                   diagnostics::Severity::WARNING,
                   "DOW: vehicle approaching open door", current_time_ms);
    }
}

} // namespace features
} // namespace adas
