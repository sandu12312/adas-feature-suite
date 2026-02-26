#include <gtest/gtest.h>
#include "adas/features/AccFeature.hpp"
#include "adas/diagnostics/DTCManager.hpp"
#include "adas/VehicleState.hpp"

using namespace adas::features;
using namespace adas::events;
using namespace adas::diagnostics;

TEST(AccFeature, AcceleratesOnClearRoad) {
    AccFeature acc(33.33f);  // set speed 120 km/h
    acc.onEvent(EventType::SPEED_UPDATE,  SpeedData{20.0f});   // currently 72 km/h
    acc.onEvent(EventType::RADAR_UPDATE,  RadarData{999.0f, 0.0f, 0.9f}); // no car ahead
    adas::VehicleState state;
    DTCManager dtc;
    acc.execute(state, dtc, 0);
    EXPECT_GT(state.ego_acceleration, 0.0f);  // should accelerate
}

TEST(AccFeature, DeceleratesWhenTooClose) {
    AccFeature acc(33.33f);
    acc.onEvent(EventType::SPEED_UPDATE,  SpeedData{30.0f});
    acc.onEvent(EventType::RADAR_UPDATE,  RadarData{10.0f, 5.0f, 0.9f}); // 10m gap, too close
    adas::VehicleState state;
    DTCManager dtc;
    acc.execute(state, dtc, 0);
    EXPECT_LT(state.ego_acceleration, 0.0f);  // should slow down
}
