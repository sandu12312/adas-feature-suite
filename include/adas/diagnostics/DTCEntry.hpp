#pragma once

#include <cstdint>
#include <string>
#include "adas/diagnostics/DTC.hpp"

namespace adas {
namespace diagnostics {

// A single diagnostic event recorded by the DTC manager.
struct DTCEntry {
    DTC         code;          // Which fault occurred
    Severity    severity;      // How critical it is
    uint64_t    timestamp_ms;  // When it was reported (milliseconds since start)
    std::string message;       // Human-readable description
};

} // namespace diagnostics
} // namespace adas
