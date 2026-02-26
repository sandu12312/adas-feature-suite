#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include "adas/features/AdasManager.hpp"
#include "adas/VehicleState.hpp"

using namespace adas;
using namespace adas::features;
using namespace adas::events;

// ─────────────────────────────────────────────────────────────────────────────
// Live simulation — runs physics-based scenarios in real time.
// Each step is 100ms of simulated time, printed with a 100ms real delay
// so you can watch the ADAS system react as conditions change.
// ─────────────────────────────────────────────────────────────────────────────

static void printHeader() {
    std::cout << std::left
              << std::setw(6)  << "T(ms)"
              << std::setw(10) << "EgoSpd"
              << std::setw(10) << "TgtSpd"
              << std::setw(10) << "Dist"
              << std::setw(8)  << "TTC"
              << std::setw(10) << "Accel"
              << std::setw(14) << "Brake"
              << std::setw(8)  << "Steer"
              << "Event\n"
              << std::string(80, '-') << "\n";
}

static void printStep(uint64_t t_ms, const VehicleState& s,
                      float distance_m, float target_speed,
                      const std::string& event_label) {
    float closing = s.ego_speed_mps - target_speed;
    float ttc     = (closing > 0.01f) ? (distance_m / closing) : 99.9f;

    std::string brake_str = s.brake_requested
        ? std::string("BRAKE ") + std::to_string(int(s.brake_intensity * 100)) + "%"
        : std::string("-");

    std::cout << std::left  << std::fixed << std::setprecision(1)
              << std::setw(6)  << t_ms
              << std::setw(10) << s.ego_speed_mps
              << std::setw(10) << target_speed
              << std::setw(10) << distance_m
              << std::setw(8)  << std::setprecision(2) << (ttc > 99.0f ? 99.9f : ttc)
              << std::setw(10) << std::setprecision(2) << s.ego_acceleration
              << std::setw(14) << brake_str
              << std::setw(8)  << std::setprecision(3) << s.steering_angle_rad
              << event_label << "\n";
}

// ── Scenario A: Emergency Brake ───────────────────────────────────────────────
// Ego drives at 30 m/s. Target suddenly brakes at T=2s.
// Watch AEB fire as TTC drops below threshold.
static void scenarioEmergencyBrake() {
    std::cout << "\n=== LIVE: Emergency Brake Scenario (10 seconds) ===\n";
    std::cout << "Ego cruises at 30 m/s. Target brakes hard at T=2s.\n\n";
    printHeader();

    AdasManager mgr;
    VehicleState state;

    const float dt        = 0.1f;   // 100ms steps
    float ego_speed       = 30.0f;  // m/s
    float target_speed    = 30.0f;  // m/s — same speed initially
    float distance        = 80.0f;  // m
    float lateral_dev     = 0.0f;

    for (int step = 0; step <= 100; ++step) {
        uint64_t t_ms = step * 100;
        std::string event;

        // Target brakes hard at T=2s
        if (t_ms >= 2000 && target_speed > 0.0f) {
            target_speed = std::max(0.0f, target_speed - 8.0f * dt);  // -8 m/s²
            if (target_speed == 0.0f && event.empty()) event = "<< TARGET STOPPED";
        }

        // Lane drift starts at T=5s (small deviation, shows LKA)
        if (t_ms >= 5000 && t_ms < 8000) {
            lateral_dev = 0.4f;
            event = (event.empty()) ? "<< LANE DRIFT" : event;
        } else if (t_ms >= 8000) {
            lateral_dev = 0.0f;
        }

        // Update distance based on speeds
        float closing_speed = ego_speed - target_speed;
        distance -= closing_speed * dt;
        distance  = std::max(distance, 2.0f);  // physical minimum (collision)

        // Reset state each cycle (features write fresh outputs)
        state              = VehicleState{};
        state.ego_speed_mps = ego_speed;

        // Publish sensor data to all features
        mgr.publish(EventType::SPEED_UPDATE, SpeedData{ego_speed});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{distance, target_speed, 0.95f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{lateral_dev, 0.9f});

        // Run all features
        mgr.execute(state, t_ms);

        // Apply acceleration to ego speed (physics integration)
        if (state.brake_requested) {
            ego_speed += state.ego_acceleration * dt;
            if (event.empty()) event = "<< AEB/ACC ACTIVE";
        } else {
            ego_speed += state.ego_acceleration * dt;
        }
        ego_speed = std::max(0.0f, ego_speed);

        // Print every 200ms to keep output readable
        if (step % 2 == 0) {
            printStep(t_ms, state, distance, target_speed, event);
        }

        // Real-time pacing (comment out for instant output)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// ── Scenario B: Door Open Warning ────────────────────────────────────────────
// Vehicle is parked with door open. Cyclist approaches from behind.
static void scenarioDoorWarning() {
    std::cout << "\n=== LIVE: Door Open Warning Scenario (6 seconds) ===\n";
    std::cout << "Vehicle parked, door open. Cyclist approaching at 5 m/s.\n\n";
    printHeader();

    AdasManager mgr;
    VehicleState state;

    float cyclist_dist = 40.0f;
    const float cyclist_speed = 5.0f;
    const float dt = 0.1f;

    for (int step = 0; step <= 60; ++step) {
        uint64_t t_ms = step * 100;

        cyclist_dist -= cyclist_speed * dt;
        cyclist_dist  = std::max(cyclist_dist, 0.5f);

        state = VehicleState{};
        state.door_open = true;

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{0.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{cyclist_dist, cyclist_speed, 0.9f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.0f, 0.9f});
        mgr.publish(EventType::DOOR_UPDATE,  DoorData{true});
        mgr.execute(state, t_ms);

        std::string event = state.dow_warning ? "<< DOW WARNING!" : "";

        if (step % 2 == 0) {
            printStep(t_ms, state, cyclist_dist, cyclist_speed, event);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, char* argv[]) {
    std::cout << "ADAS Feature Suite - Live Simulator\n";
    std::cout << "====================================\n";
    std::cout << "Press Ctrl+C to stop at any time.\n";

    // Default: run both scenarios
    // Pass "aeb" or "dow" as argument to run just one
    std::string mode = (argc > 1) ? argv[1] : "all";

    if (mode == "aeb" || mode == "all") scenarioEmergencyBrake();
    if (mode == "dow" || mode == "all") scenarioDoorWarning();

    std::cout << "\nSimulation complete.\n";
    return 0;
}
