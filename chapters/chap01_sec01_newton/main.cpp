#include "render/GLDraw.hpp"

// ============================================================================
// Chapter 1, Section 1.1: Newton's Laws of Motion (F = m * a & Inertia)
// ============================================================================

struct Puck {
    float x{-8.0f};             // Position in meters (m)
    float vx{0.0f};             // Velocity in meters/second (m/s)
    float invMass{1.0f / 2.0f}; // 2 kg mass -> stored as 1/m (0.5 kg^-1)
};

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Chap 1.1: Newton's Laws (Hold D/Right to thrust, Release to see Inertia)");

    Puck puck;
    constexpr float kDt = 1.0f / 60.0f; // 60 Hz physics step

    while (app.pollEvents()) {
        // 1. Read thruster input (Force in Newtons)
        float forceX = 0.0f;
        if (app.isKeyHeld(SDL_SCANCODE_D) || app.isKeyHeld(SDL_SCANCODE_RIGHT)) forceX =  10.0f;
        if (app.isKeyHeld(SDL_SCANCODE_A) || app.isKeyHeld(SDL_SCANCODE_LEFT))  forceX = -10.0f;

        // 2. Newton's 2nd Law: a = F / m = F * invMass
        //    Newton's 1st Law: When forceX == 0, ax == 0, so vx stays constant (Inertia!)
        float ax = forceX * puck.invMass;

        // 3. Update velocity and position
        puck.vx += ax * kDt;
        puck.x  += puck.vx * kDt;

        if (puck.x >  12.0f) puck.x = -12.0f;
        if (puck.x < -12.0f) puck.x =  12.0f;

        // 4. Draw Puck, Vectors, and Live On-Screen Metrics
        app.beginFrame();
        gl_draw::drawCircle(puck.x, 0.0f, 0.6f, gl_draw::kCyan, true);
        gl_draw::drawArrow(puck.x, 0.0f, puck.x + puck.vx * 0.4f, 0.0f, gl_draw::kWhite);
        gl_draw::drawArrow(puck.x, 0.8f, puck.x + forceX * 0.25f, 0.8f, gl_draw::kYellow);

        // Floating label right below the puck + Top-Left HUD metrics
        gl_draw::drawTextWorld(puck.x - 1.2f, -1.2f, gl_draw::kCyan, "x=%.1fm v=%.1fm/s", puck.x, puck.vx);
        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kWhite,
            "Mass: 2.0 kg (invMass = 0.5)\nForce (F): %.1f N\nAccel (a = F*invMass): %.1f m/s^2\nSpeed (vx): %.2f m/s\nPos (x): %.2f m",
            forceX, ax, puck.vx, puck.x);
        app.endFrame();
    }
    return 0;
}
