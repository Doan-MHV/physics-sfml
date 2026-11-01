#include "render/GLDraw.hpp"

// ============================================================================
// Section 1.1 Challenge: Newton's 2nd & 3rd Laws + Static Wall (invMass = 0)
// ============================================================================

struct Body {
    float x{0.0f};
    float vx{0.0f};
    float invMass{0.0f};
};

// TODO 1: Return 0.0f if massKg <= 0.0f (immovable static wall), else 1.0f / massKg
float calcInvMass(float massKg) {
    (void)massKg;
    return 0.0f; // <-- Replace this!
}

// TODO 2: Step body using Newton's 2nd Law (a = forceX * invMass), then update vx and x
void stepBody(Body& b, float forceX, float dt) {
    (void)b; (void)forceX; (void)dt;
    // <-- Write 3 lines here: compute ax, update b.vx, update b.x
}

int main(int /*argc*/, char* /*argv*/[]) {
    gl_draw::SimpleWindow app("Section 1.1 Challenge: Hold SPACE to push pods apart (R to reset)");

    Body leftPod{-1.5f, 0.0f, calcInvMass(2.0f)};   // Light Pod (2 kg)
    Body rightPod{1.5f, 0.0f, calcInvMass(6.0f)};   // Heavy Pod (6 kg)
    Body staticWall{8.0f, 0.0f, calcInvMass(0.0f)}; // Static Wall (0 kg -> infinite mass)

    constexpr float kDt = 1.0f / 60.0f;

    while (app.pollEvents()) {
        if (app.isKeyHeld(SDL_SCANCODE_R)) {
            leftPod.x = -1.5f; leftPod.vx = 0.0f;
            rightPod.x = 1.5f; rightPod.vx = 0.0f;
        }

        // Action Force on rightPod when SPACE is held (+12 N)
        float forceOnRight = app.isKeyHeld(SDL_SCANCODE_SPACE) ? 12.0f : 0.0f;

        // TODO 3: By Newton's 3rd Law (Action-Reaction), what is forceOnLeft?
        float forceOnLeft = 0.0f; // <-- Replace with equal & opposite reaction force!

        stepBody(leftPod,    forceOnLeft,  kDt);
        stepBody(rightPod,   forceOnRight, kDt);
        stepBody(staticWall, 500.0f,       kDt); // 500 N pushes wall, but invMass=0 keeps it still!

        app.beginFrame();
        gl_draw::drawCircle(leftPod.x,    0.0f, 0.5f, gl_draw::kCyan, true);
        gl_draw::drawCircle(rightPod.x,   0.0f, 0.8f, gl_draw::kOrange, true);
        gl_draw::drawCircle(staticWall.x, 0.0f, 1.0f, gl_draw::kWhite, false);

        gl_draw::drawTextWorld(leftPod.x - 1.2f,  1.0f, gl_draw::kCyan,   "2kg v=%.1fm/s", leftPod.vx);
        gl_draw::drawTextWorld(rightPod.x - 1.2f, -1.2f, gl_draw::kOrange, "6kg v=%.1fm/s", rightPod.vx);
        gl_draw::drawTextWorld(staticWall.x - 1.0f, 1.3f, gl_draw::kWhite, "Wall(0kg)");
        gl_draw::drawTextWorld(-12.2f, 6.2f, gl_draw::kYellow,
            "Hold SPACE: F_right=%.0fN, F_left=%.0fN | invMass(2kg)=%.2f, invMass(6kg)=%.2f",
            forceOnRight, forceOnLeft, leftPod.invMass, rightPod.invMass);
        app.endFrame();
    }
    return 0;
}
