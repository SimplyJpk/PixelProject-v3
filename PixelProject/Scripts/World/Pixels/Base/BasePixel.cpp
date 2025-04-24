#include "BasePixel.h"
#include "Utility/Console.h"
#include "World/WorldConstants.h"

using namespace Chunk;

BasePixel::BasePixel(Pixel::PixelType _pixel_type) noexcept
{
	// Flood all update orders with Undefined directions
	SetPixelUpdateOrder(0, {WorldDir::Undefined});
	SetPixelUpdateOrder(1, {WorldDir::Undefined});
	SetPixelUpdateOrder(2, {WorldDir::Undefined});
	SetPixelUpdateOrder(3, {WorldDir::Undefined});

	// Construct some default values based on our PixelType to avoid additional processing outside of the derived classes
	this->pixel_index = static_cast<uint8_t>(_pixel_type);
	this->pixel_type = _pixel_type;
}

const std::array<uint8_t, 8> &BasePixel::GetPixelUpdateOrder() noexcept
{
	if (_pixel_update_order_count > 1)
	{
		_chunk_order_counter++;
		if (_chunk_order_counter >= _pixel_update_order_count)
		{
			_chunk_order_counter = 0;
		}
	}
	return _pixel_update_order[_chunk_order_counter];
}

constexpr void BasePixel::InsertPixelUpdateOrder(const uint8_t index, const std::vector<Chunk::WorldDir> &directions) noexcept
{
	if (index < Pixel::MAX_UPDATE_ORDER_COUNT)
	{
		for (uint8_t i = 0; i < Chunk::NUM_DIRECTIONS; i++)
		{
			if (i < directions.size())
				_pixel_update_order[index][i] = static_cast<uint8_t>(directions[i]);
			else
				_pixel_update_order[index][i] = Chunk::NUM_DIRECTIONS;
		}
	}
}

void BasePixel::SetPixelName(const std::string_view name) noexcept
{
	size_t len = std::min(name.size(), pixel_name.size() - 1);
	std::copy_n(name.begin(), len, pixel_name.begin());
	pixel_name[len] = '\0'; // Null terminate
}

Uint32 BasePixel::GetRandomColour() noexcept
{
	// TODO : (James) Consider adding a "buffer" and iterating through that instead of RNG here which is kinda slow, and pointless for (2-4 values)
	return type_colours[_rng() % colour_count];
}

void BasePixel::GetColourAs4F(const Uint32 colour, float *out_colour) noexcept
{
	static constexpr float RECIPROCAL = 1.0f / 255.0f;
	out_colour[0] = ((colour >> 24) & 0xFF) * RECIPROCAL; // RR
	out_colour[1] = ((colour >> 16) & 0xFF) * RECIPROCAL; // GG
	out_colour[2] = ((colour >> 8) & 0xFF) * RECIPROCAL;  // BB
	out_colour[3] = (colour & 0xFF) * RECIPROCAL;		  // AA
}

void BasePixel::GetIndexAs4FColour(const short index, float *out_colour) const noexcept
{
	if (index < colour_count)
	{
		GetColourAs4F(type_colours[index], out_colour);
	}
	else
	{
		out_colour[0] = 0.0f;
		out_colour[1] = 0.0f;
		out_colour[2] = 0.0f;
		out_colour[3] = 0.0f;
	}
}

Uint64 BasePixel::GetNewPixel() noexcept
{
	if (new_pixel_count > 0)
	{
		return new_pixel_value[_rng() % new_pixel_count];
	}
	return static_cast<Uint64>(pixel_index);
}

void BasePixel::SetPixelUpdateOrder(size_t index, std::initializer_list<Chunk::WorldDir> dirs) noexcept
{
	size_t i = 0;
	for (auto dir : dirs)
	{
		if (i >= Chunk::NUM_DIRECTIONS)
			break;
		_pixel_update_order[index][i++] = static_cast<uint8_t>(dir);
	}
	// Fill remaining with Undefined
	for (; i < Chunk::NUM_DIRECTIONS; ++i)
	{
		_pixel_update_order[index][i] = static_cast<uint8_t>(Chunk::WorldDir::Undefined);
	}
}
