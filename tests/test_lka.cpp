#include <gtest/gtest.h>
#include "adas/features/LkaFeature.hpp"
#include "adas/diagnostics/DTCManager.hpp"
#include "adas/VehicleState.hpp"

using namespace adas::features;
using namespace adas::events;
using namespace adas::diagnostics;

TEST(LkaFeature, CorrectsDriftToRight) {
    LkaFeature lka;
    lka.onEvent(EventType::LANE_UPDATE, LaneData{0.5f, 0.9f});  // drifting right
    adas::VehicleState state;
    DTCManager dtc;
    lka.execute(state, dtc, 0);
    EXPECT_LT(state.steering_angle_rad, 0.0f);  // corrects left
}

TEST(LkaFeature, NoCorrectionsWhenCentered) {
    LkaFeature lka;
    lka.onEvent(EventType::LANE_UPDATE, LaneData{0.1f, 0.9f});  // within threshold
    adas::VehicleState state;
    DTCManager dtc;
    lka.execute(state, dtc, 0);
    EXPECT_FLOAT_EQ(state.steering_angle_rad, 0.0f);
}

TEST(LkaFeature, NoCorrectionsOnLowConfidence) {
    LkaFeature lka;
    lka.onEvent(EventType::LANE_UPDATE, LaneData{0.8f, 0.2f});  // low camera confidence
    adas::VehicleState state;
    DTCManager dtc;
    lka.execute(state, dtc, 0);
    EXPECT_FLOAT_EQ(state.steering_angle_rad, 0.0f);
}
