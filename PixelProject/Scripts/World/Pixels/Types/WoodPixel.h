#pragma once

#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class WoodPixel final : public BasePixel
{
public:
	WoodPixel() : BasePixel(Pixel::PixelType::Wood)
	{
		SetPixelName("Wood");
		is_updateable = true;

		colour_count = 7;
		// Yellows
		type_colours[0] = 0x421010FF;
		type_colours[1] = 0x4E3211FF;
		type_colours[2] = 0x4E1311FF;
		type_colours[3] = 0x8B0000FF;
		type_colours[4] = 0x5F4628FF;
		type_colours[5] = 0x8c403bFF;
		type_colours[6] = 0x801f1fFF;
		//
		type_colours[7] = 0xCD5C5CFF;

		_pixel_update_order_count = 0;
	}
};
