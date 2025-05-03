#pragma once

#include "World/Pixels/Base/BasePixel.h"

class DirtPixel final : public BasePixel
{
public:
	DirtPixel() : BasePixel(Pixel::PixelType::Dirt)
	{
		SetPixelName("Dirt");
		is_updateable = false;

		colour_count = 7;
		// Browns
		type_colours[0] = 0x964b00FF;
		type_colours[1] = 0x967100FF;
		type_colours[2] = 0x962600FF;
		type_colours[3] = 0x8B4513FF;
		type_colours[4] = 0xA0522DFF;
		type_colours[5] = 0xC2A48CFF;
		type_colours[6] = 0xBC8F8FFF;
		//
		type_colours[7] = 0xC0C0C0FF;

		_pixel_update_order_count = 0;

		// update_function = static_cast<UpdateFunction>(&DirtPixel::PixelUpdate);
	}

	// void PixelUpdate(PixelUpdateResult& result)
	// {
	//    result.Fail();
	// }
};
