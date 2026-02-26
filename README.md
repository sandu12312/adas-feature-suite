# ADAS Feature Suite

![CI](https://github.com/sandu12312/adas-feature-suite-1/actions/workflows/ci.yml/badge.svg)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

A C++17 implementation of core ADAS features built on a layered architecture with signal validation, event-driven communication and diagnostic reporting — matching the software stack used in production automotive ECUs.

## Features implemented

| Feature | Description |
|---------|-------------|
| **AEB** — Automatic Emergency Braking | Calculates Time-To-Collision; triggers full or partial brake below configurable thresholds |
| **ACC** — Adaptive Cruise Control | Maintains driver-set speed; follows vehicle ahead with proportional gap control |
| **LKA** — Lane Keep Assist | Applies proportional steering correction when lateral deviation exceeds threshold |
| **DOW** — Door Open Warning | Alerts driver when a vehicle approaches while a door is open |

## Architecture

```
Simulator
    │  (injects validated Signal<T> data)
    ▼
Signal Layer  ──  SignalValidator checks range / timeout / confidence
    │
    ▼
Event Bus  ──  publish(EventType, EventData) → routes to subscribed features
    │
    ├──▶ AEB Feature  ──▶ VehicleState.brake_requested
    ├──▶ ACC Feature  ──▶ VehicleState.ego_acceleration
    ├──▶ LKA Feature  ──▶ VehicleState.steering_angle_rad
    └──▶ DOW Feature  ──▶ VehicleState.dow_warning
                │
                ▼
           DTC Manager  ──  collects fault codes from all features
```

## Build

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Run tests

```bash
cd build && ctest --output-on-failure
```

## Run simulator

```bash
./build/adas_simulator
```

The simulator runs five scenarios: emergency brake, ACC free cruise, ACC following, lane departure, and door open warning.

## Tech stack

- **Language**: C++17
- **Build**: CMake 3.20+ with Ninja
- **Tests**: Google Test (fetched automatically via FetchContent)
- **Style**: Google C++ style guide, enforced via clang-format
- **CI**: GitHub Actions — build and test on every push
