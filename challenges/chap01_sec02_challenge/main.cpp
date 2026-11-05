#include "render/GLDraw.hpp"

// ============================================================================
// ASSIGNMENT: Chapter 1, Section 1.2 — Dimensional Units & Hull Drag Race
// ============================================================================
//
// SCENARIO OVERVIEW:
//   Two 4.0 kg submarines race horizontally through seawater (rho = 1000 kg/m^3,
//   friction coefficient C_f = 0.02):
//     1. Top Blue Sub    (y = +2.5 m): Streamlined hull with area S = 0.05 m^2
//     2. Bottom Orange Sub (y = -2.5 m): Bulky cargo hull with area S = 0.20 m^2
//                                        (4x larger surface area!)
//
//   When you hold [SPACE], both submarines fire identical +36.0 N engines.
//
// YOUR TASKS:
//   - Task 1 (calcDragFactorK): Pre-compute the constant drag factor
//       k = 0.5f * rho * areaS * coeffCf   (Units: kg/m)
//     so the game loop doesn't multiply 4 constants every single frame!
//   - Task 2 (calcDragForceN): Compute the opposing drag force in Newtons:
//       dragForceN = -sub.dragK * sub.vx * std::abs(sub.vx)
//   - Task 3 (stepSubmarine): Combine thrustN + dragForceN into netForceN,
//     compute acceleration (ax = netForceN * sub.invMass), and step vx and x.
//
// EXPECTED END RESULT (How to verify your solution works):
//   1. At startup, the top HUD must show:
//        dragK(Blue) = 0.50 kg/m,   dragK(Orange) = 2.00 kg/m
//   2. Hold [SPACE] (+36 N thrust on both subs):
//        - The Top Blue Sub (low drag) accelerates all the way up to a
//          Terminal Velocity of +8.49 m/s (where Drag = -36 N).
//        - The Bottom Orange Sub (4x hull area) hits a wall of water drag early
//          and tops out at +4.24 m/s (exactly HALF the top speed of the Blue Sub!).
//   3. Release [SPACE]:
//        - Unlike Section 1.1 in empty space, the red drag arrows immediately
//          slow both submarines down toward 0.0 m/s!
// ============================================================================

struct Submarine {
    float x{-10.0f};
    float y{0.0f};
    float vx{0.0f};
    float invMass{1.0f / 4.0f};  // 4 kg mass
    float dragK{0.0f};           // Pre-computed 0.5 * rho * S * C_f (kg/m)
    float lastDragN{0.0f};       // Stored for drawing the red drag arrow
};

// TODO 1: Return k = 0.5f * rho * areaS * coeffCf
float calcDragFactorK(float rho, float areaS, float coeffCf) {
    return 0.5f * rho * areaS * coeffCf;  // <-- Replace this!
}

// TODO 2: Return opposing drag force in Newtons: -sub.dragK * sub.vx * std::abs(sub.vx)
float calcDragForceN(const Submarine& sub) {
    return -sub.dragK * sub.vx * std::abs(sub.vx);  // <-- Replace this!
}

// TODO 3: Compute lastDragN (using calcDragForceN), netForceN = thrustN + lastDragN,
//         ax = netForceN * sub.invMass, and update sub.vx and sub.x by dt!
void stepSubmarine(Submarine& sub, float thrustN, float dt) {
    sub.lastDragN = calcDragForceN(sub);
    float netForceN = thrustN + sub.lastDragN;
    float ax = netForceN * sub.invMass;
    sub.vx += ax * dt;
    sub.x += sub.vx * dt;
}

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Section 1.2 Challenge: Submarine Drag Race (Hold SPACE to thrust, R to reset)");

    Submarine fastSub{-10.0f, 2.5f, 0.0f, 0.25f, calcDragFactorK(1000.0f, 0.05f, 0.02f), 0.0f};
    Submarine slowSub{-10.0f, -2.5f, 0.0f, 0.25f, calcDragFactorK(1000.0f, 0.20f, 0.02f), 0.0f};
    constexpr float kDt = 1.0f / 60.0f;

    while (app.pollEvents()) {
        if (app.isKeyHeld(SDL_SCANCODE_R)) {
            fastSub.x = -10.0f;
            fastSub.vx = 0.0f;
            slowSub.x = -10.0f;
            slowSub.vx = 0.0f;
        }

        float thrustN = app.isKeyHeld(SDL_SCANCODE_SPACE) ? 36.0f : 0.0f;

        stepSubmarine(fastSub, thrustN, kDt);
        stepSubmarine(slowSub, thrustN, kDt);
        if (fastSub.x > 12.0f) fastSub.x = -12.0f;
        if (slowSub.x > 12.0f) slowSub.x = -12.0f;

        app.beginFrame();
        gl_draw::drawCircle(fastSub.x, fastSub.y, 0.55f, gl_draw::kCyan, true);
        gl_draw::drawArrow(fastSub.x, fastSub.y - 0.7f, fastSub.x + fastSub.lastDragN * 0.1f, fastSub.y - 0.7f,
                           gl_draw::kAxisX);
        gl_draw::drawTextWorld(fastSub.x - 1.5f, fastSub.y + 0.8f, gl_draw::kCyan, "V=%.2fm/s Drag=%.1fN", fastSub.vx,
                               fastSub.lastDragN);

        gl_draw::drawCircle(slowSub.x, slowSub.y, 0.80f, gl_draw::kOrange, true);
        gl_draw::drawArrow(slowSub.x, slowSub.y - 1.0f, slowSub.x + slowSub.lastDragN * 0.1f, slowSub.y - 1.0f,
                           gl_draw::kAxisX);
        gl_draw::drawTextWorld(slowSub.x - 1.5f, slowSub.y + 1.0f, gl_draw::kOrange, "V=%.2fm/s Drag=%.1fN", slowSub.vx,
                               slowSub.lastDragN);

        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kYellow,
                               "Hold SPACE (+36N Thrust) | dragK(Blue)=%.2f, dragK(Orange)=%.2f", fastSub.dragK,
                               slowSub.dragK);
        app.endFrame();
    }
    return 0;
}
