#pragma once

#include "Input/InputContainer.h"
#include "Macros/DefaultMemberFunc.h"
#include "Macros/InlineReturn.h"

struct IVec2
{
	int x;
	int y;

	explicit constexpr IVec2(const int x = 0, const int y = 0) noexcept: x(x), y(y)
	{
	}
	constexpr IVec2(const short x, const short y) noexcept: x(static_cast<int>(x)), y(static_cast<int>(y))
	{
	}
	constexpr IVec2(const float x, const float y) noexcept: x(static_cast<int>(x)), y(static_cast<int>(y))
	{
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(x, y);
	}

	// Default 'special' member functions
	DEFAULT_COPY_CONSTRUCTOR(IVec2)
	DEFAULT_COPY_ASSIGNMENT(IVec2)
	DEFAULT_MOVE_CONSTRUCTOR(IVec2)
	DEFAULT_MOVE_ASSIGNMENT(IVec2)
	DEFAULT_DESTRUCTOR(IVec2)

	#pragma region STATIC_CONSTANTS

	STATIC_CONST_INLINE_NOEXCEPT(IVec2, Zero, IVec2(0, 0))
	STATIC_CONST_INLINE_NOEXCEPT(IVec2, One, IVec2(1, 1))
	STATIC_CONST_INLINE_NOEXCEPT(IVec2, Left, IVec2(-1, 0))
	STATIC_CONST_INLINE_NOEXCEPT(IVec2, Right, IVec2(1, 0))
	STATIC_CONST_INLINE_NOEXCEPT(IVec2, Up, IVec2(0, 1))
	STATIC_CONST_INLINE_NOEXCEPT(IVec2, Down, IVec2(0, -1))

#pragma endregion STATIC_CONSTANTS

#pragma region OPPERATOR_OVERLOADS

	IVec2 operator+(const IVec2& rhs) const;
	IVec2 operator-(const IVec2& rhs) const;

	IVec2& operator+=(const IVec2& vec2);
	IVec2& operator-=(const IVec2& vec2);
	bool operator==(const IVec2& vec2) const;

	// Scalar Operations
	IVec2 operator*(int scalar) const;
	IVec2 operator/(int scalar) const;

#pragma endregion OPPERATOR_OVERLOADS

#pragma region FUNCTIONS

	IVec2 GetNormalized() const;
	void Normalize();

	float GetMagnitude() const;
	float GetMagnitudeSquared() const;

#pragma endregion FUNCTIONS
};

namespace std
{
	template<>
	struct hash<IVec2>
	{
		size_t operator()(const IVec2& vec2) const
		{
			return hash<int>()(vec2.x) ^ hash<int>()(vec2.y);
		}
	};
}
