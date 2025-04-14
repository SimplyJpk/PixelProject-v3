#include "IVec2.h"

IVec2 IVec2::operator+(const IVec2& rhs) const
{
	return IVec2{ x + rhs.x, y + rhs.y };
}

IVec2 IVec2::operator-(const IVec2& rhs) const
{
	return IVec2{ x - rhs.x, y - rhs.y };
}

IVec2& IVec2::operator+=(const IVec2& vec2)
{
	x += vec2.x;
	y += vec2.y;
	return *this;
}

IVec2& IVec2::operator-=(const IVec2& vec2)
{
	x -= vec2.x;
	y -= vec2.y;
	return *this;
}

bool IVec2::operator==(const IVec2& vec2) const
{
	return x == vec2.x && y == vec2.y;
}

IVec2 IVec2::operator*(const int scalar) const
{
	return IVec2{ x * scalar, y * scalar };
}

IVec2 IVec2::operator/(const int scalar) const
{
	return IVec2{ x / scalar, y / scalar };
}

IVec2 IVec2::GetNormalized() const
{
	const float length = GetMagnitude();
	return IVec2{ static_cast<int>(x / length), static_cast<int>(y / length) };
}

void IVec2::Normalize()
{
	const float length = GetMagnitude();
	x = static_cast<int>(x / length);
	y = static_cast<int>(y / length);
}

float IVec2::GetMagnitude() const
{
	return static_cast<float>(sqrt(x * x + y * y));
}

float IVec2::GetMagnitudeSquared() const
{
	return static_cast<float>(x * x + y * y);
}
