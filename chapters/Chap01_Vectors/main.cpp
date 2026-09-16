#include <SFML/Graphics.hpp>
#include "math/Vec2.hpp"
#include "render/DebugDraw.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Chapter 1: Vectors & Operations Playground");
    window.setFramerateLimit(60);

    const physics::Vec2 origin{640.0f, 380.0f};

    // Vector U (Blue) and Vector V (Amber) stored in local Cartesian coords relative to origin
    // Note: in math physics, +Y is UP, but on screen +Y is DOWN.
    // We store screen offsets so dragging is intuitive, and flip Y when displaying math values!
    physics::Vec2 uOffset{220.0f, -140.0f}; // Points up-right
    physics::Vec2 vOffset{160.0f, 100.0f};  // Points down-right

    enum class DragTarget { None, U, V };
    DragTarget dragging = DragTarget::None;

    auto fontOpt = render::DebugDraw::loadDefaultFont();
    std::optional<sf::Text> hudText;
    if (fontOpt) {
        hudText.emplace(*fontOpt, "", 16);
        hudText->setFillColor(sf::Color(220, 225, 235));
        hudText->setPosition({25.0f, 25.0f});
    }

    std::cout << "====================================================\n";
    std::cout << " CHAPTER 1: VECTORS & OPERATIONS PLAYGROUND\n";
    std::cout << " - Left Click & Drag the blue or amber circles to change vectors.\n";
    std::cout << " - Observe U + V (Green), U - V (Purple), and Dot/Cross products.\n";
    std::cout << " - Press Escape to exit.\n";
    std::cout << "====================================================\n";

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                window.close();
            }

            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    physics::Vec2 mousePos{static_cast<float>(mouseBtn->position.x),
                                           static_cast<float>(mouseBtn->position.y)};
                    physics::Vec2 tipU = origin + uOffset;
                    physics::Vec2 tipV = origin + vOffset;

                    if (mousePos.distanceSquared(tipU) <= 20.0f * 20.0f) {
                        dragging = DragTarget::U;
                    } else if (mousePos.distanceSquared(tipV) <= 20.0f * 20.0f) {
                        dragging = DragTarget::V;
                    }
                }
            }

            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    dragging = DragTarget::None;
                }
            }

            if (const auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                physics::Vec2 mousePos{static_cast<float>(mouseMove->position.x),
                                       static_cast<float>(mouseMove->position.y)};
                if (dragging == DragTarget::U) {
                    uOffset = mousePos - origin;
                } else if (dragging == DragTarget::V) {
                    vOffset = mousePos - origin;
                }
            }
        }

        // Compute physics math (Math Cartesian: y = -screenY)
        physics::Vec2 mathU{uOffset.x, -uOffset.y};
        physics::Vec2 mathV{vOffset.x, -vOffset.y};

        float lenU = mathU.length();
        float lenV = mathV.length();
        float dotProduct = mathU.dot(mathV);
        float crossProduct = mathU.cross(mathV);

        float angleDeg = 0.0f;
        if (lenU > 0.001f && lenV > 0.001f) {
            float cosTheta = std::clamp(dotProduct / (lenU * lenV), -1.0f, 1.0f);
            angleDeg = std::acos(cosTheta) * (180.0f / 3.1415926535f);
        }

        // Render
        window.clear(sf::Color(20, 22, 28));

        // 1. Grid with origin
        render::DebugDraw::drawGrid(window, window.getSize(), 40.0f, origin);

        // 2. Vector Subtraction (U - V): arrow from tip of V to tip of U
        physics::Vec2 tipU = origin + uOffset;
        physics::Vec2 tipV = origin + vOffset;
        render::DebugDraw::drawArrow(window, tipV, tipU, sf::Color(192, 132, 252), 2.0f, 8.0f);

        // 3. Parallelogram helper lines for Addition
        render::DebugDraw::drawLine(window, tipU, tipU + vOffset, sf::Color(245, 158, 11, 100), 1.5f);
        render::DebugDraw::drawLine(window, tipV, tipV + uOffset, sf::Color(59, 130, 246, 100), 1.5f);

        // 4. Vector Addition Resultant (U + V)
        render::DebugDraw::drawArrow(window, origin, origin + uOffset + vOffset, sf::Color(16, 185, 129), 3.5f, 12.0f);

        // 5. Vectors U and V
        render::DebugDraw::drawArrow(window, origin, tipU, sf::Color(59, 130, 246), 3.0f, 10.0f);
        render::DebugDraw::drawArrow(window, origin, tipV, sf::Color(245, 158, 11), 3.0f, 10.0f);

        // Drag handle circles at tips
        render::DebugDraw::drawCircle(window, tipU, 7.0f, sf::Color(59, 130, 246), sf::Color::White, 2.0f);
        render::DebugDraw::drawCircle(window, tipV, 7.0f, sf::Color(245, 158, 11), sf::Color::White, 2.0f);

        // Origin center marker
        render::DebugDraw::drawCircle(window, origin, 4.0f, sf::Color::White);

        // 6. On-screen HUD
        if (hudText) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1);
            ss << "[ CHAPTER 1: VECTOR MATH LIVE HUD ]\n";
            ss << "Vector U (Blue)   : (" << mathU.x << ", " << mathU.y << ")  | Length: " << lenU << "\n";
            ss << "Vector V (Amber)  : (" << mathV.x << ", " << mathV.y << ")  | Length: " << lenV << "\n";
            ss << "Vector U + V (Green) : (" << (mathU + mathV).x << ", " << (mathU + mathV).y << ")\n";
            ss << "Vector U - V (Purple): (" << (mathU - mathV).x << ", " << (mathU - mathV).y << ")\n";
            ss << "----------------------------------------------\n";
            ss << "Dot Product (U . V) : " << dotProduct << "  -->  ";
            if (dotProduct > 20.0f) ss << "Facing Same Way (Acute < 90 deg)\n";
            else if (dotProduct < -20.0f) ss << "Facing Away (Obtuse > 90 deg)\n";
            else ss << "Orthogonal / Perpendicular (90 deg)\n";

            ss << "2D Cross (U x V)    : " << crossProduct << "  -->  ";
            if (crossProduct > 20.0f) ss << "V is Counter-Clockwise from U (+)\n";
            else if (crossProduct < -20.0f) ss << "V is Clockwise from U (-)\n";
            else ss << "Collinear / Parallel (0)\n";

            ss << "Angle Between       : " << angleDeg << " deg\n";
            ss << "----------------------------------------------\n";
            ss << "Tip: Click and drag the Blue or Amber circle handles!";

            hudText->setString(ss.str());
            window.draw(*hudText);
        }

        window.display();
    }

    return 0;
}
