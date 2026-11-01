#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <OpenGL/gl.h>
#include <iostream>

#include "render/GLDraw.hpp"

// ============================================================================
// GamePhysics Starter Template (C++20 + SDL3 + OpenGL on macOS)
// Book: Physics for Game Developers (2nd Edition) by Bourg & Bywalec
// ============================================================================

int main(int /*argc*/, char* /*argv*/[]) {
    // 1. Initialize SDL3 Video subsystem
    //    (Note: In SDL3, SDL_Init returns true on success and false on failure!)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[Error] SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // 2. Request a double-buffered OpenGL 2.1 Compatibility context
    //    (Allows immediate-mode glBegin/glEnd for clean, visual math/physics drawing)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    constexpr int kWindowWidth = 1280;
    constexpr int kWindowHeight = 720;

    // In SDL3, SDL_CreateWindow takes (title, width, height, flags) and shows by default
    SDL_Window* window = SDL_CreateWindow(
        "Physics for Game Developers - SDL3 + OpenGL Template",
        kWindowWidth,
        kWindowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "[Error] SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // 3. Create the OpenGL rendering context and enable VSync (60 FPS)
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "[Error] SDL_GL_CreateContext failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_SetSwapInterval(1);

    std::cout << "[GamePhysics] SDL3 + OpenGL initialized successfully.\n";
    std::cout << "[GamePhysics] Press ESC or close the window to exit.\n";

    // 4. Main Simulation & Render Loop
    bool running = true;
    Uint64 prevTicks = SDL_GetPerformanceCounter();
    const double perfFreq = static_cast<double>(SDL_GetPerformanceFrequency());

    int currentWidth = kWindowWidth;
    int currentHeight = kWindowHeight;
    SDL_GetWindowSizeInPixels(window, &currentWidth, &currentHeight);

    while (running) {
        // --- A. Poll Input & Window Events (SDL3 Event Names) ---
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running = false;
            } else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED ||
                       event.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSizeInPixels(window, &currentWidth, &currentHeight);
            }
        }

        // --- B. Compute Frame Delta Time (dt in seconds) ---
        Uint64 nowTicks = SDL_GetPerformanceCounter();
        float dt = static_cast<float>(static_cast<double>(nowTicks - prevTicks) / perfFreq);
        prevTicks = nowTicks;
        (void)dt; // Physics step will use dt starting in Chapter 1

        // --- C. Render Scene with OpenGL ---
        gl_draw::clearScreen();

        // Set up 2D Cartesian world coordinates (1 unit = 1 meter = 50 pixels)
        gl_draw::setup2DCartesianView(currentWidth, currentHeight, 50.0f);

        // Draw Cartesian grid and X/Y axes
        gl_draw::drawCartesianGrid(20, 1.0f);

        // Draw a sample origin marker (0, 0) and unit basis vectors i=(1,0), j=(0,1)
        gl_draw::drawCircle(0.0f, 0.0f, 0.12f, gl_draw::kWhite, true);
        gl_draw::drawArrow(0.0f, 0.0f, 2.0f, 0.0f, gl_draw::kAxisX);
        gl_draw::drawArrow(0.0f, 0.0f, 0.0f, 2.0f, gl_draw::kAxisY);

        // Swap front and back buffers
        SDL_GL_SwapWindow(window);
    }

    // 5. Clean up SDL3 & OpenGL resources
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
