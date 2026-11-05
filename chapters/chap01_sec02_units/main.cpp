#include "render/GLDraw.hpp"

// ============================================================================
// Chapter 1, Section 1.2: Units, Dimensional Consistency & Fluid Drag (R_f)
// Book Formula: R_f = 0.5 * rho * V^2 * S * C_f  (Units: kg*m/s^2 = Newtons)
// ============================================================================

struct Boat {
    float x{-9.0f};             // Position (m)
    float vx{0.0f};             // Velocity (m/s)
    float invMass{1.0f / 4.0f}; // 4.0 kg boat -> invMass = 0.25 kg^-1
};

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Chap 1.2: Units & Fluid Drag R_f (Hold D/Right for +16N Thrust)");

    Boat boat;
    constexpr float kRho = 1000.0f; // Water density (kg/m^3)
    constexpr float kS   = 0.10f;   // Wetted hull area (m^2)
    constexpr float kCf  = 0.02f;   // Skin friction coefficient (dimensionless)
    constexpr float kDt  = 1.0f / 60.0f;

    while (app.pollEvents()) {
        // 1. Engine thrust force in Newtons (N)
        float thrustN = 0.0f;
        if (app.isKeyHeld(SDL_SCANCODE_D) || app.isKeyHeld(SDL_SCANCODE_RIGHT)) thrustN =  16.0f;
        if (app.isKeyHeld(SDL_SCANCODE_A) || app.isKeyHeld(SDL_SCANCODE_LEFT))  thrustN = -16.0f;

        // 2. Book Section 1.2 Fluid Drag: R_f = -0.5 * rho * vx * |vx| * S * C_f
        //    Using (-vx * std::abs(vx)) makes drag always point OPPOSITE to motion!
        float dragN = -0.5f * kRho * boat.vx * std::abs(boat.vx) * kS * kCf;

        // 3. Net Force & Newton's 2nd Law (both thrustN and dragN are in Newtons!)
        float netForceN = thrustN + dragN;
        float ax = netForceN * boat.invMass;

        boat.vx += ax * kDt;
        boat.x  += boat.vx * kDt;
        if (boat.x > 12.0f) boat.x = -12.0f;
        if (boat.x < -12.0f) boat.x = 12.0f;

        // 4. Render Boat, Force Arrows (Yellow=Thrust, Red=Drag), and Metrics
        app.beginFrame();
        gl_draw::drawCircle(boat.x, 0.0f, 0.65f, gl_draw::kCyan, true);
        gl_draw::drawArrow(boat.x,  0.85f, boat.x + thrustN * 0.2f, 0.85f, gl_draw::kYellow);
        gl_draw::drawArrow(boat.x, -0.85f, boat.x + dragN   * 0.2f, -0.85f, gl_draw::kAxisX);
        gl_draw::drawArrow(boat.x,  0.0f,  boat.x + boat.vx * 0.5f, 0.0f,  gl_draw::kWhite);

        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kWhite,
            "Thrust: %+.1f N (Yellow) | Water Drag R_f: %+.2f N (Red)\n"
            "Net Force: %+.2f N -> Accel: %+.2f m/s^2\n"
            "Speed V: %.2f m/s (Terminal Max Speed at +16N = 4.00 m/s)",
            thrustN, dragN, netForceN, ax, boat.vx);
        app.endFrame();
    }
    return 0;
}
