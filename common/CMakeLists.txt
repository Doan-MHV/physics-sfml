add_library(common_physics INTERFACE)

target_include_directories(common_physics INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common_physics INTERFACE
        SFML::Graphics
        SFML::Window
        SFML::System
        OpenGL::GL
)
