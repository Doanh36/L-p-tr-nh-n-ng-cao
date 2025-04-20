#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>

class Vector2D {
public:
    float X, Y;

    Vector2D(float x = 0, float y = 0) : X(x), Y(y) {}

    // addition +
    inline Vector2D operator+(const Vector2D& v2) const {
        return Vector2D(X + v2.X, Y + v2.Y);
    }

    // +=
    inline Vector2D& operator+=(const Vector2D& v2) {
        X += v2.X;
        Y += v2.Y;
        return *this;
    }

    // subtraction -
    inline Vector2D operator-(const Vector2D& v2) const {
        return Vector2D(X - v2.X, Y - v2.Y);
    }

    // multiplication *
    inline Vector2D operator*(float scalar) const {
        return Vector2D(X * scalar, Y * scalar);
    }

    // length (norm)
    float Length() const {
        return std::sqrt(X * X + Y * Y);
    }

    // normalized vector
    Vector2D Normalized() const {
        float len = Length();
        return (len != 0) ? Vector2D(X / len, Y / len) : Vector2D(0, 0);
    }

    void Log(std::string msg = "") const {
        std::cout << msg << " (X, Y) = (" << X << ", " << Y << ")\n";
    }
};

#endif // VECTOR 2D_H
