#pragma once

#include "adas/events/EventType.hpp"
#include "adas/events/EventData.hpp"

namespace adas {
namespace events {

// Interface that every ADAS feature implements to receive events from the EventBus.
class IEventSubscriber {
public:
    virtual ~IEventSubscriber() = default;

    // Called by EventBus when an event of interest is published.
    virtual void onEvent(EventType type, const EventData& data) = 0;
};

} // namespace events
} // namespace adas
