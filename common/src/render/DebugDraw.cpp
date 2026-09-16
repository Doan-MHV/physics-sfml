#include "render/DebugDraw.hpp"
#include <filesystem>
#include <vector>

namespace render {

void DebugDraw::drawArrow(sf::RenderTarget& target, 
                          physics::Vec2 start, 
                          physics::Vec2 end, 
                          sf::Color color, 
                          float thickness, 
                          float headSize) {
    physics::Vec2 d = end - start;
    float len = d.length();
    if (len < 0.001f) return;

    physics::Vec2 u = d / len;
    physics::Vec2 n = u.perpendicular();
    float h = std::min(headSize, len * 0.5f);
    physics::Vec2 b = end - u * h;

    physics::Vec2 p1 = end;
    physics::Vec2 p2 = b + n * (h * 0.45f);
    physics::Vec2 p3 = b - n * (h * 0.45f);

    physics::Vec2 w = n * (thickness * 0.5f);
    physics::Vec2 s1 = start + w;
    physics::Vec2 s2 = start - w;
    physics::Vec2 s3 = b + w;
    physics::Vec2 s4 = b - w;

    sf::VertexArray va(sf::PrimitiveType::Triangles, 9);
    // Arrow head
    va[0] = sf::Vertex{p1.toSfml(), color};
    va[1] = sf::Vertex{p2.toSfml(), color};
    va[2] = sf::Vertex{p3.toSfml(), color};

    // Shaft quad (2 triangles)
    va[3] = sf::Vertex{s1.toSfml(), color};
    va[4] = sf::Vertex{s2.toSfml(), color};
    va[5] = sf::Vertex{s3.toSfml(), color};

    va[6] = sf::Vertex{s3.toSfml(), color};
    va[7] = sf::Vertex{s2.toSfml(), color};
    va[8] = sf::Vertex{s4.toSfml(), color};

    target.draw(va);
}

void DebugDraw::drawLine(sf::RenderTarget& target, 
                         physics::Vec2 start, 
                         physics::Vec2 end, 
                         sf::Color color, 
                         float thickness) {
    if (thickness <= 1.0f) {
        sf::Vertex line[2] = {
            sf::Vertex{start.toSfml(), color},
            sf::Vertex{end.toSfml(), color}
        };
        target.draw(line, 2, sf::PrimitiveType::Lines);
        return;
    }

    physics::Vec2 d = end - start;
    float len = d.length();
    if (len < 0.001f) return;

    physics::Vec2 u = d / len;
    physics::Vec2 n = u.perpendicular();
    physics::Vec2 w = n * (thickness * 0.5f);

    sf::VertexArray va(sf::PrimitiveType::Triangles, 6);
    va[0] = sf::Vertex{(start + w).toSfml(), color};
    va[1] = sf::Vertex{(start - w).toSfml(), color};
    va[2] = sf::Vertex{(end + w).toSfml(), color};

    va[3] = sf::Vertex{(end + w).toSfml(), color};
    va[4] = sf::Vertex{(start - w).toSfml(), color};
    va[5] = sf::Vertex{(end - w).toSfml(), color};

    target.draw(va);
}

void DebugDraw::drawCircle(sf::RenderTarget& target, 
                           physics::Vec2 center, 
                           float radius, 
                           sf::Color fillColor, 
                           sf::Color outlineColor, 
                           float outlineThickness) {
    sf::CircleShape circle(radius);
    circle.setOrigin({radius, radius});
    circle.setPosition(center.toSfml());
    circle.setFillColor(fillColor);
    circle.setOutlineColor(outlineColor);
    circle.setOutlineThickness(outlineThickness);
    target.draw(circle);
}

void DebugDraw::drawGrid(sf::RenderTarget& target, 
                         sf::Vector2u windowSize, 
                         float spacing, 
                         physics::Vec2 origin,
                         sf::Color gridColor, 
                         sf::Color axisColor) {
    // Vertical grid lines
    for (float x = std::fmod(origin.x, spacing); x < static_cast<float>(windowSize.x); x += spacing) {
        drawLine(target, {x, 0.0f}, {x, static_cast<float>(windowSize.y)}, gridColor, 1.0f);
    }

    // Horizontal grid lines
    for (float y = std::fmod(origin.y, spacing); y < static_cast<float>(windowSize.y); y += spacing) {
        drawLine(target, {0.0f, y}, {static_cast<float>(windowSize.x), y}, gridColor, 1.0f);
    }

    // Primary Axes through origin
    if (origin.y >= 0.0f && origin.y <= static_cast<float>(windowSize.y)) {
        drawLine(target, {0.0f, origin.y}, {static_cast<float>(windowSize.x), origin.y}, axisColor, 1.5f);
    }
    if (origin.x >= 0.0f && origin.x <= static_cast<float>(windowSize.x)) {
        drawLine(target, {origin.x, 0.0f}, {origin.x, static_cast<float>(windowSize.y)}, axisColor, 1.5f);
    }
}

std::optional<sf::Font> DebugDraw::loadDefaultFont() {
    static const std::vector<std::string> candidates = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/SFNS.ttf",
        "/Library/Fonts/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf"
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            try {
                sf::Font font(path);
                return font;
            } catch (...) {
                // Continue to next candidate
            }
        }
    }
    return std::nullopt;
}

} // namespace render
