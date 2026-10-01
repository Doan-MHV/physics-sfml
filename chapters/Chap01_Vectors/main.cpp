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
    if (!font.openFromFile("../../assets/fonts/Arial.ttf")) {
        // Fallback if running directly from inside cmake-build-debug subfolder:
        font.openFromFile("assets/fonts/Arial.ttf");
    }

    Vec2 playerPos(640.0f, 360.0f);
    float playerAngleRad = -3.1415926535f * 0.5f;

    const float radarRadius = 70.0f;
    sf::CircleShape radarRing(radarRadius);
    radarRing.setOrigin({radarRadius, radarRadius});
    radarRing.setPosition({playerPos.x, playerPos.y});
    radarRing.setFillColor(sf::Color::Transparent);
    radarRing.setOutlineColor(sf::Color(60, 65, 80));
    radarRing.setOutlineThickness(2.0f);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            playerAngleRad -= 0.05f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            playerAngleRad += 0.05f;
        }

        Vec2 forward(std::cos(playerAngleRad), std::sin(playerAngleRad));

        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        Vec2 enemyPos(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y));

        Vec2 toEnemy = enemyPos - playerPos;

        float distance = toEnemy.length();

        Vec2 enemyDir = toEnemy.normalized();

        float dot = forward.dot(enemyDir);

        float cross = forward.cross(enemyDir);

        Vec2 radarPipPos = playerPos + (enemyDir * radarRadius);

        sf::Color threatColor;
        std::string sightStatus;
        if (dot > 0.0f) {
            threatColor = sf::Color(70, 220, 120);  // Green (In Sight)
            sightStatus = "IN SIGHT (Front)";
        } else {
            threatColor = sf::Color(240, 70, 70);  // Red (Blind Spot)
            sightStatus = "BEHIND (Blind Spot!)";
        }
        std::string steerStatus;
        if (cross > 0.05f) {
            steerStatus = "TURN RIGHT ->";
        } else if (cross < -0.05f) {
            steerStatus = "<- TURN LEFT";
        } else {
            steerStatus = "LOCKED DEAD AHEAD!";
        }

        window.clear(sf::Color(22, 24, 30));

        window.draw(radarRing);

        drawArrow(window, playerPos, enemyPos, sf::Color(80, 90, 110));
        drawArrow(window, playerPos, playerPos + (forward * 100.0f), sf::Color::White);

        sf::CircleShape threatPip(8.0f);
        threatPip.setOrigin({8.0f, 8.0f});
        threatPip.setPosition({radarPipPos.x, radarPipPos.y});
        threatPip.setFillColor(threatColor);
        window.draw(threatPip);

        sf::CircleShape enemyDot(6.0f);
        enemyDot.setOrigin({6.0f, 6.0f});
        enemyDot.setPosition({enemyPos.x, enemyPos.y});
        enemyDot.setFillColor(threatColor);
        window.draw(enemyDot);

        drawLabel(window, font, "Player (Press A / D to Rotate)", {playerPos.x - 90.0f, playerPos.y + 85.0f},
                  sf::Color(160, 170, 190));
        drawLabel(window, font, "Threat", {enemyPos.x + 12.0f, enemyPos.y - 10.0f}, threatColor);
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "================ CHAPTER 1 FINALE: COMBAT MATH ================\n"
           << "Distance (Length)       : " << distance << " px\n"
           << "Direction (Normalized)  : (" << enemyDir.x << ", " << enemyDir.y << ")\n"
           << "Dot Product (Front/Back): " << dot << "  --> " << sightStatus << "\n"
           << "Cross Product (Steer)   : " << cross << "  --> " << steerStatus << "\n"
           << "===============================================================";
        drawLabel(window, font, ss.str(), {40.0f, 30.0f}, threatColor);

        window.display();
    }

    return 0;
}