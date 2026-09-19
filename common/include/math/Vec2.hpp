

#ifndef GAMEPHYSICS_VEC2_HPP
#define GAMEPHYSICS_VEC2_HPP

inline constexpr float EPSILON = 0.0001f;

struct Vec2 {
    float x{0.0f}, y{0.0f};

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    Vec2 operator-(const Vec2& rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Vec2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    Vec2 operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    [[nodiscard]] float lengthSquared() const {
        return x * x + y * y;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vec2 normalized() {
        float len = length();
        if (len > EPSILON) {
            return {x / len, y / len};
        }

        return Vec2{0.0f, 0.0f};
    }

    [[nodiscard]] float dot(const Vec2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }
};

#endif  // GAMEPHYSICS_VEC2_HPP
