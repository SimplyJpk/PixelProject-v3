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

		colour_count = 3;
		// Yellows
		type_colours[0] = 0x421010FF;
		type_colours[1] = 0x4E3211FF;
		type_colours[2] = 0x4E1311FF;

		_pixel_update_order_count = 0;

	}

};
