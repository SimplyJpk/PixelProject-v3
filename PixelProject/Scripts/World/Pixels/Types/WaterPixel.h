#pragma once

#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;

class WaterPixel : public BasePixel
{
public:
	WaterPixel() : BasePixel(Pixel::PixelType::Water)
	{
		SetPixelName("Water");
		is_updateable = true;

		colour_count = 7;
		// Darker, Uniform Blues
		type_colours[0] = 0x0000FFFF;
		type_colours[1] = 0x2E2EE6FF;
		type_colours[2] = 0x1818F3FF;
		type_colours[3] = 0x1818F4FF;
		type_colours[4] = 0x234E6CFF;
		type_colours[5] = 0x325F8FFF;
		type_colours[6] = 0x1718F4FF;

		type_colours[7] = 0x0F3A6DFF;

		_pixel_update_order_count = 4;
		SetPixelUpdateOrder(0, {WorldDir::South, WorldDir::SouthEast, WorldDir::SouthWest, WorldDir::East});
		SetPixelUpdateOrder(1, {WorldDir::South, WorldDir::East, WorldDir::West, WorldDir::SouthEast});
		SetPixelUpdateOrder(2, {WorldDir::South, WorldDir::SouthWest, WorldDir::SouthEast, WorldDir::West});
		SetPixelUpdateOrder(3, {WorldDir::South, WorldDir::West, WorldDir::East, WorldDir::SouthWest});

		update_function = static_cast<UpdateFunction>(&WaterPixel::PixelUpdate);

		new_pixel_value[0] = PixelMask::Density::SetValue(pixel_index, 3);
		new_pixel_count = 1;
	}

	void PixelUpdate(PixelUpdateResult &data, Uint64 &pixel_value, const Uint64 &neighbour_value) noexcept
	{
		if (UpdateDensity(data, pixel_value, neighbour_value, _rng)) [[unlikely]]
		{
			data.Pass();
			return;
		}

		switch (data.Dir())
		{
		case WorldDir::East:
		case WorldDir::West:
		case WorldDir::SouthEast:
		case WorldDir::SouthWest:
		{
			switch (data.NeighbourType())
			{
			case PixelType::Space:
				data.Pass();
				return;
			default:
				break;
			}
		}
		// Lets water spread out more easily
		case WorldDir::South:
			switch (data.NeighbourType())
			{
			case PixelType::Space:
				_rng() % 4 == 0 ? data.Fail() : data.Pass();
				return;
			default:
				break;
			}
		default:
			break;
		}
		data.Fail();
	}
};
