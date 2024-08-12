#include "Vector2D.h"

// Constructors
Vector2D::Vector2D() : x(0), y(0) {}
Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

// Getters and Setters
float Vector2D::getX() const {
    return x;
}

void Vector2D::setX(float x) {
    this->x = x;
}

float Vector2D::getY() const {
    return y;
}

void Vector2D::setY(float y) {
    this->y = y;
}

// Vector operations
float Vector2D::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalized() const {
    float mag = magnitude();
    return Vector2D(x / mag, y / mag);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(x / scalar, y / scalar);
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
