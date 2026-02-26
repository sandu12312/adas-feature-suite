#include "adas/diagnostics/DTCManager.hpp"
#include <algorithm>
#include <iostream>

namespace adas {
namespace diagnostics {

void DTCManager::report(DTC code, Severity severity,
                        const std::string& message, uint64_t timestamp_ms) {
    entries_.push_back({code, severity, timestamp_ms, message});
}

void DTCManager::clear(DTC code) {
    entries_.erase(
        std::remove_if(entries_.begin(), entries_.end(),
                       [code](const DTCEntry& e) { return e.code == code; }),
        entries_.end());
}

bool DTCManager::hasActive(DTC code) const {
    return std::any_of(entries_.begin(), entries_.end(),
                       [code](const DTCEntry& e) { return e.code == code; });
}

const std::vector<DTCEntry>& DTCManager::entries() const {
    return entries_;
}

void DTCManager::dump() const {
    for (const auto& e : entries_) {
        const char* sev = (e.severity == Severity::INFO)     ? "INFO"
                        : (e.severity == Severity::WARNING)  ? "WARN"
                                                             : "CRIT";
        std::cout << "[DTC][" << sev << "][t=" << e.timestamp_ms << "ms] "
                  << e.message << "\n";
    }
}

} // namespace diagnostics
} // namespace adas
