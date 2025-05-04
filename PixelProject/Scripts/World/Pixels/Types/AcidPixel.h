#pragma once

#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;

class AcidPixel : public BasePixel
{
public:
	AcidPixel() : BasePixel(Pixel::PixelType::Acid)
	{
		SetPixelName("Acid");
		is_updateable = true;

		colour_count = 7;
		// Bright, Corrosive Greens
		type_colours[0] = 0x00FF00FF;
		type_colours[1] = 0x32CD32FF;
		type_colours[2] = 0x98FB98FF;
		type_colours[3] = 0x00FC02FF;
		type_colours[4] = 0x4EF84EFF;
		type_colours[5] = 0xADFF2FFF;
		type_colours[6] = 0x7CFC00FF;
		//
		type_colours[7] = 0x00FA9AFF;

		_pixel_update_order_count = 4;
		SetPixelUpdateOrder(0, {WorldDir::South, WorldDir::SouthEast, WorldDir::SouthWest, WorldDir::East});
		SetPixelUpdateOrder(1, {WorldDir::South, WorldDir::East, WorldDir::West, WorldDir::SouthEast});
		SetPixelUpdateOrder(2, {WorldDir::South, WorldDir::SouthWest, WorldDir::SouthEast, WorldDir::West});
		SetPixelUpdateOrder(3, {WorldDir::South, WorldDir::West, WorldDir::East, WorldDir::SouthWest});

		update_function = static_cast<UpdateFunction>(&AcidPixel::PixelUpdate);
	}

	void PixelUpdate(PixelUpdateResult &data, Uint64 &pixel_value)
	{
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
