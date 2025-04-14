#include "Vec2.h"

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2{ x + rhs.x, y + rhs.y };
}