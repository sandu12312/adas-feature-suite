#include <gtest/gtest.h>
#include "adas/features/DowFeature.hpp"
#include "adas/diagnostics/DTCManager.hpp"
#include "adas/VehicleState.hpp"

using namespace adas::features;
using namespace adas::events;
using namespace adas::diagnostics;

TEST(DowFeature, WarningWhenApproachingWithDoorOpen) {
    DowFeature dow;
    dow.onEvent(EventType::DOOR_UPDATE,   DoorData{true});
    dow.onEvent(EventType::RADAR_UPDATE,  RadarData{8.0f, 5.0f, 0.9f});  // 8m, 18 km/h
    adas::VehicleState state;
    DTCManager dtc;
    dow.execute(state, dtc, 0);
    EXPECT_TRUE(state.dow_warning);
}

TEST(DowFeature, NoWarningWhenDoorClosed) {
    DowFeature dow;
    dow.onEvent(EventType::DOOR_UPDATE,   DoorData{false});
    dow.onEvent(EventType::RADAR_UPDATE,  RadarData{8.0f, 5.0f, 0.9f});
    adas::VehicleState state;
    DTCManager dtc;
    dow.execute(state, dtc, 0);
    EXPECT_FALSE(state.dow_warning);
}

TEST(DowFeature, NoWarningWhenVehicleFar) {
    DowFeature dow;
    dow.onEvent(EventType::DOOR_UPDATE,   DoorData{true});
    dow.onEvent(EventType::RADAR_UPDATE,  RadarData{50.0f, 5.0f, 0.9f}); // far away
    adas::VehicleState state;
    DTCManager dtc;
    dow.execute(state, dtc, 0);
    EXPECT_FALSE(state.dow_warning);
}
