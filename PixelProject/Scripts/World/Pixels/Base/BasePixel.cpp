#include "BasePixel.h"
#include "Utility/Console.h"

using namespace Chunk;

BasePixel::BasePixel()
{
	// Flood all update orders with Undefined directions
	SET_PIXEL_UPDATE_ORDER(0, WorldDir::Undefined);
	SET_PIXEL_UPDATE_ORDER(1, WorldDir::Undefined);
	SET_PIXEL_UPDATE_ORDER(2, WorldDir::Undefined);
	SET_PIXEL_UPDATE_ORDER(3, WorldDir::Undefined);
}

const std::array<uint8_t, 8>& BasePixel::GetPixelUpdateOrder()
{
	if (_pixel_update_order_count > 1) {
		_chunk_order_counter++;
		if (_chunk_order_counter >= _pixel_update_order_count) {
			_chunk_order_counter = 0;
		}
	}
	return _pixel_update_order[_chunk_order_counter];
}

constexpr void BasePixel::InsertPixelUpdateOrder(const uint8_t index, const std::vector<Chunk::WorldDir>& directions)
{
	if (index < Pixel::MAX_UPDATE_ORDER_COUNT) {
		for (uint8_t i = 0; i < Chunk::NUM_DIRECTIONS; i++) {
			if (i < directions.size())
				_pixel_update_order[index][i] = static_cast<uint8_t>(directions[i]);
			else
				_pixel_update_order[index][i] = Chunk::NUM_DIRECTIONS;
		}
	}
}

void BasePixel::SetPixelName(const char* const name)
{
	if (strlen(name) > 0) {
		strcpy_s(pixel_name.data(), Pixel::MAX_UPDATE_ORDER_COUNT, name);
	}
}

Uint32 BasePixel::GetRandomColour()
{
	// TODO : (James) Consider adding a "buffer" and iterating through that instead of RNG here which is kinda slow, and pointless for (2-4 values)
	return type_colours[distribution(rand_engine)];
}

void BasePixel::GetColourAs4F(const Uint32 colour, float* out_colour)
{
	static constexpr float RECIPROCAL = 1.0f / 255.0f;
	out_colour[0] = ((colour >> 24) & 0xFF) * RECIPROCAL; // RR
	out_colour[1] = ((colour >> 16) & 0xFF) * RECIPROCAL; // GG
	out_colour[2] = ((colour >> 8) & 0xFF) * RECIPROCAL;  // BB
	out_colour[3] = (colour & 0xFF) * RECIPROCAL;         // AA
}

void BasePixel::GetIndexAs4FColour(const short index, float* out_colour) const
{
	if (index < colour_count) {
		GetColourAs4F(type_colours[index], out_colour);
	}
	else {
		out_colour[0] = 0.0f;
		out_colour[1] = 0.0f;
		out_colour[2] = 0.0f;
		out_colour[3] = 0.0f;
	}
}

Uint32 BasePixel::GetNewPixel()
{
	if (new_pixel_count > 0) {
		return new_pixel_value[distribution(rand_engine)];
	}
	return static_cast<Uint32>(pixel_index);
}

