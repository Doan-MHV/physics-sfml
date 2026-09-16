#include <SFML/Graphics.hpp>
#include "math/Vec2.hpp"
#include <iostream>

int main() {
    // 1. Math verification from Chapter 1
    physics::Vec2 v1{3.0f, 4.0f};
    physics::Vec2 v2{1.0f, 2.0f};

    std::cout << "=== Chapter 1 Vector Math Verification ===\n";
    std::cout << "v1: " << v1 << " | Length: " << v1.length() << " (Expected: 5)\n";
    std::cout << "v1 + v2: " << (v1 + v2) << "\n";
    std::cout << "v1 . v2 (Dot product): " << v1.dot(v2) << "\n";
    std::cout << "v1 x v2 (2D Cross product): " << v1.cross(v2) << "\n";
    std::cout << "Normalized v1: " << v1.normalized() << " | Length: " << v1.normalized().length() << "\n";
    std::cout << "==========================================\n\n";

    // 2. Initialize SFML 3 Window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Game Physics Engine - Milestone 1 (Vec2 & Kinematics)");
    window.setFramerateLimit(60);

    // Ball particle representation
    physics::Vec2 position{640.0f, 360.0f};
    physics::Vec2 velocity{120.0f, -80.0f}; // Pixels per second

    sf::CircleShape ball(20.0f);
    ball.setFillColor(sf::Color(70, 150, 240));
    ball.setOrigin({20.0f, 20.0f});

    sf::Clock clock;

    std::cout << "[Physics Engine] Window running. Press Escape or Close to exit.\n";

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                window.close();
            }
        }

        // Basic Euler update (Chapter 2 Kinematics: r = r + v * dt)
        position += velocity * dt;

        // Bounce off window boundaries
        if (position.x - 20.0f < 0.0f || position.x + 20.0f > 1280.0f) {
            velocity.x = -velocity.x;
        }
        if (position.y - 20.0f < 0.0f || position.y + 20.0f > 720.0f) {
            velocity.y = -velocity.y;
        }

        ball.setPosition(position.toSfml());

        window.clear(sf::Color(22, 24, 30));
        window.draw(ball);
        window.display();
    }

    return 0;
}