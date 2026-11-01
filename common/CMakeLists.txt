add_library(common_physics INTERFACE)

target_include_directories(common_physics INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(common_physics INTERFACE
    GL_SILENCE_DEPRECATION
)

target_link_libraries(common_physics INTERFACE
    SDL3::SDL3
    OpenGL::GL
)
