#include "Vector.h"

#include <cmath>

float Vector2f::Dot(const Vector2f& a, const Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

float Vector2f::Cross(const Vector2f& a, const Vector2f& b)
{
    return a.x * b.y - a.y * b.x;
}

bool Vector2f::Normalize()
{
    const float length = std::hypot(x, y);

    if (length == 0.f || std::isfinite(length) == false)
        return false;

    x /= length;
    y /= length;

    return true;
}

Vector2f Vector2f::Normalized() const
{
    Vector2f result = *this;
    result.Normalize();
    return result;
}

Vector2f Vector2f::operator+(const Vector2f& other) const
{
    return { x + other.x, y + other.y };
}

Vector2f Vector2f::operator-(const Vector2f& other) const
{
    return { x - other.x, y - other.y };
}

Vector2f Vector2f::operator*(float scalar) const
{
    return { x * scalar, y * scalar };
}

Vector2f Vector2f::operator/(float scalar) const
{
    return { x / scalar, y / scalar };
}

Vector2f operator*(float scalar, const Vector2f& vector)
{
    return vector * scalar;
}

Vector2f& Vector2f::operator+=(const Vector2f& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

bool Vector2f::operator==(const Vector2f& other) const
{
    return x == other.x && y == other.y;
}
