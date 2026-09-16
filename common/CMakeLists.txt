add_library(common_physics STATIC
    src/render/DebugDraw.cpp
)

target_include_directories(common_physics PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common_physics PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
    OpenGL::GL
)
