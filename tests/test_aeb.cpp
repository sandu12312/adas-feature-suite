#include <gtest/gtest.h>
#include "adas/features/AebFeature.hpp"
#include "adas/diagnostics/DTCManager.hpp"
#include "adas/VehicleState.hpp"

using namespace adas::features;
using namespace adas::events;
using namespace adas::diagnostics;

TEST(AebFeature, FullBrakeOnCriticalTTC) {
    AebFeature aeb;
    // ego 30 m/s, target stopped, distance 30m → TTC = 1.0s < 1.5s
    aeb.onEvent(EventType::SPEED_UPDATE,  SpeedData{30.0f});
    aeb.onEvent(EventType::RADAR_UPDATE,  RadarData{30.0f, 0.0f, 0.9f});
    adas::VehicleState state;
    DTCManager dtc;
    aeb.execute(state, dtc, 0);
    EXPECT_TRUE(state.brake_requested);
    EXPECT_FLOAT_EQ(state.brake_intensity, 1.0f);
}

TEST(AebFeature, PartialBrakeOnWarningTTC) {
    AebFeature aeb;
    // ego 30 m/s, target stopped, distance 60m → TTC = 2.0s  (1.5 < 2.0 < 3.0)
    aeb.onEvent(EventType::SPEED_UPDATE,  SpeedData{30.0f});
    aeb.onEvent(EventType::RADAR_UPDATE,  RadarData{60.0f, 0.0f, 0.9f});
    adas::VehicleState state;
    DTCManager dtc;
    aeb.execute(state, dtc, 0);
    EXPECT_TRUE(state.brake_requested);
    EXPECT_GT(state.brake_intensity, 0.0f);
    EXPECT_LT(state.brake_intensity, 1.0f);
}

TEST(AebFeature, NoBrakeWhenClear) {
    AebFeature aeb;
    // ego 30 m/s, target also 30 m/s (same speed) — closing speed = 0
    aeb.onEvent(EventType::SPEED_UPDATE,  SpeedData{30.0f});
    aeb.onEvent(EventType::RADAR_UPDATE,  RadarData{50.0f, 30.0f, 0.9f});
    adas::VehicleState state;
    DTCManager dtc;
    aeb.execute(state, dtc, 0);
    EXPECT_FALSE(state.brake_requested);
}

TEST(AebFeature, NoBrakeOnLowConfidence) {
    AebFeature aeb;
    aeb.onEvent(EventType::SPEED_UPDATE,  SpeedData{30.0f});
    aeb.onEvent(EventType::RADAR_UPDATE,  RadarData{20.0f, 0.0f, 0.2f});  // low confidence
    adas::VehicleState state;
    DTCManager dtc;
    aeb.execute(state, dtc, 0);
    EXPECT_FALSE(state.brake_requested);
}
