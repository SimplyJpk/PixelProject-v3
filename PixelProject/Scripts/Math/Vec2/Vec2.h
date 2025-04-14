#pragma once

#include "Macros/InlineReturn.h"
#include "Macros/DefaultMemberFunc.h"

struct Vec2
{
	float x;
	float y;

	explicit constexpr Vec2(const float x = 0, const float y = 0) noexcept: x(x), y(y)
	{
	}
	constexpr Vec2(const int x, const int y) noexcept: x(static_cast<float>(x)), y(static_cast<float>(y))
	{
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(x, y);
	}

	// Default 'special' member functions
	DEFAULT_COPY_CONSTRUCTOR(Vec2)
	DEFAULT_COPY_ASSIGNMENT(Vec2)
	DEFAULT_MOVE_CONSTRUCTOR(Vec2)
	DEFAULT_MOVE_ASSIGNMENT(Vec2)
	DEFAULT_DESTRUCTOR(Vec2)

#pragma region STATIC_CONSTANTS

	STATIC_CONST_INLINE_NOEXCEPT(Vec2, Zero, Vec2(0.0f, 0.0f))
	STATIC_CONST_INLINE_NOEXCEPT(Vec2, One, Vec2(1.0f, 1.0f))
	STATIC_CONST_INLINE_NOEXCEPT(Vec2, Left, Vec2(-1.0f, 0.0f))
	STATIC_CONST_INLINE_NOEXCEPT(Vec2, Right, Vec2(1.0f, 0.0f))
	STATIC_CONST_INLINE_NOEXCEPT(Vec2, Up, Vec2(0.0f, 1.0f))
	STATIC_CONST_INLINE_NOEXCEPT(Vec2, Down, Vec2(0.0f, -1.0f))

#pragma endregion STATIC_CONSTANTS

#pragma region OPPERATOR_OVERLOADS

	Vec2 operator+(const Vec2& rhs) const;

#pragma endregion OPPERATOR_OVERLOADS
};
