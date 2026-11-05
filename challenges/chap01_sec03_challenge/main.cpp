#include "render/GLDraw.hpp"

// ============================================================================
// ASSIGNMENT: Chapter 1, Section 1.3 — Screen, World & Local Spaces
// ============================================================================
//
// SCENARIO OVERVIEW:
//   You are piloting a Cyan Warship in Global World Space (move with WASD).
//   Bolted onto your warship's hull are two hardpoints defined in the ship's
//   own Body-Fixed Local Coordinate System:
//     1. Front Radar Dish (Green) : Local offset (+1.5 m,  0.0 m)
//     2. Rear Gun Turret  (Orange): Local offset (-1.0 m, +0.8 m)
//
//   Meanwhile, you aim at an enemy target using your mouse cursor, which SDL3
//   reports in Screen Pixel Coordinates (top-left origin, +Y pointing down).
//
// YOUR TASKS:
//   - Task 1 (screenToWorld): Convert SDL3 screen pixels (pxX, pxY) into
//     Cartesian Global World meters (worldX, worldY).
//   - Task 2 (localToWorld): Convert a hardpoint's Ship Local offset
//     (localX, localY) into Global World meters so OpenGL draws the Radar Dish
//     and Rear Turret attached to the moving ship!
//   - Task 3 (worldToLocal): Convert the enemy's Global World position into
//     the Rear Gun Turret's Local Space so the turret knows the enemy's offset!
//
// EXPECTED END RESULT (How to verify your solution works):
//   1. Move the ship with [W][A][S][D]:
//        - The Green Radar Dish (+1.5m, 0.0m) and Orange Turret (-1.0m, +0.8m)
//          must stay locked onto the ship hull and move together with it!
//   2. Move your mouse cursor anywhere in the window:
//        - The Yellow Target circle must sit directly underneath your mouse
//          cursor (even when moving up/down, proving +Y is flipped properly!).
//        - A Yellow aiming arrow must stretch from the Orange Rear Turret
//          directly to your mouse cursor, and the HUD must display the exact
//          local offset from the Turret to the Target!
// ============================================================================

struct Coord2D {
    float x{0.0f};
    float y{0.0f};
};

// TODO 1: Convert SDL3 Screen Pixels (pxX, pxY) -> Global World Meters (x, y)
//         Hint: halfW = winW * 0.5f, halfH = winH * 0.5f
Coord2D screenToWorld(float pxX, float pxY, int winW, int winH, float pixelsPerMeter) {
    (void)pxX; (void)pxY; (void)winW; (void)winH; (void)pixelsPerMeter;
    return {0.0f, 0.0f}; // <-- Replace this!
}

// TODO 2: Convert Ship Local Offset (local) -> Global World Position (world = shipWorld + local)
Coord2D localToWorld(Coord2D shipWorld, Coord2D localOffset) {
    (void)shipWorld; (void)localOffset;
    return {0.0f, 0.0f}; // <-- Replace this!
}

// TODO 3: Convert Target Global World Position -> Local Position relative to an origin
//         (local = targetWorld - originWorld)
Coord2D worldToLocal(Coord2D originWorld, Coord2D targetWorld) {
    (void)originWorld; (void)targetWorld;
    return {0.0f, 0.0f}; // <-- Replace this!
}

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Section 1.3 Challenge: Move Ship (WASD) & Aim Turret with Mouse");

    Coord2D shipWorld{-3.0f, -1.0f};
    const Coord2D radarLocalOffset{1.5f, 0.0f};   // 1.5m in front of ship center
    const Coord2D turretLocalOffset{-1.0f, 0.8f}; // 1.0m behind, 0.8m above ship center

    constexpr float kPixelsPerMeter = 50.0f;
    constexpr float kDt = 1.0f / 60.0f;

    while (app.pollEvents()) {
        if (app.isKeyHeld(SDL_SCANCODE_D)) shipWorld.x += 4.0f * kDt;
        if (app.isKeyHeld(SDL_SCANCODE_A)) shipWorld.x -= 4.0f * kDt;
        if (app.isKeyHeld(SDL_SCANCODE_W)) shipWorld.y += 4.0f * kDt;
        if (app.isKeyHeld(SDL_SCANCODE_S)) shipWorld.y -= 4.0f * kDt;

        float mousePxX = 0.0f, mousePxY = 0.0f;
        SDL_GetMouseState(&mousePxX, &mousePxY);
        int winW = 1280, winH = 720;
        SDL_GetWindowSize(app.window, &winW, &winH);

        Coord2D targetWorld = screenToWorld(mousePxX, mousePxY, winW, winH, kPixelsPerMeter);
        Coord2D radarWorld  = localToWorld(shipWorld, radarLocalOffset);
        Coord2D turretWorld = localToWorld(shipWorld, turretLocalOffset);
        Coord2D targetRelToTurret = worldToLocal(turretWorld, targetWorld);

        app.beginFrame(kPixelsPerMeter);
        // Draw Ship hull (Cyan) + attached Radar (Green) + attached Turret (Orange)
        gl_draw::drawCircle(shipWorld.x,   shipWorld.y,   0.70f, gl_draw::kCyan,   false);
        gl_draw::drawCircle(radarWorld.x,  radarWorld.y,  0.22f, gl_draw::kAxisY,  true);
        gl_draw::drawCircle(turretWorld.x, turretWorld.y, 0.28f, gl_draw::kOrange, true);

        // Draw aiming arrow from Rear Turret to Mouse Target
        gl_draw::drawArrow(turretWorld.x, turretWorld.y, targetWorld.x, targetWorld.y, gl_draw::kYellow);
        gl_draw::drawCircle(targetWorld.x, targetWorld.y, 0.18f, gl_draw::kYellow, true);

        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kWhite,
            "Ship World: (%+.1fm, %+.1fm) | Turret World: (%+.1fm, %+.1fm)\n"
            "Mouse Target World: (%+.2fm, %+.2fm) | Target in Turret Local Space: (%+.2fm, %+.2fm)",
            shipWorld.x, shipWorld.y, turretWorld.x, turretWorld.y,
            targetWorld.x, targetWorld.y, targetRelToTurret.x, targetRelToTurret.y);
        app.endFrame();
    }
    return 0;
}
