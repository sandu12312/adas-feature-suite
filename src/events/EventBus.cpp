#include "adas/events/EventBus.hpp"

namespace adas {
namespace events {

void EventBus::subscribe(EventType type, IEventSubscriber* subscriber) {
    subscribers_[type].push_back(subscriber);
}

void EventBus::publish(EventType type, const EventData& data) {
    auto it = subscribers_.find(type);
    if (it == subscribers_.end()) return;

    for (IEventSubscriber* sub : it->second) {
        sub->onEvent(type, data);
    }
}

} // namespace events
} // namespace adas
