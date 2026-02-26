#include "adas/features/AdasManager.hpp"
#include "adas/features/AebFeature.hpp"
#include "adas/features/AccFeature.hpp"
#include "adas/features/LkaFeature.hpp"
#include "adas/features/DowFeature.hpp"

namespace adas {
namespace features {

AdasManager::AdasManager() {
    // Create all features
    auto aeb = std::make_unique<AebFeature>();
    auto acc = std::make_unique<AccFeature>();
    auto lka = std::make_unique<LkaFeature>();
    auto dow = std::make_unique<DowFeature>();

    // Subscribe each feature to the events it needs
    event_bus_.subscribe(events::EventType::RADAR_UPDATE, aeb.get());
    event_bus_.subscribe(events::EventType::SPEED_UPDATE, aeb.get());

    event_bus_.subscribe(events::EventType::RADAR_UPDATE, acc.get());
    event_bus_.subscribe(events::EventType::SPEED_UPDATE, acc.get());

    event_bus_.subscribe(events::EventType::LANE_UPDATE,  lka.get());

    event_bus_.subscribe(events::EventType::RADAR_UPDATE, dow.get());
    event_bus_.subscribe(events::EventType::DOOR_UPDATE,  dow.get());

    features_.push_back(std::move(aeb));
    features_.push_back(std::move(acc));
    features_.push_back(std::move(lka));
    features_.push_back(std::move(dow));
}

void AdasManager::publish(events::EventType type, const events::EventData& data) {
    event_bus_.publish(type, data);
}

void AdasManager::execute(VehicleState& state, uint64_t current_time_ms) {
    for (auto& feature : features_) {
        feature->execute(state, dtc_manager_, current_time_ms);
    }
}

const diagnostics::DTCManager& AdasManager::dtcManager() const {
    return dtc_manager_;
}

} // namespace features
} // namespace adas
