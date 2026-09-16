#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

namespace physics {

struct Vec2 {
    float x{0.0f};
    float y{0.0f};

    constexpr Vec2() = default;
    constexpr Vec2(float x_, float y_) : x(x_), y(y_) {}
    constexpr explicit Vec2(float scalar) : x(scalar), y(scalar) {}

    // Seamless conversion from SFML Vector2f
    constexpr Vec2(const sf::Vector2f& sfVec) : x(sfVec.x), y(sfVec.y) {}

    // Seamless conversion to SFML Vector2f for rendering
    [[nodiscard]] constexpr sf::Vector2f toSfml() const { return {x, y}; }

    constexpr explicit operator sf::Vector2f() const { return {x, y}; }

    // --- Basic Vector Arithmetic ---
    constexpr Vec2 operator+(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
    constexpr Vec2 operator-(const Vec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
    constexpr Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
    constexpr Vec2 operator/(float scalar) const {
        float inv = 1.0f / scalar;
        return {x * inv, y * inv};
    }
    constexpr Vec2 operator-() const { return {-x, -y}; }

    constexpr Vec2& operator+=(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    constexpr Vec2& operator-=(const Vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    constexpr Vec2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    constexpr Vec2& operator/=(float scalar) {
        float inv = 1.0f / scalar;
        x *= inv;
        y *= inv;
        return *this;
    }

    constexpr bool operator==(const Vec2& rhs) const = default;

    // --- Length & Normalization ---
    [[nodiscard]] constexpr float lengthSquared() const { return x * x + y * y; }

    [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }

    [[nodiscard]] Vec2 normalized() const {
        float lenSq = lengthSquared();
        if (lenSq > 1e-8f) {
            float invLen = 1.0f / std::sqrt(lenSq);
            return {x * invLen, y * invLen};
        }
        return {0.0f, 0.0f};
    }

    void normalize() { *this = normalized(); }

    // --- Dot Product ---
    // u . v = ux * vx + uy * vy = ||u|| ||v|| cos(theta)
    [[nodiscard]] constexpr float dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }

    // --- 2D Cross Product (Determinant / Wedge product) ---
    // u x v = ux * vy - uy * vx
    // Returns signed scalar area: positive if rhs is counter-clockwise from this
    [[nodiscard]] constexpr float cross(const Vec2& rhs) const { return x * rhs.y - y * rhs.x; }

    // Cross product of a 2D vector with a scalar z (angular velocity w x r)
    // w x r = (-w * r.y, w * r.x)
    [[nodiscard]] static constexpr Vec2 cross(float s, const Vec2& v) { return {-s * v.y, s * v.x}; }

    // r x w = (w * r.y, -w * r.x)
    [[nodiscard]] static constexpr Vec2 cross(const Vec2& v, float s) { return {s * v.y, -s * v.x}; }

    // Perpendicular vector: rotated 90 degrees counter-clockwise
    [[nodiscard]] constexpr Vec2 perpendicular() const { return {-y, x}; }

    // Distance between two points
    [[nodiscard]] constexpr float distanceSquared(const Vec2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy;
    }

    [[nodiscard]] float distance(const Vec2& other) const { return std::sqrt(distanceSquared(other)); }
};

// Scalar * Vec2 operator
constexpr inline Vec2 operator*(float scalar, const Vec2& vec) {
    return vec * scalar;
}

inline std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

}  // namespace physics
