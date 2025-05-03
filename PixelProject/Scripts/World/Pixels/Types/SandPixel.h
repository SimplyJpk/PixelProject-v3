#pragma once

#include "Config/ConfigFile.h"
#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class SandPixel final : public BasePixel
{
public:
	SandPixel() : BasePixel(Pixel::PixelType::Sand)
	{
		SetPixelName("Sand");
		is_updateable = true;

		colour_count = 7;
		// Yellows
		type_colours[0] = 0xE2D9AEFF;
		type_colours[1] = 0xE8BB9AFF;
		type_colours[2] = 0xEDC9AFFF;
		type_colours[3] = 0xF4A460FF;
		type_colours[4] = 0xF0E68CFF;
		type_colours[5] = 0xFAFAD2FF;
		type_colours[6] = 0xFFDEADFF;
		//
		type_colours[7] = 0xDEB887FF;

		_pixel_update_order_count = 2;
		SetPixelUpdateOrder(0, {WorldDir::South, WorldDir::SouthEast, WorldDir::SouthWest});
		SetPixelUpdateOrder(1, {WorldDir::South, WorldDir::SouthWest, WorldDir::SouthEast});

		update_function = static_cast<UpdateFunction>(&SandPixel::PixelUpdate);
	}

	void PixelUpdate(PixelUpdateResult &data, Uint64 &pixel_value)
	{
		switch (data.Dir())
		{
		case WorldDir::SouthEast:
		case WorldDir::South:
		case WorldDir::SouthWest:
		{
			switch (data.NeighbourType())
			{
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
