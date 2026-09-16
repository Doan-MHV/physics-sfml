#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "math/Vec2.hpp"
#include "render/DebugDraw.hpp"

struct Projectile {
    physics::Vec2 pos;
    physics::Vec2 vel;
    physics::Vec2 accel{0.0f, 400.0f};  // Gravity down in screen coords (+Y)
    std::vector<physics::Vec2> trail;
    bool active{true};
    float peakY;
    float startX;
    float timeAlive{0.0f};
    float maxRange{0.0f};
    float maxHeight{0.0f};
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Chapter 2: Kinematics & Ballistics Playground");
    window.setFramerateLimit(60);

    const physics::Vec2 cannonBase{100.0f, 620.0f};
    const float groundY = 620.0f;

    float cannonAngleDeg = 45.0f;  // 45 degrees launch
    float launchSpeed = 380.0f;    // pixels per second
    bool enableAirDrag = false;
    bool paused = false;

    std::vector<Projectile> projectiles;

    auto fontOpt = render::DebugDraw::loadDefaultFont();
    std::optional<sf::Text> hudText;
    if (fontOpt) {
        hudText.emplace(*fontOpt, "", 16);
        hudText->setFillColor(sf::Color(220, 225, 235));
        hudText->setPosition({25.0f, 25.0f});
    }

    std::cout << "====================================================\n";
    std::cout << " CHAPTER 2: KINEMATICS & BALLISTICS PLAYGROUND\n";
    std::cout << " - SPACEBAR     : Fire cannon\n";
    std::cout << " - UP / DOWN    : Adjust launch angle\n";
    std::cout << " - LEFT / RIGHT : Adjust launch speed\n";
    std::cout << " - D            : Toggle air resistance (Drag)\n";
    std::cout << " - P            : Pause / Resume simulation\n";
    std::cout << " - C            : Clear all balls\n";
    std::cout << " - ESCAPE       : Exit\n";
    std::cout << "====================================================\n";

    auto fireProjectile = [&]() {
        float rad = cannonAngleDeg * (3.1415926535f / 180.0f);
        // Note: In screen coords, UP is -Y, so vy is negative!
        physics::Vec2 dir{std::cos(rad), -std::sin(rad)};
        physics::Vec2 initialPos = cannonBase + dir * 35.0f;
        physics::Vec2 initialVel = dir * launchSpeed;

        Projectile p;
        p.pos = initialPos;
        p.vel = initialVel;
        p.peakY = initialPos.y;
        p.startX = initialPos.x;
        p.trail.push_back(initialPos);
        projectiles.push_back(p);
    };

