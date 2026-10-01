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

    // 1. POSITION (pixels)
    Vec2 position(640.0f, 360.0f);
    // 2. VELOCITY (pixels per second)
    Vec2 velocity(260.0f, 160.0f);

    const float ballRadius = 18.0f;
    sf::CircleShape ball(ballRadius);
    ball.setOrigin({ballRadius, ballRadius});
    ball.setFillColor(sf::Color(70, 160, 245));
    ball.setOutlineColor(sf::Color::White);
    ball.setOutlineThickness(2.0f);

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Steer velocity with WASD / Arrow Keys
        float steerSpeed = 400.0f * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            velocity.x -= steerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            velocity.x += steerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            velocity.y -= steerSpeed;  // Up is -Y
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            velocity.y += steerSpeed;  // Down is +Y

        position = position + (velocity * dt);

        // Screen boundary bounce (reversing velocity component)
        if (position.x - ballRadius < 0.0f) {
            position.x = ballRadius;
            velocity.x = -velocity.x;
        } else if (position.x + ballRadius > 1280.0f) {
            position.x = 1280.0f - ballRadius;
            velocity.x = -velocity.x;
        }
        if (position.y - ballRadius < 0.0f) {
            position.y = ballRadius;
            velocity.y = -velocity.y;
        } else if (position.y + ballRadius > 720.0f) {
            position.y = 720.0f - ballRadius;
            velocity.y = -velocity.y;
        }
        ball.setPosition({position.x, position.y});

        // Render
        window.clear(sf::Color(22, 24, 30));
        // 1. Draw Ball
        window.draw(ball);
        // 2. Draw attached live Velocity Vector (Green arrow)
        drawArrow(window, position, position + (velocity * 0.35f), sf::Color(60, 220, 130));
        // 3. Live HUD
        float speed = velocity.length();
        int fps = static_cast<int>(1.0f / (dt > 0.0f ? dt : 0.016f));
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1);
        ss << "=== CHAPTER 2.1: VELOCITY & DELTA TIME ===\n"
           << "Position (r)    : (" << position.x << ", " << position.y << ") px\n"
           << "Velocity (v)    : (" << velocity.x << ", " << velocity.y << ") px/s\n"
           << "Speed (||v||)   : " << speed << " px/s\n"
           << "Frame Time (dt) : " << (dt * 1000.0f) << " ms (" << fps << " FPS)\n"
           << "-----------------------------------------\n"
           << "Controls: WASD / Arrow Keys to change velocity!";
        drawLabel(window, font, ss.str(), {30.0f, 30.0f}, sf::Color::White);

        window.display();
    }

    return 0;
}