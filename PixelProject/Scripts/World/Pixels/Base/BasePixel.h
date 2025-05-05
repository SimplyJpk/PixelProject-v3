#pragma once

#include <SDL3/SDL_stdinc.h>

#include <vector>
#include <array>
#include <random>

#include "PixelUpdateResult.h"
#include "Utility/XorShift.h"
#include "World/WorldConstants.h"
#include "World/PixelMask.h"

class BasePixel
{
protected:
	// No-Op function
	void NoOpUpdate(PixelUpdateResult &, Uint64 &, const Uint64 &) noexcept {}

public:
	using UpdateFunction = void (BasePixel::*)(PixelUpdateResult &, Uint64 &, const Uint64 &) noexcept;

	// Constructor, taking in the pixel type to help with initialisation
	BasePixel(Pixel::PixelType _pixel_type = Pixel::PixelType::UNDEF) noexcept;

public:
	Pixel::PixelType pixel_type = Pixel::PixelType::UNDEF;
	std::array<char, Pixel::MAX_NAME_LENGTH> pixel_name = {"None"};

	bool is_updateable = false;

	// Pixel Settings
	uint8_t pixel_index = 0;
	uint8_t colour_count = 0;

	Uint32 type_colours[Pixel::MAX_COLOUR_COUNT] = {0};

	// TODO : (James) Can this be wrapped into something more flexible?
	uint8_t new_pixel_count = 0;
	Uint64 new_pixel_value[Pixel::MAX_NEW_VALUE_COUNT] = {0};
	Uint64 GetNewPixel() noexcept; // TODO: (James) We could optimize this a fair bit if we were willing to convert BasePixel to a template, potentially use of function pointer to derived class might be faster than what we have without templates, but.. maybe not?

	const std::array<uint8_t, 8> &GetPixelUpdateOrder() noexcept;
	Uint32 GetRandomColour() noexcept;
	// Method that converts the hex colour to 4f, and returns inside a passed in array
	static void GetColourAs4F(Uint32 colour, float *out_colour) noexcept;
	void GetIndexAs4FColour(short index, float *out_colour) const noexcept;

	// Pixel Updating
	UpdateFunction update_function = &BasePixel::NoOpUpdate;
	void UpdatePixel(PixelUpdateResult &result, Uint64 &pixel_value, const Uint64 &neighbour_value) noexcept
	{
		(this->*update_function)(result, pixel_value, neighbour_value);
	}

protected:
	// Protected methods, snack-sized updates for specific Masks called from derived UpdatePixel methods
	// Update Lifetime (Decay)
	constexpr bool UpdateLifetime(PixelUpdateResult &data, Uint64 &pixel_value, const Pixel::PixelType decay_into) noexcept
	{
		pixel_value = PixelMask::Lifetime::DecrementValue(pixel_value);

		if (PixelMask::Lifetime::IsValueZero(pixel_value)) [[unlikely]]
		{
			data.SetLocal(decay_into);
			return false;
		}
		return true;
	}

	constexpr bool UpdateDensity(PixelUpdateResult &data, Uint64 &pixel_value, const Uint64 &neighbour_value, XorShift &_rng) noexcept
	{
		const auto density = PixelMask::Density::GetValue(pixel_value);
		if (density == 0)
		{
			return false;
		}
		const auto neighbour_density = PixelMask::Density::GetValue(neighbour_value);
		if (neighbour_density == 0)
		{
			return false;
		}

		const int8_t densityDifference = density - neighbour_density;
		// If densities are the same, we skip
		if (densityDifference == 0)
		{
			return false;
		}

		// Get the vertical direction the pixel is trying to move.
		const int8_t pixelDirection = Chunk::GetYDirFromWorldDir(data.Dir());

		// Determine the expected vertical direction based on the density difference.
		int8_t expectedDirection = 1;
		if (densityDifference < 0)
		{
			// Less dense, so trend upwards
			expectedDirection = -1;
		}

		if (pixelDirection == expectedDirection)
		{
			// An attempt at branchless rolling
			const auto diff = std::abs(densityDifference);

			const int fullChance = (diff > 5);
			const int chancePercent = 18 * diff;
			const int roll = _rng() % 100;

			const int partialChance = (roll < chancePercent);

			// Basically, if the pixel is denser than the neighbor, it has a chance to move, 18% per density diff
			return (fullChance | partialChance) != 0;
		}
		return false;
	}

protected:
	XorShift _rng;

	uint8_t _chunk_order_counter = 0;
	uint8_t _pixel_update_order_count = 0;
	std::array<std::array<uint8_t, Chunk::NUM_DIRECTIONS>, Pixel::MAX_UPDATE_ORDER_COUNT> _pixel_update_order;

	void SetPixelUpdateOrder(size_t index, std::initializer_list<Chunk::WorldDir> dirs) noexcept;
	void SetPixelName(const std::string_view name) noexcept;
};
