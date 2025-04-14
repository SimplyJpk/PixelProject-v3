#include "SVec2.h"

SVec2 SVec2::operator+(const SVec2& rhs) const
{
	return SVec2{ static_cast<short>(x + rhs.x), static_cast<short>(y + rhs.y) };
}