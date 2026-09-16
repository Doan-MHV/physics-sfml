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

    Vec2 baseU(180.0f, -120.0f);
    Vec2 v(140.0f, 80.0f);
    float scaleFactor = 1.0f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Keyboard controls to test SCALAR MULTIPLICATION (*)
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Up) {
                    scaleFactor += 0.1f;  // Stretch
                } else if (key->code == sf::Keyboard::Key::Down) {
                    scaleFactor -= 0.1f;  // Shrink
                }
            }
        }

        Vec2 u = baseU * scaleFactor;

        Vec2 tipU = origin + u;
        Vec2 tipV = origin + v;

        Vec2 tipAdd = origin + (u + v);

        window.clear(sf::Color(24, 26, 32));

        drawArrow(window, origin, tipU, sf::Color::Red);
        drawArrow(window, origin, tipV, sf::Color::Cyan);
        drawArrow(window, origin, tipAdd, sf::Color::Green);
        drawArrow(window, tipV, tipU, sf::Color::Yellow);

        drawLabel(window, font,
                  "Start Point\n(" + std::to_string((int)origin.x) + ", " + std::to_string((int)origin.y) + ")", origin,
                  sf::Color(180, 180, 180));
        // 2. Vector U moved the point here:
        std::string labelU = "Vector U: (" + std::to_string((int)u.x) + ", " + std::to_string((int)u.y) + ")\n" +
                             "-> New Point: (" + std::to_string((int)tipU.x) + ", " + std::to_string((int)tipU.y) + ")";
        drawLabel(window, font, labelU, tipU, sf::Color::Red);
        // 3. Vector V moved the point here:
        std::string labelV = "Vector V: (" + std::to_string((int)v.x) + ", " + std::to_string((int)v.y) + ")\n" +
                             "-> New Point: (" + std::to_string((int)tipV.x) + ", " + std::to_string((int)tipV.y) + ")";
        drawLabel(window, font, labelV, tipV, sf::Color::Cyan);
        // 4. Combined Vector (U + V) moved the point here:
        std::string labelAdd = "Vector (U + V): (" + std::to_string((int)(u + v).x) + ", " +
                               std::to_string((int)(u + v).y) + ")\n" + "-> Final Point: (" +
                               std::to_string((int)tipAdd.x) + ", " + std::to_string((int)tipAdd.y) + ")";
        drawLabel(window, font, labelAdd, tipAdd, sf::Color::Green);

        window.display();
    }

    return 0;
}