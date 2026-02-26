#pragma once

namespace adas {

// Shared state written by ADAS features each processing cycle.
// Acts as the output interface between features and the actuator layer.
struct VehicleState {
    // Ego vehicle
    float ego_speed_mps      = 0.0f;   // Current speed (m/s)
    float ego_acceleration   = 0.0f;   // Commanded acceleration (m/s²); negative = decelerate
    float steering_angle_rad = 0.0f;   // Commanded steering correction (rad); +right, -left

    // Target vehicle ahead (populated from radar)
    float target_distance_m  = 999.0f; // Distance to the vehicle ahead (m)
    float target_speed_mps   = 0.0f;   // Speed of the vehicle ahead (m/s)

    // Lane
    float lateral_deviation_m = 0.0f;  // Offset from lane centre (m)

    // Actuator requests
    bool  brake_requested  = false;    // Any feature is requesting braking
    float brake_intensity  = 0.0f;     // Brake force [0.0 – 1.0]
    bool  dow_warning      = false;    // Door Open Warning active
    bool  door_open        = false;    // Physical door state
};

} // namespace adas
