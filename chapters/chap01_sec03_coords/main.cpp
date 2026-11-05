#include "render/GLDraw.hpp"

// ============================================================================
// Chapter 1, Section 1.3: Coordinate Systems
//   1. SDL3 Screen Pixels: Origin (0,0) at Top-Left, +X Right, +Y DOWN
//   2. Global World Space: Origin (0,0) at Center,   +X Right, +Y UP (Meters)
//   3. Body Local Space  : Origin (0,0) at Ship Center (local = world - ship)
// ============================================================================

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Chap 1.3: Coordinate Systems (Move Ship with WASD, Aim with Mouse)");

    float shipX = -3.0f, shipY = -1.0f;
    constexpr float kPixelsPerMeter = 50.0f;
    constexpr float kDt = 1.0f / 60.0f;

    while (app.pollEvents()) {
        // 1. Move Ship in Global World Coordinates (meters) with WASD
        if (app.isKeyHeld(SDL_SCANCODE_D)) shipX += 4.0f * kDt;
        if (app.isKeyHeld(SDL_SCANCODE_A)) shipX -= 4.0f * kDt;
        if (app.isKeyHeld(SDL_SCANCODE_W)) shipY += 4.0f * kDt;
        if (app.isKeyHeld(SDL_SCANCODE_S)) shipY -= 4.0f * kDt;

        // 2. Read Mouse in SDL3 Screen Pixels (Top-Left is 0,0 and +Y goes DOWN!)
        float mousePxX = 0.0f, mousePxY = 0.0f;
        SDL_GetMouseState(&mousePxX, &mousePxY);

        int winW = 1280, winH = 720;
        SDL_GetWindowSize(app.window, &winW, &winH);

        // 3. Convert Screen Pixels -> Global World Meters (Center is 0,0 and +Y goes UP!)
        float targetWorldX = (mousePxX - (winW * 0.5f)) / kPixelsPerMeter;
        float targetWorldY = ((winH * 0.5f) - mousePxY) / kPixelsPerMeter;

        // 4. Convert Global World -> Ship Local Coordinates (Relative to Ship!)
        float targetLocalX = targetWorldX - shipX;
        float targetLocalY = targetWorldY - shipY;

        app.beginFrame(kPixelsPerMeter);
        // Draw Ship's Local Axes (Cyan crosshairs centered on the ship)
        gl_draw::drawLine(shipX - 2.0f, shipY, shipX + 2.0f, shipY, gl_draw::kCyan, 1.5f);
        gl_draw::drawLine(shipX, shipY - 2.0f, shipX, shipY + 2.0f, gl_draw::kCyan, 1.5f);
        gl_draw::drawCircle(shipX, shipY, 0.45f, gl_draw::kCyan, true);

        // Draw Mouse Target & Local Relative Arrow from Ship to Target
        gl_draw::drawArrow(shipX, shipY, targetWorldX, targetWorldY, gl_draw::kYellow);
        gl_draw::drawCircle(targetWorldX, targetWorldY, 0.20f, gl_draw::kYellow, true);

        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kWhite,
            "1. SDL3 Screen Pixels : (%.0f px, %.0f px) [Origin Top-Left, +Y Down]\n"
            "2. Global World Space : (%+.2f m, %+.2f m) [Origin Center, +Y Up]\n"
            "3. Ship Local Space   : (%+.2f m, %+.2f m) [Target relative to Ship]",
            mousePxX, mousePxY, targetWorldX, targetWorldY, targetLocalX, targetLocalY);
        app.endFrame();
    }
    return 0;
}
