#pragma once

#include "Config/ConfigFile.h"
#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class SandPixel final : public BasePixel
{
 public:
	SandPixel()
	{
		pixel_type = Pixel::PixelType::Sand;
		SET_PIXEL_NAME("Sand");
		is_updateable = true;

		colour_count = 3;
		// Yellows
		type_colours[0] = 0xE2D9AEFF;
		type_colours[1] = 0xE8BB9AFF;
		type_colours[2] = 0xEDC9AFFF;

		_pixel_update_order_count = 2;
		SET_PIXEL_UPDATE_ORDER(0, WorldDir::South, WorldDir::SouthEast, WorldDir::SouthWest);
		SET_PIXEL_UPDATE_ORDER(1, WorldDir::South, WorldDir::SouthWest, WorldDir::SouthEast);

		distribution = std::uniform_int_distribution<int>(0, colour_count - 1);

		update_function = static_cast<UpdateFunction>(&SandPixel::PixelUpdate);
	}

	void PixelUpdate(PixelUpdateResult& data, Uint32& pixel_value)
	{
		switch (data.Dir()) {
		case WorldDir::SouthEast:
		case WorldDir::South:
		case WorldDir::SouthWest: {
			switch (data.NeighbourType()) {
			case PixelType::Space:
				data.Pass();
				return;
			case PixelType::Water:
				(_rng() % 3 == 0 ? data.Pass() : data.Fail());
				return;
			default:
				break;
			}
			return;
		}
		default:
			break;
		}
		data.Fail();
	}

};
