#pragma once

namespace adas {
namespace events {

// Identifies the type of event published on the EventBus.
// Publishers tag each event with one of these values;
// subscribers filter by type to receive only what they need.
enum class EventType {
    RADAR_UPDATE,  // New distance measurement from radar  (consumers: AEB, ACC, DOW)
    SPEED_UPDATE,  // Ego vehicle speed updated            (consumers: AEB, ACC)
    LANE_UPDATE,   // Lateral deviation from lane centre   (consumers: LKA)
    DOOR_UPDATE    // Door open/closed state changed       (consumers: DOW)
};

} // namespace events
} // namespace adas
