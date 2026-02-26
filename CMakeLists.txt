cmake_minimum_required(VERSION 3.20)
project(adas_feature_suite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

# ── Google Test ───────────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# ── Library ───────────────────────────────────────────────────────────────────
add_library(adas_lib
    src/signals/SignalValidator.cpp
    src/events/EventBus.cpp
    src/diagnostics/DTCManager.cpp
    src/features/AebFeature.cpp
    src/features/AccFeature.cpp
    src/features/LkaFeature.cpp
    src/features/DowFeature.cpp
    src/features/AdasManager.cpp
)
target_include_directories(adas_lib PUBLIC include)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()

add_executable(adas_tests
    tests/test_signals.cpp
    tests/test_eventbus.cpp
    tests/test_aeb.cpp
    tests/test_acc.cpp
    tests/test_lka.cpp
    tests/test_dow.cpp
)
target_link_libraries(adas_tests adas_lib GTest::gtest_main)
add_test(NAME adas_tests COMMAND adas_tests)

# ── Simulator ─────────────────────────────────────────────────────────────────
add_executable(adas_simulator simulator/main.cpp)
target_link_libraries(adas_simulator adas_lib)
