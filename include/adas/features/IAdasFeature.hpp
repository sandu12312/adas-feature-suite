#pragma once

#include "adas/events/IEventSubscriber.hpp"
#include "adas/VehicleState.hpp"
#include "adas/diagnostics/DTCManager.hpp"

namespace adas {
namespace features {

// Base interface for all ADAS features.
// Each feature subscribes to relevant events and acts on VehicleState each cycle.
class IAdasFeature : public events::IEventSubscriber {
public:
    virtual ~IAdasFeature() = default;

    // Apply feature logic and write outputs to state. Called once per cycle.
    virtual void execute(VehicleState& state,
                         diagnostics::DTCManager& dtc,
                         uint64_t current_time_ms) = 0;

    // Human-readable name used for logging.
    virtual const char* name() const = 0;
};

} // namespace features
} // namespace adas
