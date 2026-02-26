#pragma once

#include <vector>
#include "adas/diagnostics/DTCEntry.hpp"

namespace adas {
namespace diagnostics {

// Collects and stores diagnostic trouble codes reported by ADAS features.
class DTCManager {
public:
    // Record a new DTC event.
    void report(DTC code, Severity severity,
                const std::string& message, uint64_t timestamp_ms);

    // Remove all active entries for the given code.
    void clear(DTC code);

    // Returns true if the given code has at least one active entry.
    bool hasActive(DTC code) const;

    // Read-only access to the full log.
    const std::vector<DTCEntry>& entries() const;

    // Print all entries to stdout.
    void dump() const;

private:
    std::vector<DTCEntry> entries_;
};

} // namespace diagnostics
} // namespace adas
