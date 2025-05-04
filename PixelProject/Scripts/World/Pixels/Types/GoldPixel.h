#pragma once

#include "Config/ConfigFile.h"
#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class GoldPixel final : public BasePixel
{
public:
	GoldPixel() : BasePixel(Pixel::PixelType::Gold)
	{
		SetPixelName("Gold");
		is_updateable = true;

		colour_count = 7;
		// Metallic Gold
		type_colours[0] = 0xFFD700FF;
		type_colours[1] = 0xDAA520FF;
		type_colours[2] = 0xB8860BFF;
		type_colours[3] = 0xFFEC8BFF;
		type_colours[4] = 0xE5C100FF;
		type_colours[5] = 0xE3DA81FF;
		type_colours[6] = 0xBDB76BFF;
		//
		type_colours[7] = 0xCD7F32FF;

		_pixel_update_order_count = 2;
		SetPixelUpdateOrder(0, {WorldDir::South, WorldDir::SouthEast, WorldDir::SouthWest});
		SetPixelUpdateOrder(1, {WorldDir::South, WorldDir::SouthWest, WorldDir::SouthEast});

		update_function = static_cast<UpdateFunction>(&GoldPixel::PixelUpdate);
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
