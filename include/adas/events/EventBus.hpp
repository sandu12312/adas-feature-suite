#pragma once

#include <map>
#include <vector>
#include "adas/events/EventType.hpp"
#include "adas/events/EventData.hpp"
#include "adas/events/IEventSubscriber.hpp"

namespace adas {
namespace events {

// Routes published events to all subscribers registered for that EventType.
class EventBus {
public:
    // Register a subscriber to receive events of the given type.
    void subscribe(EventType type, IEventSubscriber* subscriber);

    // Deliver an event to every subscriber registered for that type.
    void publish(EventType type, const EventData& data);

private:
    std::map<EventType, std::vector<IEventSubscriber*>> subscribers_;
};

} // namespace events
} // namespace adas
