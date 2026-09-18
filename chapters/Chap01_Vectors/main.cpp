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

    Vec2 origin(500.0f, 400.0f);

    const float unitCircleRadius = 80.0f;
    sf::CircleShape unitCircle(unitCircleRadius);
    unitCircle.setOrigin({unitCircleRadius, unitCircleRadius});
    unitCircle.setPosition({origin.x, origin.y});
    unitCircle.setFillColor(sf::Color::Transparent);
    unitCircle.setOutlineColor(sf::Color(70, 75, 90));
    unitCircle.setOutlineThickness(1.5f);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        Vec2 mousePos(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        Vec2 toMouse = mousePos - origin;
        float rawLength = toMouse.length();

        Vec2 direction = toMouse.normalized();
        float dirLength = direction.length();

        Vec2 unitArrowTip = origin + (direction * unitCircleRadius);

        window.clear(sf::Color(24, 26, 32));

        window.draw(unitCircle);

        drawArrow(window, origin, mousePos, sf::Color(140, 150, 170));
        drawArrow(window, origin, unitArrowTip, sf::Color(60, 220, 130));

        drawLabel(window, font, "Origin (Player)", origin, sf::Color(180, 180, 180));

        std::ostringstream ssMouse;
        ssMouse << std::fixed << std::setprecision(1);
        ssMouse << "Raw Vector (to Mouse)\n"
                << "Distance: " << rawLength << " px";
        drawLabel(window, font, ssMouse.str(), mousePos, sf::Color(140, 150, 170));
        std::ostringstream ssNorm;
        ssNorm << std::fixed << std::setprecision(2);
        ssNorm << "Normalized Unit Vector\n"
               << "Direction: (" << direction.x << ", " << direction.y << ")\n"
               << "Length: " << dirLength << " (Always 1.0!)";
        drawLabel(window, font, ssNorm.str(), {unitArrowTip.x + 10.0f, unitArrowTip.y + 10.0f},
                  sf::Color(60, 220, 130));

        window.display();
    }

    return 0;
}