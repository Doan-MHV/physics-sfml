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

    Vec2 guardPos(640.0f, 360.0f);

    Vec2 guardForward(1.0f, 0.0f);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        Vec2 playerPos(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        Vec2 toPlayer = (playerPos - guardPos).normalized();

        float dotProduct = guardForward.dot(toPlayer);

        float cosTheta = std::clamp(dotProduct, -1.0f, 1.0f);
        float angleDeg = std::acos(cosTheta) * 180.0f / 3.14159265358979323846f;

        sf::Color arrowColor;
        std::string statusText;

        if (dotProduct > 0.05f) {
            arrowColor = sf::Color(50, 220, 120);
            statusText = "IN SIGHT!";
        } else if (dotProduct < -0.05f) {
            arrowColor = sf::Color(240, 70, 70);
            statusText = "OUT OF SIGHT!";
        } else {
            arrowColor = sf::Color(70, 200, 255);
            statusText = "EXACTLY PERPENDICULAR!";
        }

        window.clear(sf::Color(24, 26, 32));

        sf::Vertex boundaryLine[] = {sf::Vertex{sf::Vector2f(guardPos.x, 60.0f), sf::Color(60, 65, 80)},
                                     sf::Vertex{sf::Vector2f(guardPos.x, 660.0f), sf::Color(60, 65, 80)}};
        window.draw(boundaryLine, 2, sf::PrimitiveType::Lines);

        drawArrow(window, guardPos, guardPos + (guardForward * 120.0f), sf::Color::White);
        drawArrow(window, guardPos, playerPos, arrowColor);

        sf::CircleShape guardBody(14.0f);
        guardBody.setOrigin({14.0f, 14.0f});
        guardBody.setPosition({guardPos.x, guardPos.y});
        guardBody.setFillColor(sf::Color(100, 110, 130));
        guardBody.setOutlineColor(sf::Color::White);
        guardBody.setOutlineThickness(2.0f);
        window.draw(guardBody);

        drawLabel(window, font, "Guard", {guardPos.x - 20.0f, guardPos.y + 20.0f}, sf::Color::White);
        drawLabel(window, font, "Vision Boundary (90 deg)", {guardPos.x + 10.0f, 70.0f}, sf::Color(100, 110, 130));

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "Dot Product : " << dotProduct << "\n"
           << "Angle       : " << angleDeg << " deg\n"
           << "Status      : " << statusText;
        drawLabel(window, font, ss.str(), {40.0f, 40.0f}, arrowColor);

        window.display();
    }

    return 0;
}