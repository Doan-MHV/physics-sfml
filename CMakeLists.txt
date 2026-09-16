cmake_minimum_required(VERSION 3.24)
project(GamePhysics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 COMPONENTS Graphics Window System CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# Common shared physics library (math, debug rendering, utils)
add_subdirectory(common)

# Individual chapter visual projects
add_subdirectory(chapters)

# Root executable target
add_executable(GamePhysics main.cpp)
target_link_libraries(GamePhysics PRIVATE common_physics)
