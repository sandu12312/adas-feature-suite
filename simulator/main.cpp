#include <iostream>
#include <iomanip>
#include <string>
#include "adas/features/AdasManager.hpp"
#include "adas/VehicleState.hpp"

using namespace adas;
using namespace adas::features;
using namespace adas::events;

static void printState(const VehicleState& s, uint64_t t_ms, const char* label) {
    std::string brake_str = s.brake_requested
        ? std::string("YES(") + std::to_string(int(s.brake_intensity * 100)) + "%)"
        : std::string("no");

    std::cout << "[t=" << std::setw(4) << t_ms << "ms] " << label << "\n"
              << "  speed="  << std::fixed << std::setprecision(1) << s.ego_speed_mps  << " m/s"
              << "  accel="  << std::setprecision(2) << s.ego_acceleration   << " m/s^2"
              << "  steer="  << s.steering_angle_rad << " rad"
              << "  brake="  << brake_str
              << "  DOW="    << (s.dow_warning ? "WARN" : "ok")
              << "\n";
}

int main() {
    std::cout << "ADAS Feature Suite - Simulator\n";
    std::cout << "==============================\n";

    // ── Scenario 1: Emergency Brake ──────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 30.0f;

        std::cout << "\n=== Scenario 1: Emergency Brake ===\n";
        std::cout << "Ego: 30 m/s (108 km/h) | Target: stopped | Distance: 20m\n";
        std::cout << "Expected: TTC = 0.67s -> AEB full brake\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{30.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{20.0f, 0.0f, 0.95f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.0f, 0.95f});  // centered
        mgr.execute(state, 1000);
        printState(state, 1000, "result");
        mgr.dtcManager().dump();
    }

    // ── Scenario 2: ACC Free Cruise ───────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 25.0f;

        std::cout << "\n=== Scenario 2: ACC Free Cruise ===\n";
        std::cout << "Ego: 25 m/s | No car ahead | Set speed: 33.3 m/s (120 km/h)\n";
        std::cout << "Expected: ACC accelerates\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{25.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{999.0f, 0.0f, 0.9f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.0f, 0.9f});
        mgr.execute(state, 2000);
        printState(state, 2000, "result");
        mgr.dtcManager().dump();
    }

    // ── Scenario 3: ACC Following Too Close ───────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 30.0f;

        std::cout << "\n=== Scenario 3: ACC Following - Too Close ===\n";
        std::cout << "Ego: 30 m/s | Target: 20 m/s | Gap: 10m\n";
        std::cout << "Expected: AEB fires (TTC=1s), ACC decelerates\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{30.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{10.0f, 20.0f, 0.9f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.0f, 0.9f});
        mgr.execute(state, 3000);
        printState(state, 3000, "result");
        mgr.dtcManager().dump();
    }

    // ── Scenario 4: Lane Departure ────────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 30.0f;

        std::cout << "\n=== Scenario 4: Lane Departure ===\n";
        std::cout << "Lateral deviation: +0.5m (drifting right)\n";
        std::cout << "Expected: LKA applies left steering correction\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{30.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{999.0f, 0.0f, 0.9f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.5f, 0.92f});
        mgr.execute(state, 4000);
        printState(state, 4000, "result");
        mgr.dtcManager().dump();
    }

    // ── Scenario 5: Door Open Warning ────────────────────────────────────────
    {
        AdasManager mgr;
        VehicleState state;
        state.ego_speed_mps = 0.0f;  // parked
        state.door_open     = true;

        std::cout << "\n=== Scenario 5: Door Open Warning ===\n";
        std::cout << "Parked | Door open | Cyclist at 8m approaching at 5 m/s\n";
        std::cout << "Expected: DOW warning active\n";

        mgr.publish(EventType::SPEED_UPDATE, SpeedData{0.0f});
        mgr.publish(EventType::RADAR_UPDATE, RadarData{8.0f, 5.0f, 0.9f});
        mgr.publish(EventType::LANE_UPDATE,  LaneData{0.0f, 0.9f});
        mgr.publish(EventType::DOOR_UPDATE,  DoorData{true});
        mgr.execute(state, 5000);
        printState(state, 5000, "result");
        mgr.dtcManager().dump();
    }

    std::cout << "\nSimulation complete.\n";
    return 0;
}
