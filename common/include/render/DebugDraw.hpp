#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

#include "math/Vec2.hpp"

namespace render {
class DebugDraw {
public:
  static void drawArrow(sf::RenderTarget& target, physics::Vec2 start, physics::Vec2 end, sf::Color color,
                        float thickness = 2.5f, float headSize = 10.0f);

  static void drawLine(sf::RenderTarget& target, physics::Vec2 start, physics::Vec2 end, sf::Color color,
                       float thickness = 1.0f);

  static void drawCircle(sf::RenderTarget& target, physics::Vec2 center, float radius, sf::Color fillColor,
                         sf::Color outlineColor = sf::Color::Transparent, float outlineThickness = 0.0f);

  static void drawGrid(sf::RenderTarget& target, sf::Vector2u windowSize, float spacing = 40.0f,
                       physics::Vec2 origin = {0.0f, 0.0f}, sf::Color gridColor = sf::Color(35, 38, 48),
                       sf::Color axisColor = sf::Color(70, 76, 92));

  static std::optional<sf::Font> loadDefaultFont();
};
}  // namespace render
