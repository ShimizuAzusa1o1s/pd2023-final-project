#include "vector_2d.h"

Vector2D::Vector2D() {
    this->x = 0;
    this->y = 0;
}

Vector2D::Vector2D(float x, float y) {
    this->x = x;
    this->y = y;
}

// Scale the vector to length 1. A zero vector stays zero.
void Vector2D::unit_vector() {
    double x_square = pow(this->x, 2);
    double y_square = pow(this->y, 2);
    double length = sqrt(x_square + y_square);
    if (length == 0) return;
    this->x /= length;
    this->y /= length;
    return;
}

Vector2D &Vector2D::add(const Vector2D &vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2D &Vector2D::subtract(const Vector2D &vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D &Vector2D::multiply(const Vector2D &vec) {
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

Vector2D &Vector2D::divide(const Vector2D &vec) {
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2) {
    return v1.add(v2);
}

Vector2D &operator-(Vector2D &v1, const Vector2D &v2) {
    return v1.subtract(v2);
}

Vector2D &operator*(Vector2D &v1, const Vector2D &v2) {
    return v1.multiply(v2);
}

Vector2D &operator/(Vector2D &v1, const Vector2D &v2) {
    return v1.divide(v2);
}

Vector2D &Vector2D::operator+=(const Vector2D &vec) {
    return this->add(vec);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec) {
    return this->subtract(vec);
}

Vector2D &Vector2D::operator*=(const Vector2D &vec) {
    return this->multiply(vec);
}

Vector2D &Vector2D::operator/=(const Vector2D &vec) {
    return this->divide(vec);
}

Vector2D &Vector2D::operator*(const int &i) {
    this->x *= i;
    this->y *= i;
    return *this;
}

Vector2D &Vector2D::zero() {
    this->x = 0;
    this->y = 0;
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const Vector2D &vec) {
    stream << "(" << vec.x << "," << vec.y << ")";
    return stream;
}
