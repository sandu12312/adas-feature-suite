cmake_minimum_required(VERSION 3.20)
project(adas_feature_suite VERSION 1.0.0 LANGUAGES CXX)

# ── C++ Standard ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Compiler warnings ─────────────────────────────────────────────────────────
add_compile_options(-Wall -Wextra -Wpedantic)

# ── Google Test (downloaded automatically, no install needed) ─────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# ── Include paths ─────────────────────────────────────────────────────────────
include_directories(include)

# ── Source files (we add here as we create them) ──────────────────────────────
set(ADAS_SOURCES
    # src/signals/SignalValidator.cpp       <- uncomment when you create it
    # src/events/EventBus.cpp               <- uncomment when you create it
    # src/diagnostics/DTCManager.cpp        <- uncomment when you create it
    # src/features/AebFeature.cpp           <- uncomment when you create it
    # src/features/AccFeature.cpp           <- uncomment when you create it
    # src/features/LkaFeature.cpp           <- uncomment when you create it
    # src/features/DowFeature.cpp           <- uncomment when you create it
    # src/features/AdasManager.cpp          <- uncomment when you create it
)

# ── Library target ────────────────────────────────────────────────────────────
# add_library(adas_lib ${ADAS_SOURCES})    <- uncomment when ADAS_SOURCES is not empty
# target_include_directories(adas_lib PUBLIC include)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()

# add_executable(adas_tests               <- uncomment when first test file exists
#     tests/test_signals.cpp
# )
# target_link_libraries(adas_tests adas_lib GTest::gtest_main)
# add_test(NAME adas_tests COMMAND adas_tests)

# ── Simulator ─────────────────────────────────────────────────────────────────
# add_executable(adas_simulator simulator/main.cpp)  <- uncomment at Phase 6
# target_link_libraries(adas_simulator adas_lib)
