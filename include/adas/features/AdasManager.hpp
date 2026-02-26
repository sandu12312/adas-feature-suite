#pragma once

#include <memory>
#include <vector>
#include "adas/events/EventBus.hpp"
#include "adas/diagnostics/DTCManager.hpp"
#include "adas/features/IAdasFeature.hpp"
#include "adas/VehicleState.hpp"

namespace adas {
namespace features {

// Owns all ADAS features and coordinates event routing and execution each cycle.
class AdasManager {
public:
    AdasManager();

    // Publish a sensor event — the EventBus delivers it to subscribed features.
    void publish(events::EventType type, const events::EventData& data);

    // Run all features and update the shared vehicle state.
    void execute(VehicleState& state, uint64_t current_time_ms);

    // Access the DTC log after execution.
    const diagnostics::DTCManager& dtcManager() const;

private:
    events::EventBus                             event_bus_;
    diagnostics::DTCManager                      dtc_manager_;
    std::vector<std::unique_ptr<IAdasFeature>>   features_;
};

} // namespace features
} // namespace adas
