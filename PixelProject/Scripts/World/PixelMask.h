#pragma once
#include <SDL3/SDL_stdinc.h>
#include "Utility/XorShift.h"

namespace PixelMask
{
	// TODO: (James) Do we want to add a "ZeroPoint" to the Masks? We could use this to have 'fake' signed values, but I'm unsure if this helps us (Offset, we can have -2 to +13 instead of -8 to +7) or if it just adds complexity.
	template <Uint64 BitMask, Uint8 Count, Uint8 Depth, typename ValueType = Uint8>
	struct Mask
	{
		static constexpr Uint64 BITS = BitMask;

		static constexpr Uint8 COUNT = Count;
		static constexpr Uint8 DEPTH = Depth;

		// To try increase support by not needing 64bit values in shader, we pass in low/high bits separately due to uint64 limitations
		static constexpr Uint32 GPU_SHADER_BITS = (BITS > 0xFFFFFFFF) ? ((BITS >> 32) & 0xFFFFFFFF) : (BITS & 0xFFFFFFFF);
		static constexpr Uint32 GPU_SHADER_BITS_DEPTH = (BITS > 0xFFFFFFFF) ? (DEPTH - 32) : DEPTH;
		static constexpr ValueType MAX_VALUE = (1ULL << COUNT) - 1;

#pragma region Getter Methods

		static constexpr Uint64 GetBitMask() { return BITS; } // TODO: (James) Maybe remove? We can just use BITS directly, maybe rename BITS to MASK or something?
		/**
		 * @brief Get the Max Value object for the related mask bits. ie; if COUNT is 3, the max value is 7 regardless of where those bits are located.
		 * ie; 0b0000'0111'0000 and 0b0000'0000'0111 would both return 7 as the max value.
		 *
		 * @return constexpr ValueType The max value of the mask bits.
		 */
		static constexpr ValueType GetMaxValue() { return MAX_VALUE; }

		static constexpr ValueType GetValue(Uint64 pixelData)
		{
			return static_cast<ValueType>((pixelData & BITS) >> DEPTH);
		}
		static constexpr bool IsValueZero(Uint64 pixelData)
		{
			return GetValue(pixelData) == 0;
		}

#pragma endregion

#pragma region Setter Methods

		static constexpr Uint64 SetValue(Uint64 pixelData, ValueType new_value)
		{
			return (pixelData & ~BITS) | ((static_cast<Uint64>(new_value) & GetMaxValue()) << DEPTH);
		}

#pragma endregion

#pragma region Decrement Methods

		/**
		 * @brief Decrements the value relevant to the masks bits by 1 if it is greater than 0, otherwise returns the value unchanged.
		 * This method uses the [[likely]] attribute to hint the compiler that the value is greater than 0.
		 * Consider the use of DecrementValueFast for more "performance" at the cost of underflow checks.
		 *
		 * @param pixelData The pixel data to decrement the value from.
		 * @return constexpr Uint64 The decremented value.
		 */
		static constexpr Uint64 DecrementValue(Uint64 pixelData)
		{
			ValueType current = GetValue(pixelData);
			if (current > 0) [[likely]]
			{
				return SetValue(pixelData, current - 1);
			}
			return pixelData;
		}

		/**
		 * @brief Decrements the value relevant to the masks bits by 1 without checking if it is greater than 0.
		 * This method is "faster" than DecrementValue but may cause underflow if the value is already 0.
		 *
		 * @param pixelData The pixel data to decrement the value from.
		 * @return constexpr Uint64 The decremented value.
		 */
		static constexpr Uint64 DecrementValueFast(Uint64 pixelData)
		{
			ValueType current = GetValue(pixelData);
			return SetValue(pixelData, current - 1);
		}

		/**
		 * @brief Decrements the value relevant to the masks bits by 1 if it is greater than 0, otherwise returns the value unchanged.
		 * This method is branchless and thus may be faster in some cases, but depends on additional instructions so may be slower unless it is likely to be 0.
		 *
		 * @param pixelData The pixel data to decrement the value from.
		 * @return constexpr Uint64 The decremented value.
		 */
		static constexpr Uint64 DecrementValueBranchless(Uint64 pixelData)
		{
			ValueType current = GetValue(pixelData);
			// Decrement if > 0, otherwise keep current value
			ValueType new_value = current - (current > 0);
			return SetValue(pixelData, new_value);
		}

#pragma endregion

#pragma region Increment Methods