    // Fire initial shot so screen isn't empty
    fireProjectile();

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;  // Clamp delta time to avoid large jumps

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (key->code == sf::Keyboard::Key::Space) {
                    fireProjectile();
                } else if (key->code == sf::Keyboard::Key::D) {
                    enableAirDrag = !enableAirDrag;
                } else if (key->code == sf::Keyboard::Key::P) {
                    paused = !paused;
                } else if (key->code == sf::Keyboard::Key::C) {
                    projectiles.clear();
                } else if (key->code == sf::Keyboard::Key::Up) {
                    cannonAngleDeg = std::min(85.0f, cannonAngleDeg + 3.0f);
                } else if (key->code == sf::Keyboard::Key::Down) {
                    cannonAngleDeg = std::max(5.0f, cannonAngleDeg - 3.0f);
                } else if (key->code == sf::Keyboard::Key::Right) {
                    launchSpeed = std::min(700.0f, launchSpeed + 25.0f);
                } else if (key->code == sf::Keyboard::Key::Left) {
                    launchSpeed = std::max(100.0f, launchSpeed - 25.0f);
                }
            }

            // Mouse click to aim cannon directly at cursor
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    physics::Vec2 mousePos{static_cast<float>(mouseBtn->position.x),
                                           static_cast<float>(mouseBtn->position.y)};
                    physics::Vec2 diff = mousePos - cannonBase;
                    if (diff.x > 5.0f) {
                        float angle = -std::atan2(diff.y, diff.x) * (180.0f / 3.1415926535f);
                        cannonAngleDeg = std::clamp(angle, 5.0f, 85.0f);
                        fireProjectile();
                    }
                }
            }
        }

        // Physics Update (Semi-Implicit Euler)
        if (!paused) {
            for (auto& p : projectiles) {
                if (!p.active) continue;

                p.timeAlive += dt;

                // 1. Calculate Acceleration (Gravity + Air Drag)
                const float gravity = 450.0f;  // px/s^2 down
                p.accel = {0.0f, gravity};

                if (enableAirDrag) {
                    // F_drag = -0.5 * rho * v^2 * Cd * A
                    // In acceleration form: a_drag = -c * ||v|| * v
                    float speed = p.vel.length();
                    float dragCoeff = 0.0025f;
                    physics::Vec2 dragAccel = -dragCoeff * speed * p.vel;
                    p.accel += dragAccel;
                }

                // 2. Semi-Implicit Euler (Industry Standard):
                // v = v + a * dt
                p.vel += p.accel * dt;
                // x = x + v * dt
                p.pos += p.vel * dt;

                // Track trajectory & stats
                if (p.pos.y < p.peakY) {
                    p.peakY = p.pos.y;
                    p.maxHeight = groundY - p.peakY;
                }

                if (p.trail.empty() || p.trail.back().distanceSquared(p.pos) > 12.0f) {
                    p.trail.push_back(p.pos);
                }

                // Ground bounce / termination
                if (p.pos.y >= groundY - 6.0f) {
                    p.pos.y = groundY - 6.0f;
                    p.maxRange = p.pos.x - p.startX;

                    if (std::abs(p.vel.y) > 60.0f) {
                        p.vel.y = -p.vel.y * 0.55f;  // Coefficient of restitution e = 0.55
                        p.vel.x *= 0.75f;            // Ground friction
                    } else {
                        p.vel = {0.0f, 0.0f};
                        p.active = false;
                    }
                }

                // Out of screen bounds
                if (p.pos.x > 1350.0f || p.pos.x < -50.0f) {
                    p.active = false;
                }
            }
        }

        // Render
        window.clear(sf::Color(18, 20, 26));

        // 1. Background Grid
        render::DebugDraw::drawGrid(window, window.getSize(), 50.0f, {0.0f, groundY});

        // 2. Ground plane
        render::DebugDraw::drawLine(window, {0.0f, groundY}, {1280.0f, groundY}, sf::Color(120, 130, 150), 3.0f);

        // 3. Draw Projectile Trails & Balls
        for (const auto& p : projectiles) {
            // Draw trail
            for (size_t i = 1; i < p.trail.size(); ++i) {
                render::DebugDraw::drawLine(window, p.trail[i - 1], p.trail[i], sf::Color(56, 189, 248, 120), 2.0f);
            }

            // Draw Ball
            sf::Color ballColor = p.active ? sf::Color(56, 189, 248) : sf::Color(100, 116, 139);
            render::DebugDraw::drawCircle(window, p.pos, 7.0f, ballColor, sf::Color::White, 1.5f);

            // Draw Attached Live Vectors for active projectile
            if (p.active) {
                // Green: Velocity Vector v
                render::DebugDraw::drawArrow(window, p.pos, p.pos + p.vel * 0.15f, sf::Color(16, 185, 129), 2.5f, 8.0f);

                // Red: Acceleration Vector a
                render::DebugDraw::drawArrow(window, p.pos, p.pos + p.accel * 0.15f, sf::Color(244, 63, 94), 2.0f,
                                             7.0f);
            }
        }

        // 4. Draw Cannon
        float rad = cannonAngleDeg * (3.1415926535f / 180.0f);
        physics::Vec2 barrelDir{std::cos(rad), -std::sin(rad)};
        physics::Vec2 barrelEnd = cannonBase + barrelDir * 40.0f;
        render::DebugDraw::drawLine(window, cannonBase, barrelEnd, sf::Color(148, 163, 184), 8.0f);
        render::DebugDraw::drawCircle(window, cannonBase, 12.0f, sf::Color(71, 85, 105), sf::Color::White, 2.0f);

        // Draw aiming guide line (dashed aim vector)
        render::DebugDraw::drawArrow(window, barrelEnd, barrelEnd + barrelDir * (launchSpeed * 0.2f),
                                     sf::Color(250, 204, 21, 150), 1.5f, 6.0f);

        // 5. Live HUD
        if (hudText) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1);
            ss << "[ CHAPTER 2: KINEMATICS & BALLISTICS LIVE HUD ]\n";
            ss << "Launch Angle : " << cannonAngleDeg << " deg (UP / DOWN arrows or Left Click to aim)\n";
            ss << "Launch Speed : " << launchSpeed << " px/s (LEFT / RIGHT arrows)\n";
            ss << "Air Drag (D) : " << (enableAirDrag ? "ON (Quadratic Drag active)" : "OFF (Pure Galileo Parabola)")
               << "\n";
            ss << "Simulation   : " << (paused ? "PAUSED (P to resume)" : "RUNNING") << "\n";
            ss << "--------------------------------------------------------\n";

            if (!projectiles.empty()) {
                const auto& lastP = projectiles.back();
                ss << "Latest Shot  : Peak Height = " << lastP.maxHeight << " px | Range = " << lastP.maxRange
                   << " px\n";
                if (lastP.active) {
                    ss << "Live Velocity: (" << lastP.vel.x << ", " << -lastP.vel.y
                       << ") | Speed = " << lastP.vel.length() << " px/s\n";
                }
            }

            ss << "--------------------------------------------------------\n";
            ss << "Legend: [Green Arrow = Velocity (v)]  [Red Arrow = Acceleration / Gravity (a)]\n";
            ss << "Press SPACE to fire! Press C to clear.";

            hudText->setString(ss.str());
            window.draw(*hudText);
        }

        window.display();
    }

    return 0;
}
