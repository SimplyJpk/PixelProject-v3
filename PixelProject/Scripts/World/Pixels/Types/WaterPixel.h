#pragma once

#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;

class WaterPixel : public BasePixel
{
 public:
	WaterPixel() : BasePixel(Pixel::PixelType::Water)
	{
		SET_PIXEL_NAME("Water");
		is_updateable = true;

		colour_count = 3;
		// Blues
		type_colours[0] = 0x0000FFFF;
		type_colours[1] = 0x2E2EE6FF;
		type_colours[2] = 0x1818F3FF;

		_pixel_update_order_count = 4;
		SET_PIXEL_UPDATE_ORDER(0, WorldDir::South, WorldDir::SouthEast, WorldDir::SouthWest, WorldDir::East);
		SET_PIXEL_UPDATE_ORDER(1, WorldDir::South, WorldDir::East, WorldDir::West, WorldDir::SouthEast);
		SET_PIXEL_UPDATE_ORDER(2, WorldDir::South, WorldDir::SouthWest, WorldDir::SouthEast, WorldDir::West);
		SET_PIXEL_UPDATE_ORDER(3, WorldDir::South, WorldDir::West, WorldDir::East, WorldDir::SouthWest);

		distribution = std::uniform_int_distribution<int>(0, colour_count - 1);

		update_function = static_cast<UpdateFunction>(&WaterPixel::PixelUpdate);
	}

	void PixelUpdate(PixelUpdateResult& data, Uint64& pixel_value)
	{
		switch (data.Dir()) {
		case WorldDir::East:
		case WorldDir::West:
		case WorldDir::SouthEast:
		case WorldDir::SouthWest: {
			switch (data.NeighbourType()) {
			case PixelType::Space:
				data.Pass();
				return;
			default:
				break;
			}
		}
		// Lets water spread out more easily
		case WorldDir::South:
			switch (data.NeighbourType()) {
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
