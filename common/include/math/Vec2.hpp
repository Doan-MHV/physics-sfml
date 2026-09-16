

#ifndef GAMEPHYSICS_VEC2_HPP
#define GAMEPHYSICS_VEC2_HPP

struct Vec2 {
    float x{0.0f}, y{0.0f};

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& rhs) const {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator-(const Vec2& rhs) const {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
};

#endif  // GAMEPHYSICS_VEC2_HPP