		/**
		 * @brief Increments the value relevant to the masks bits by 1 if it is less than the maximum value, otherwise returns the value unchanged.
		 * This method uses the [[likely]] attribute to hint the compiler that the value is less than the maximum value.
		 * Consider the use of IncrementValueFast for more "performance" at the cost of overflow checks.
		 *
		 * @param pixelData The pixel data to increment the value from.
		 * @return constexpr Uint64 The incremented value.
		 */
		static constexpr Uint64 IncrementValue(Uint64 pixelData)
		{
			ValueType current = GetValue(pixelData);
			if (current < MAX_VALUE) [[likely]]
			{
				return SetValue(pixelData, current + 1);
			}
			return pixelData;
		}

		/**
		 * @brief Increments the value relevant to the masks bits by 1 without checking if it is less than the maximum value.
		 * This method is "faster" than IncrementValue but may cause overflow if the value is already the maximum value.
		 *
		 * @param pixelData The pixel data to increment the value from.
		 * @return constexpr Uint64 The incremented value.
		 */
		static constexpr Uint64 IncrementValueFast(Uint64 pixelData)
		{
			ValueType current = GetValue(pixelData);
			return SetValue(pixelData, current + 1);
		}

		/**
		 * @brief Increments the value relevant to the masks bits by 1 if it is less than the maximum value, otherwise returns the value unchanged.
		 * This method is branchless and thus may be faster in some cases, but depends on additional instructions so may be slower unless it is likely to be less than the maximum value.
		 *
		 * @param pixelData The pixel data to increment the value from.
		 * @return constexpr Uint64 The incremented value.
		 */
		static constexpr Uint64 IncrementValueBranchless(Uint64 pixelData)
		{
			ValueType current = GetValue(pixelData);
			// Increment if < MAX_VALUE, otherwise keep current value
			ValueType new_value = current + (current < MAX_VALUE);
			return SetValue(pixelData, new_value);
		}

#pragma endregion

#pragma region Utility Methods

		static constexpr ValueType AddValue(Uint64 pixelData, ValueType value)
		{
			ValueType current = GetValue(pixelData);
			ValueType new_value = std::min(current + value, MAX_VALUE);
			return SetValue(pixelData, new_value);
		}

		static constexpr bool IsMaxValue(Uint64 pixelData)
		{
			return GetValue(pixelData) == MAX_VALUE;
		}

#pragma endregion

#pragma region Random Value Methods

		/**
		 * @brief Gets a random value within the valid range for this mask (0 to MAX_VALUE)
		 *
		 * @return constexpr ValueType A random value between 0 and MAX_VALUE inclusive
		 */
		static constexpr ValueType GetRandomValidValue(XorShift &_rng) noexcept
		{
			return static_cast<ValueType>(_rng() % (MAX_VALUE + 1));
		}

		/**
		 * @brief Gets random bits positioned according to this mask's bit pattern
		 *
		 * @return constexpr Uint64 Random bits in the mask's position
		 */
		static constexpr Uint64 GetRandomValidBits(XorShift &_rng) noexcept
		{
			return (static_cast<Uint64>(_rng() % (MAX_VALUE + 1)) << DEPTH) & BITS;
		}

		/**
		 * @brief Sets a random value to the mask bits within the given pixel data
		 *
		 * @param pixelData The pixel data to modify
		 * @return constexpr Uint64 The modified pixel data with random bits set
		 */
		static constexpr Uint64 SetRandomValue(Uint64 pixelData, XorShift &_rng) noexcept
		{
			return SetValue(pixelData, GetRandomValidValue(_rng));
		}

		/**
		 * @brief Modifies the pixel data by setting random bits according to this mask's pattern
		 *
		 * @param pixelData The pixel data to modify
		 * @return constexpr Uint64 The modified pixel data
		 */
		static constexpr Uint64 SetRandomBits(Uint64 pixelData, XorShift &_rng) noexcept
		{
			// Clear the bits for this mask and set random ones
			return (pixelData & ~BITS) | (GetRandomValidBits(_rng) & BITS);
		}

#pragma endregion

#pragma region Assertions

