#pragma once

#include <SDL3/SDL_stdinc.h>

#include <vector>
#include <array>
#include <random>

#include "PixelUpdateResult.h"
#include "Utility/XorShift.h"
#include "World/WorldConstants.h"

class BasePixel
{
 public:
	BasePixel(Pixel::PixelType _pixel_type = Pixel::PixelType::UNDEF);

 public:
	Pixel::PixelType pixel_type = Pixel::PixelType::UNDEF;
	std::array<char, Pixel::MAX_NAME_LENGTH> pixel_name = { "None" };

	bool is_updateable = false;

	// Random seeded with pixel_type
	std::minstd_rand rand_engine;
	std::uniform_int_distribution<int> distribution;

	// Pixel Settings
	uint8_t pixel_index = 0;
	uint8_t colour_count = 0;

	Uint32 type_colours[Pixel::MAX_COLOUR_COUNT] = { 0 };

	// TODO : (James) Can this be wrapped into something more flexible?
	uint8_t new_pixel_count = 0;
	Uint64 new_pixel_value[Pixel::MAX_NEW_VALUE_COUNT] = { 0 };
	Uint64 GetNewPixel();

	const std::array<uint8_t, 8>& GetPixelUpdateOrder();
	Uint32 GetRandomColour();
	// Method that converts the hex colour to 4f, and returns inside a passed in array
	static void GetColourAs4F(Uint32 colour, float* out_colour);
	void GetIndexAs4FColour(short index, float* out_colour) const;

	// Pixel Updating
	typedef void (BasePixel::*UpdateFunction)(PixelUpdateResult&, Uint64& pixel_value);
	UpdateFunction update_function = nullptr;
	
	void UpdatePixel(PixelUpdateResult& result, Uint64& pixel_value)
	{
		if (update_function) {
			(this->*update_function)(result, pixel_value);
		}
	}

 protected:
	XorShift _rng;

	uint8_t _chunk_order_counter = 0;
	uint8_t _pixel_update_order_count = 0;
	std::array<std::array<uint8_t, Chunk::NUM_DIRECTIONS>, Pixel::MAX_UPDATE_ORDER_COUNT> _pixel_update_order;

	constexpr void InsertPixelUpdateOrder(const uint8_t index, const std::vector<Chunk::WorldDir>& directions);
	void SetPixelName(const char* const name);

	// Constexpr helper function, sets the index of the pixel update order, primarily for the constructor
	#define SET_PIXEL_UPDATE_ORDER(index, ...) \
   do { \
   constexpr WorldDir values[] = { __VA_ARGS__, WorldDir::Undefined, WorldDir::Undefined, \
   WorldDir::Undefined, WorldDir::Undefined, WorldDir::Undefined, \
   WorldDir::Undefined, WorldDir::Undefined, WorldDir::Undefined, WorldDir::Undefined }; \
   for (uint8_t i = 0; i < Chunk::NUM_DIRECTIONS; i++) { \
   _pixel_update_order[index][i] = static_cast<uint8_t>(values[i]); \
   } \
   } while (false)

 private:
	#define SET_PIXEL_NAME(name) std::copy_n(name, sizeof(name), pixel_name.begin())

};