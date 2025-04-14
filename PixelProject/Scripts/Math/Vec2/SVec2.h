#pragma once

#include "Macros/InlineReturn.h"
#include "Macros/DefaultMemberFunc.h"

struct SVec2
{
	short x;
	short y;

	explicit constexpr SVec2(const short x = 0, const short y = 0) noexcept: x(x), y(y)
	{
	}
	constexpr SVec2(const int x, const int y) noexcept: x(static_cast<short>(x)), y(static_cast<short>(y))
	{
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(x, y);
	}

	// Default 'special' member functions
	DEFAULT_COPY_CONSTRUCTOR(SVec2)
	DEFAULT_COPY_ASSIGNMENT(SVec2)
	DEFAULT_MOVE_CONSTRUCTOR(SVec2)
	DEFAULT_MOVE_ASSIGNMENT(SVec2)
	DEFAULT_DESTRUCTOR(SVec2)

#pragma region STATIC_CONSTANTS

	STATIC_CONST_INLINE_NOEXCEPT(SVec2, Zero, SVec2(0, 0))
	STATIC_CONST_INLINE_NOEXCEPT(SVec2, One, SVec2(1, 1))
	STATIC_CONST_INLINE_NOEXCEPT(SVec2, Left, SVec2(-1, 0))
	STATIC_CONST_INLINE_NOEXCEPT(SVec2, Right, SVec2(1, 0))
	STATIC_CONST_INLINE_NOEXCEPT(SVec2, Up, SVec2(0, 1))
	STATIC_CONST_INLINE_NOEXCEPT(SVec2, Down, SVec2(0, -1))

#pragma endregion STATIC_CONSTANTS

#pragma region OPPERATOR_OVERLOADS

	SVec2 operator+(const SVec2& rhs) const;

#pragma endregion OPPERATOR_OVERLOADS

};