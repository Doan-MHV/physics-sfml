#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "math/Vec2.hpp"

void drawArrow(sf::RenderWindow& window, Vec2 start, Vec2 end, sf::Color color) {
    sf::Vertex line[] = {sf::Vertex(sf::Vector2f(start.x, start.y), color),
                         sf::Vertex(sf::Vector2f(end.x, end.y), color)};
    window.draw(line, 2, sf::PrimitiveType::Lines);

    sf::CircleShape tip(6.0f);
    tip.setOrigin({6.0f, 6.0f});
    tip.setPosition({end.x, end.y});
    tip.setFillColor(color);
    window.draw(tip);
}

void drawLabel(sf::RenderWindow& window, const sf::Font& font, const std::string& textStr, Vec2 pos, sf::Color color) {
    sf::Text label(font, textStr, 14);
    label.setFillColor(color);
    label.setPosition({pos.x + 8.0f, pos.y - 18.0f});
    window.draw(label);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Chap1 - Vector");
    window.setFramerateLimit(60);

    sf::Font font;
    // Cross-platform: load font directly from project assets
    if (!font.openFromFile("assets/fonts/Arial.ttf")) {
        // Fallback if running directly from inside cmake-build-debug subfolder:
        font.openFromFile("../../assets/fonts/Arial.ttf");
    }

    Vec2 turretPos(640.0f, 360.0f);
    Vec2 turretForward(0.0f, -1.0f);

    Vec2 turretNormal = turretForward.perpendicular();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        Vec2 targetPos(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        Vec2 toTarget = (targetPos - turretPos).normalized();

        float crossProduct = turretForward.cross(toTarget);
        float dotProduct = turretForward.dot(toTarget);

        sf::Color steeringColor;
        std::string steeringText;

        if (crossProduct > 0.05f) {
            steeringColor = sf::Color(70, 220, 120);  // Green
            steeringText = "STEER RIGHT / CLOCKWISE (+)";
        } else if (crossProduct < -0.05f) {
            steeringColor = sf::Color(245, 170, 60);  // Amber
            steeringText = "STEER LEFT / COUNTER-CLOCKWISE (-)";
        } else {
            steeringColor = sf::Color(80, 200, 255);  // Cyan
            steeringText = "LOCKED ON! (CROSS PRODUCT = 0)";
        }

        window.clear(sf::Color(24, 26, 32));

        // 1. Draw Forward Aim Line (White: Length 140px)
        drawArrow(window, turretPos, turretPos + (turretForward * 140.0f), sf::Color::White);
        // 2. Draw Perpendicular Normal Line (Magenta: 90 deg)
        drawArrow(window, turretPos, turretPos + (turretNormal * 80.0f), sf::Color(230, 80, 230));
        // 3. Draw Line to Mouse Target (Colored by Steering decision!)
        drawArrow(window, turretPos, targetPos, steeringColor);

        sf::CircleShape turretBase(15.0f);
        turretBase.setOrigin({15.0f, 15.0f});
        turretBase.setPosition({turretPos.x, turretPos.y});
        turretBase.setFillColor(sf::Color(70, 80, 100));
        turretBase.setOutlineColor(sf::Color::White);
        turretBase.setOutlineThickness(2.0f);
        window.draw(turretBase);

        drawLabel(window, font, "Forward", {turretPos.x + 10.0f, turretPos.y - 150.0f}, sf::Color::White);
        drawLabel(window, font, "Normal (-y, x)",
                  {turretPos.x + (turretNormal.x * 80.0f) + 10.0f, turretPos.y + (turretNormal.y * 80.0f)},
                  sf::Color(230, 80, 230));
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "=== 2D CROSS PRODUCT (STEERING DECISION) ===\n"
           << "Cross Product : " << crossProduct << "\n"
           << "Dot Product   : " << dotProduct << "\n"
           << "Steering AI   : " << steeringText;
        drawLabel(window, font, ss.str(), {40.0f, 40.0f}, steeringColor);

        window.display();
    }

    return 0;
}