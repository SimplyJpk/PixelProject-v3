#pragma once

#include "World/Pixels/Base/BasePixel.h"

class SpacePixel final : public BasePixel
{
 public:
	SpacePixel()
	{
		pixel_type = Pixel::PixelType::Space;
		SET_PIXEL_NAME("Space");
		is_updateable = false;

		colour_count = 1;
		// Space
		type_colours[0] = 0x00000000;

		_pixel_update_order_count = 0;

		distribution = std::uniform_int_distribution<int>(0, colour_count - 1);

	}

};
