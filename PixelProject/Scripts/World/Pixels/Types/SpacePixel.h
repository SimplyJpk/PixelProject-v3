#pragma once

#include "World/Pixels/Base/BasePixel.h"

class SpacePixel final : public BasePixel
{
 public:
	SpacePixel() : BasePixel(Pixel::PixelType::Space)
	{
		SetPixelName("Space");
		is_updateable = false;

		colour_count = 1;
		// Space
		type_colours[0] = 0x00000000;

		_pixel_update_order_count = 0;
	}

};
