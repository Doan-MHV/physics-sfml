cmake_minimum_required(VERSION 3.24)
project(GamePhysics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fallback search path on macOS when building without the vcpkg toolchain flag
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")
endif()

find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Shared math & OpenGL visual helpers
add_subdirectory(common)

# Chapter-by-chapter section examples (Instructor demos)
add_subdirectory(chapters)

# Section-by-section fill-in-the-logic homeworks
add_subdirectory(homeworks)

# End-of-chapter open-ended architecture challenges
add_subdirectory(challenges)

# Root starter template executable
add_executable(GamePhysics main.cpp)
target_link_libraries(GamePhysics PRIVATE common_physics)
