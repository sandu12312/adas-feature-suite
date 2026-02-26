#include "adas/features/LkaFeature.hpp"
#include <algorithm>
#include <cmath>
#include <variant>

namespace adas {
namespace features {

void LkaFeature::onEvent(events::EventType type, const events::EventData& data) {
    if (type == events::EventType::LANE_UPDATE) {
        const auto& l      = std::get<events::LaneData>(data);
        lateral_deviation_m_ = l.lateral_deviation_m;
        confidence_          = l.confidence;
    }
}

void LkaFeature::execute(VehicleState& state,
                          diagnostics::DTCManager& dtc,
                          uint64_t current_time_ms) {
    if (confidence_ < kMinConfidence) {
        dtc.report(diagnostics::DTC::LKA_LOW_CONFIDENCE,
                   diagnostics::Severity::WARNING,
                   "LKA: camera confidence too low", current_time_ms);
        return;
    }

    if (std::abs(lateral_deviation_m_) > kDeviationThreshold) {
        float correction = -lateral_deviation_m_ * kSteeringGain;
        state.steering_angle_rad = std::clamp(correction, -kMaxSteering, kMaxSteering);
    }
}

} // namespace features
} // namespace adas
