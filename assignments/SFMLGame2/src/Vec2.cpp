#include <math.h>

#include "Vec2.h"

Vec2::Vec2()
	: Vec2(0, 0)
{
}

Vec2::Vec2(float x, float y)
	: x(x)
	, y(y)
{
}

bool Vec2::operator==(const Vec2 &rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2 &rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator/=(const float val)
{
	x /= val;
	y /= val;
}

float Vec2::dist(const Vec2 &rhs) const
{
	return sqrtf(powf(x - rhs.x, 2.0f) + powf(y - rhs.y, 2.0f));
}
