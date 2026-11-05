#include "render/GLDraw.hpp"

// ============================================================================
// ASSIGNMENT: Chapter 1, Section 1.1 — Newton's Three Laws & Inverse Mass
// ============================================================================
//
// SCENARIO OVERVIEW:
//   Two space pods sit in zero-gravity space next to an immovable concrete wall:
//     1. Left Blue Pod  (x = -1.5 m, mass = 2.0 kg)
//     2. Right Orange Pod (x = +1.5 m, mass = 6.0 kg)
//     3. White Static Wall (x = +8.0 m, mass = 0.0 kg -> Infinite Mass!)
//
//   When you hold [SPACE], an explosive thruster between the two pods pushes
//   the Right Orange Pod to the right with +12.0 N of force.
//
// YOUR TASKS:
//   - Task 1 (calcInvMass): Convert mass (kg) into Inverse Mass (1 / mass).
//     If massKg <= 0.0f, return 0.0f so static objects never move.
//   - Task 2 (stepBody): Implement Newton's 2nd Law (ax = forceX * invMass)
//     and step velocity (vx) and position (x) forward by dt seconds.
//   - Task 3 (Action-Reaction): Apply Newton's 3rd Law in main() so the Left
//     Blue Pod receives the equal and opposite reaction force of the Right Pod.
//
// EXPECTED END RESULT (How to verify your solution works):
//   1. At startup, the top yellow HUD must show:
//        invMass(2kg) = 0.50,  invMass(6kg) = 0.17
//   2. Hold [SPACE] for 1 full second (60 frames), then release [SPACE]:
//        - The Right Orange Pod (6 kg) accelerates at +2.0 m/s^2 and glides
//          to the right at +2.0 m/s.
//        - The Left Blue Pod (2 kg) receives -12 N reaction force, accelerates
//          3x faster (-6.0 m/s^2), and glides to the left at -6.0 m/s.
//        - When you release [SPACE], both pods keep gliding forever at those
//          exact speeds (Newton's 1st Law: Inertia).
//        - The White Static Wall (0 kg) stays frozen at x = 8.0 m (v = 0 m/s)
//          even though 500 N of force pushes on it every frame!
// ============================================================================

struct Body {
    float x{0.0f};
    float vx{0.0f};
    float invMass{0.0f};
};

// TODO 1: Return 0.0f if massKg <= 0.0f (immovable static wall), else 1.0f / massKg
float calcInvMass(float massKg) {
    if (massKg <= 0.0f)
        return 0.0f;
    else
        return 1.0f / massKg;
}

// TODO 2: Step body using Newton's 2nd Law (a = forceX * invMass), then update vx and x
void stepBody(Body& b, float forceX, float dt) {
    // <-- Write 3 lines here: compute ax, update b.vx, update b.x
    float ax = forceX * b.invMass;
    b.vx += ax * dt;
    b.x += b.vx * dt;
}

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Section 1.1 Challenge: Hold SPACE to push pods apart (R to reset)");

    Body leftPod{-1.5f, 0.0f, calcInvMass(2.0f)};    // Light Pod (2 kg)
    Body rightPod{1.5f, 0.0f, calcInvMass(6.0f)};    // Heavy Pod (6 kg)
    Body staticWall{8.0f, 0.0f, calcInvMass(0.0f)};  // Static Wall (0 kg -> infinite mass)

    constexpr float kDt = 1.0f / 60.0f;

    while (app.pollEvents()) {
        if (app.isKeyHeld(SDL_SCANCODE_R)) {
            leftPod.x = -1.5f;
            leftPod.vx = 0.0f;
            rightPod.x = 1.5f;
            rightPod.vx = 0.0f;
        }

        // Action Force on rightPod when SPACE is held (+12 N)
        float forceOnRight = app.isKeyHeld(SDL_SCANCODE_SPACE) ? 12.0f : 0.0f;

        // TODO 3: By Newton's 3rd Law (Action-Reaction), what is forceOnLeft?
        float forceOnLeft = -forceOnRight;  // <-- Replace with equal & opposite reaction force!

        stepBody(leftPod, forceOnLeft, kDt);
        stepBody(rightPod, forceOnRight, kDt);
        stepBody(staticWall, 500.0f, kDt);  // 500 N pushes wall, but invMass=0 keeps it still!

        app.beginFrame();
        gl_draw::drawCircle(leftPod.x, 0.0f, 0.5f, gl_draw::kCyan, true);
        gl_draw::drawCircle(rightPod.x, 0.0f, 0.8f, gl_draw::kOrange, true);
        gl_draw::drawCircle(staticWall.x, 0.0f, 1.0f, gl_draw::kWhite, false);

        gl_draw::drawTextWorld(leftPod.x - 1.2f, 1.0f, gl_draw::kCyan, "2kg v=%.1fm/s", leftPod.vx);
        gl_draw::drawTextWorld(rightPod.x - 1.2f, -1.2f, gl_draw::kOrange, "6kg v=%.1fm/s", rightPod.vx);
        gl_draw::drawTextWorld(staticWall.x - 1.0f, 1.3f, gl_draw::kWhite, "Wall(0kg)");
        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kYellow,
                               "Hold SPACE: F_right=%.0fN, F_left=%.0fN | invMass(2kg)=%.2f, invMass(6kg)=%.2f",
                               forceOnRight, forceOnLeft, leftPod.invMass, rightPod.invMass);
        app.endFrame();
    }
    return 0;
}
