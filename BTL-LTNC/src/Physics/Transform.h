#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector2D.h"
#include <iostream>
#include <string>

class Transform {
public:
    Vector2D position;   // Tâm của object
    Vector2D velocity;   // Hướng di chuyển * speed
    float speed;

    Transform(float x = 0.0f, float y = 0.0f, float s = 0.0f)
        : position(x, y), speed(s), velocity(0, 0) {}

    void Log(std::string msg = "") const {
        std::cout << msg << " Position: (" << position.X << ", " << position.Y << ")"
                  << " | Velocity: (" << velocity.X << ", " << velocity.Y << ")"
                  << " | Speed: " << speed << std::endl;
    }

    void Translate(const Vector2D& v) {
        position += v;
    }

    void TranslateX(float x) {
        position.X += x;
    }

    void TranslateY(float y) {
        position.Y += y;
    }
};

#endif // TRANSFORM_H