		static_assert(std::is_integral_v<ValueType>, "ValueType must be an integral type");
		// Assert MAX_VALUE <= std::numeric_limits<ValueType>::max() and print out seen and max value
		static_assert(MAX_VALUE <= std::numeric_limits<ValueType>::max(), "ValueType cannot hold the maximum value of the mask");
		// Check the mask matches a generated mask of the same count and depth
		static_assert(BITS == ((1ULL << Count) - 1) << Depth, "BitMask does not match Count and Depth");

#pragma endregion
	};

#pragma region Pixel Masks

	// Aliases for the Masks <- Most Significant Bit (MSB) is on the left, Least Significant Bit (LSB) is on the right ->

	// Component, used to determine when a pixel is destroyed. 																		(Bits: 6 [U], values 0 to +63)
	using Lifetime = Mask<0b1111'1100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 6, 58, Uint8>;
	// Component, can be unique between pixels and used to determine how a pixel interacts with other pixels or the world. 			(Bits: 4 [U], values 0 to +15)
	using Behaviour = Mask<0b0000'0011'1100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 4, 54, Uint8>;
	// Component, used to determine buoyancy of a pixel. 																			(Bits: 4 [U], values 0 to +15)
	using Density = Mask<0b0000'0000'0011'1100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 4, 50, Uint8>;
	// Component, whether or not a pixel is on fire. 																				(Bits: 1 [U], values 0 to +1)
	using Burning = Mask<0b0000'0000'0000'0010'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 1, 49, Uint8>;
	// Component, how flammable a pixel is. 																						(Bits: 3 [U], values 0 to +7)
	using Flammability = Mask<0b0000'0000'0000'0001'1100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 3, 46, Uint8>;
	// Component, used to determine the temperature of a pixel. 																	(Bits: 4 [U], values 0 to +15)
	using Temperature = Mask<0b0000'0000'0000'0000'0011'1100'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 4, 42, Uint8>;
	// Component, how many pixels are in this pixel. 																				(Bits: 3 [U], values 0 to +7)
	using CloneCount = Mask<0b0000'0000'0000'0000'0000'0011'1000'0000'0000'0000'0000'0000'0000'0000'0000'0000, 3, 39, Uint8>;

	// New Masks somewhere in here, we can't have overlapping masks, use caution when adding new masks. However, the compiler will moan at you if you mess up the bitmask.
	using WHATS_LEFT = Mask<0b0000'0000'0000'0000'0000'0000'0111'1111'1111'1111'1110'0000'0000'0000'0000'0000, 18, 20, Uint8>;

	// Component, normalized velocity of a pixel, ie; number of pixel steps it needs to make per tick. 								(Bits: 3 [U], values 0 to +7)
	using Velocity = Mask<0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0001'1100'0000'0000'0000'0000, 3, 17, Uint8>;
	// Component, used to determine the X velocity of a pixel. (Maybe we want 4 bits?)												(Bits: 3 [U], values 0 to +7)
	using XVelocity = Mask<0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0011'1000'0000'0000'0000, 3, 14, Uint8>;
	// Component, used to determine the Y velocity of a pixel. (Maybe we want 4 bits?)												(Bits: 3 [U], values 0 to +7)
	using YVelocity = Mask<0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0111'0000'0000'0000, 3, 11, Uint8>;
	// Component, used to determine the light level of a pixel. 																	(Bits: 4 [U], values 0 to +15)
	using Light = Mask<0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000, 4, 8, Uint8>;
	// Component, sub-index for the pixel colours. 																					(Bits: 3 [U], values 0 to +7)
	using SubIndex = Mask<0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1110'0000, 3, 5, Uint8>;
	// Component, used to determine the index of a pixel.																			(Bits: 5 [U], values 0 to +31)
	using Index = Mask<0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0001'1111, 5, 0, Uint8>;

#pragma endregion

	template <typename... Masks>
	static constexpr Uint64 BatchUpdate(Uint64 pixelData, auto... values)
	{
		// Scream if values/masks don't match
		static_assert(sizeof...(Masks) == sizeof...(values), "Number of masks must match number of values");

		// Combine masks
		constexpr Uint64 combinedMask = (... | Masks::BITS);
		return (pixelData & ~combinedMask) |
			   (... | (((static_cast<Uint64>(values)) << Masks::DEPTH) & Masks::BITS));
	}
}