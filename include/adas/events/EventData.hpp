#pragma once

#include <variant>
#include <cstdint>

namespace adas {
namespace events {

// Payload for a radar distance measurement event.
struct RadarData {
    float distance_m       = 0.0f;  // Distance to the vehicle ahead (metres)
    float target_speed_mps = 0.0f;  // Speed of the vehicle ahead (m/s)
    float confidence       = 0.0f;  // Sensor certainty [0.0 – 1.0]
};

// Payload for an ego vehicle speed update event.
struct SpeedData {
    float speed_mps = 0.0f;  // Ego vehicle speed (m/s)
};

// Payload for a lane deviation measurement event.
struct LaneData {
    float lateral_deviation_m = 0.0f;  // Offset from lane centre (m); +right, -left
    float confidence          = 0.0f;  // Camera certainty [0.0 – 1.0]
};

// Payload for a door state change event.
struct DoorData {
    bool is_open = false;  // True when the door is open
};

// A single EventData value holds exactly one of the four payload types.
using EventData = std::variant<RadarData, SpeedData, LaneData, DoorData>;

} // namespace events
} // namespace adas
