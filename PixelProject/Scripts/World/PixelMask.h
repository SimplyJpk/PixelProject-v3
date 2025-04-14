#pragma once
#include <SDL3/SDL_stdinc.h>

namespace PixelMask
{
	struct Index
	{
		inline constexpr static Uint32 BITS = 0b0000'0000'0000'0000'0000'0000'0001'1111;
		inline constexpr static Uint8 COUNT = 5;
		inline constexpr static Uint8 DEPTH = 0 + 1;

		constexpr static Uint32 GetMask()
		{
			return BITS << DEPTH;
		}
		constexpr static Uint32 GetMaxValue()
		{
			return (1 << COUNT) - 1;
		}
		constexpr static uint8_t GetValue(const Uint32 value)
		{
			const Uint32 maskedValue = (value & BITS) << 1;
			return static_cast<Uint8>((maskedValue & GetMask()) >> DEPTH);
		}
	};

	struct Lifetime
	{
		inline constexpr static Uint32 BITS = 0b1111'1100'0000'0000'0000'0000'0000'0000;
		inline constexpr static Uint8 COUNT = 6;
		inline constexpr static Uint8 DEPTH = 26 + 1;

		constexpr static Uint32 GetMask()
		{
			return BITS << DEPTH;
		}
		constexpr static Uint32 GetMaxValue()
		{
			return (1 << COUNT) - 1;
		}
		constexpr static uint8_t GetValue(const Uint32 value)
		{
			return (value & GetMask()) >> DEPTH;
		}
	};

	struct Behaviour
	{
		inline constexpr static Uint32 BITS = 0b0000'0011'1100'0000'0000'0000'0000'0000;
		inline constexpr static uint8_t COUNT = 4;
		inline constexpr static uint8_t DEPTH = 22 + 1;

		constexpr static Uint32 GetMask()
		{
			return BITS << DEPTH;
		}
		constexpr static Uint32 GetMaxValue()
		{
			return (1 << COUNT) - 1;
		}
		constexpr static uint8_t GetValue(const Uint32 value)
		{
			return (value & GetMask()) >> DEPTH;
		}
	};

	struct Light
	{
		inline constexpr static Uint32 BITS = 0b0000'0000'0000'0000'0000'0000'1110'0000;
		inline constexpr static uint8_t COUNT = 3;
		inline constexpr static uint8_t DEPTH = 5 + 1;

		constexpr static Uint32 GetMask()
		{
			return BITS << DEPTH;
		}
		constexpr static Uint32 GetMaxValue()
		{
			return (1 << COUNT) - 1;
		}
		constexpr static uint8_t GetValue(const Uint32 value)
		{
			return (value & GetMask()) >> DEPTH;
		}
	};

	// TODO : (James) Unused, but here as a reminder for future use
	inline constexpr static Uint32 REMAINING_BITS = 0b0000'0000'0011'1111'1111'1111'0000'0000;
}