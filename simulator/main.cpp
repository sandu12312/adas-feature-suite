#include <iostream>
#include <iomanip>
#include "adas/features/AdasManager.hpp"
#include "adas/signals/SignalValidator.hpp"
#include "adas/VehicleState.hpp"

using namespace adas;
using namespace adas::features;
using namespace adas::events;
using namespace adas::signals;

// Prints the vehicle state in a compact, readable format
static void printState(const VehicleState& s, uint64_t t_ms, const char* label) {
    std::cout << "[t=" << std::setw(4) << t_ms << "ms] " << label << "\n"
              << "  speed="      << std::fixed << std::setprecision(1) << s.ego_speed_mps  << " m/s"
              << "  accel="      << s.ego_acceleration   << " m/s²"
              << "  steer="      << s.steering_angle_rad << " rad"
              << "  brake="      << (s.brake_requested ? "YES(" + std::to_string(int(s.brake_intensity * 100)) + "%)" : "no")
              << "  DOW="        << (s.dow_warning ? "WARN" : "ok")
              << "\n";
}

static void runScenario(const char* title,
                        AdasManager& mgr,
                        VehicleState state,
                        auto publishFn,
                        uint64_t t_ms) {
    std::cout << "\n=== " << title << " ===\n";
    publishFn(mgr, t_ms);
    mgr.execute(state, t_ms);
    printState(state, t_ms, title);
    mgr.dtcManager().dump();
}

int main() {
    std::cout << "ADAS Feature Suite — Simulator\n";
    std::cout << "==============================\n";

    // ── Scenario 1: Emergency Brake ──────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 30.0f;  // 108 km/h

        std::cout << "\n=== Scenario 1: Emergency Brake ===\n";
        std::cout << "Ego: 30 m/s | Target: stopped | Distance: 20m → TTC = 0.67s\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{30.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{20.0f, 0.0f, 0.95f});
        mgr.execute(state, 1000);
        printState(state, 1000, "AEB response");
        mgr.dtcManager().dump();
    }

    // ── Scenario 2: ACC Highway Cruise ───────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 25.0f;  // 90 km/h — below set speed

        std::cout << "\n=== Scenario 2: ACC Highway Cruise ===\n";
        std::cout << "Ego: 25 m/s | No car ahead → should accelerate to 33.3 m/s\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{25.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{999.0f, 0.0f, 0.9f});  // clear road
        mgr.execute(state, 2000);
        printState(state, 2000, "ACC response");
        mgr.dtcManager().dump();
    }

    // ── Scenario 3: ACC Following ─────────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 30.0f;

        std::cout << "\n=== Scenario 3: ACC Following — Too Close ===\n";
        std::cout << "Ego: 30 m/s | Target: 20 m/s | Gap: 10m → should decelerate\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{30.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{10.0f, 20.0f, 0.9f});
        mgr.execute(state, 3000);
        printState(state, 3000, "ACC following response");
        mgr.dtcManager().dump();
    }

    // ── Scenario 4: Lane Departure ────────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;

        std::cout << "\n=== Scenario 4: Lane Departure ===\n";
        std::cout << "Lateral deviation: +0.5m (drifting right) → LKA corrects left\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{30.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{999.0f, 0.0f, 0.9f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.5f, 0.92f});
        mgr.execute(state, 4000);
        printState(state, 4000, "LKA response");
        mgr.dtcManager().dump();
    }

    // ── Scenario 5: Door Open Warning ────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.door_open = true;

        std::cout << "\n=== Scenario 5: Door Open Warning ===\n";
        std::cout << "Door open | Cyclist at 8m, 5 m/s → DOW warning\n";

        mgr.publish(EventType::DOOR_UPDATE,  DoorData{true});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{8.0f, 5.0f, 0.9f});
        mgr.publish(EventType::SPEED_UPDATE, SpeedData{0.0f});  // parked
        mgr.execute(state, 5000);
        printState(state, 5000, "DOW response");
        mgr.dtcManager().dump();
    }

    std::cout << "\nSimulation complete.\n";
    return 0;
}
