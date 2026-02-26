#include <gtest/gtest.h>
#include "adas/events/EventBus.hpp"
#include "adas/events/IEventSubscriber.hpp"

using namespace adas::events;

// Minimal subscriber that records the last event it received
class MockSubscriber : public IEventSubscriber {
public:
    EventType last_type = EventType::RADAR_UPDATE;
    bool      received  = false;

    void onEvent(EventType type, const EventData&) override {
        last_type = type;
        received  = true;
    }
};

TEST(EventBus, SubscriberReceivesMatchingEvent) {
    EventBus bus;
    MockSubscriber sub;
    bus.subscribe(EventType::RADAR_UPDATE, &sub);
    bus.publish(EventType::RADAR_UPDATE, RadarData{30.0f, 0.0f, 0.9f});
    EXPECT_TRUE(sub.received);
    EXPECT_EQ(sub.last_type, EventType::RADAR_UPDATE);
}

TEST(EventBus, SubscriberIgnoresOtherEvents) {
    EventBus bus;
    MockSubscriber sub;
    bus.subscribe(EventType::SPEED_UPDATE, &sub);
    bus.publish(EventType::RADAR_UPDATE, RadarData{30.0f, 0.0f, 0.9f});
    EXPECT_FALSE(sub.received);
}

TEST(EventBus, MultipleSubscribersReceiveEvent) {
    EventBus bus;
    MockSubscriber a, b;
    bus.subscribe(EventType::RADAR_UPDATE, &a);
    bus.subscribe(EventType::RADAR_UPDATE, &b);
    bus.publish(EventType::RADAR_UPDATE, RadarData{30.0f, 0.0f, 0.9f});
    EXPECT_TRUE(a.received);
    EXPECT_TRUE(b.received);
}
