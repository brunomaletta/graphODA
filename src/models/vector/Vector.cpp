#include "Vector.hpp"

Vector::Vector() {}

Vector::Vector(float x_, float y_) {
  x = x_;
  y = y_;
}

Vector::Vector(pair<float, float> v) { x = v.first, y = v.second; }

Vector operator+(const Vector &u, const Vector &v) {
  return Vector(u.x + v.x, u.y + v.y);
}

Vector operator-(const Vector &u, const Vector &v) {
  return Vector(u.x - v.x, u.y - v.y);
}

Vector operator*(const Vector &u, const float c) {
  return Vector(u.x * c, u.y * c);
}

float Vector::norm() { return sqrt(x * x + y * y); }

float Vector::angle() { return atan2(y, x); }

Vector Vector::rotate(float th) {
  return Vector(x * cos(th) - y * sin(th), x * sin(th) + y * cos(th));
}

// projecao sobre o vetor v
Vector Vector::project(Vector v) { return v * dot(*this, v) * (1 / dot(v, v)); }

// norma do produvo vetorial
float cross(Vector u, Vector v) { return u.x * v.y - u.y * v.x; }

float dist(Vector u, Vector v) { return (u - v).norm(); }

// produto escalar
float dot(Vector u, Vector v) { return u.x * v.x + u.y * v.y; }
