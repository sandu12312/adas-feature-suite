#pragma once

namespace adas {
namespace diagnostics {

// Diagnostic Trouble Codes reported by ADAS features when faults are detected.
enum class DTC {
    AEB_SENSOR_FAULT    = 0x1001,  // AEB could not get valid sensor data
    AEB_ACTIVATED       = 0x1002,  // AEB triggered an emergency brake
    ACC_SENSOR_FAULT    = 0x1003,  // ACC could not get valid sensor data
    LKA_LOW_CONFIDENCE  = 0x1004,  // Camera confidence too low for lane assist
    DOW_SENSOR_FAULT    = 0x1005,  // DOW could not get valid radar data
    DOW_WARNING_ACTIVE  = 0x1006   // Vehicle approaching while door is open
};

// Indicates how critical a reported DTC is.
enum class Severity {
    INFO,      // Normal operational event (e.g. feature activated as intended)
    WARNING,   // Degraded operation — feature still running but with limitations
    CRITICAL   // Feature disabled — safe state entered
};

} // namespace diagnostics
} // namespace adas
