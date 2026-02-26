#include <gtest/gtest.h>
#include "adas/signals/Signal.hpp"
#include "adas/signals/SignalStatus.hpp"
#include "adas/signals/SignalValidator.hpp"

using namespace adas::signals;

TEST(SignalValidator, ValidSignal) {
    SignalValidator v(0.0f, 200.0f, 0.6f, 200);
    Signal<float> s;
    s.value        = 50.0f;
    s.confidence   = 0.9f;
    s.timestamp_ms = 100;
    v.validate(s, 150);
    EXPECT_EQ(s.status, SignalStatus::VALID);
}

TEST(SignalValidator, TimeoutSignal) {
    SignalValidator v(0.0f, 200.0f, 0.6f, 200);
    Signal<float> s;
    s.value        = 50.0f;
    s.confidence   = 0.9f;
    s.timestamp_ms = 0;
    v.validate(s, 500);  // 500ms old, limit is 200ms
    EXPECT_EQ(s.status, SignalStatus::TIMEOUT);
}

TEST(SignalValidator, OutOfRangeSignal) {
    SignalValidator v(0.0f, 200.0f, 0.6f, 200);
    Signal<float> s;
    s.value        = -5.0f;  // below physical minimum
    s.confidence   = 0.9f;
    s.timestamp_ms = 100;
    v.validate(s, 150);
    EXPECT_EQ(s.status, SignalStatus::OUT_OF_RANGE);
}

TEST(SignalValidator, LowConfidenceSignal) {
    SignalValidator v(0.0f, 200.0f, 0.6f, 200);
    Signal<float> s;
    s.value        = 50.0f;
    s.confidence   = 0.3f;  // below minimum confidence
    s.timestamp_ms = 100;
    v.validate(s, 150);
    EXPECT_EQ(s.status, SignalStatus::LOW_CONFIDENCE);
}

TEST(SignalValidator, TimeoutTakesPriorityOverRange) {
    SignalValidator v(0.0f, 200.0f, 0.6f, 200);
    Signal<float> s;
    s.value        = -5.0f;  // also out of range
    s.confidence   = 0.9f;
    s.timestamp_ms = 0;
    v.validate(s, 500);
    EXPECT_EQ(s.status, SignalStatus::TIMEOUT);  // timeout checked first
}
