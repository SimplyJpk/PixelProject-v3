#pragma once

namespace PixelProject::Utility
{
	constexpr inline void SetBit(unsigned int& value, const unsigned short bit) noexcept
	{
		value |= (1U << bit);
	}

	constexpr inline void ClearBit(unsigned int& value, const unsigned short bit) noexcept
	{
		value &= ~(1U << bit);
	}

	constexpr inline bool IsBitSet(const unsigned int value, const unsigned short bit) noexcept
	{
		return (value & (1U << bit)) != 0;
	}

	constexpr inline void ToggleBit(unsigned int& value, const unsigned short bit) noexcept
	{
		value ^= (1U << bit);
	}
}