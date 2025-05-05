#pragma once

#include "World/PixelMask.h"
#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class SteamPixel final : public BasePixel
{
public:
	SteamPixel() : BasePixel(Pixel::PixelType::Steam)
	{
		SetPixelName("Steam");
		is_updateable = true;

		colour_count = 7;
		// Light, Ethereal Colors for Steam
		type_colours[0] = 0xF5F5F5FF;
		type_colours[1] = 0xE0E0E0FF;
		type_colours[2] = 0xD3D3D3FF;
		type_colours[3] = 0xADD8E6FF;
		type_colours[4] = 0xB0E0E6FF;
		type_colours[5] = 0xE6E6FAFF;
		type_colours[6] = 0xFFFAFAFF;
		//
		type_colours[7] = 0xF0FFFFFFF;

		_pixel_update_order_count = 2;
		SetPixelUpdateOrder(0, {WorldDir::North,
								WorldDir::NorthEast,
								WorldDir::NorthWest});
		SetPixelUpdateOrder(1,
							{WorldDir::North,
							 WorldDir::NorthWest,
							 WorldDir::NorthEast});

		update_function = static_cast<UpdateFunction>(&SteamPixel::PixelUpdate);

		constexpr Uint8 STEAM_LIFETIMES[] = {40, 46, 61, 57};
		new_pixel_value[0] = PixelMask::Lifetime::SetValue(pixel_index, STEAM_LIFETIMES[0]);
		new_pixel_value[1] = PixelMask::Lifetime::SetValue(pixel_index, STEAM_LIFETIMES[1]);
		new_pixel_value[2] = PixelMask::Lifetime::SetValue(pixel_index, STEAM_LIFETIMES[2]);
		new_pixel_value[3] = PixelMask::Lifetime::SetValue(pixel_index, STEAM_LIFETIMES[3]);

		new_pixel_count = 4;
	}

	void PixelUpdate(PixelUpdateResult &data, Uint64 &pixel_value, const Uint64 &neighbour_value) noexcept
	{
		if (!UpdateLifetime(data, pixel_value, PixelType::Water)) [[unlikely]]
		{
			return;
		}

		switch (data.Dir())
		{
		case WorldDir::North:
		case WorldDir::NorthWest:
		case WorldDir::NorthEast:
		{
			switch (data.NeighbourType())
			{
			case PixelType::Space:
				_rng() % 10 == 0 ? data.Fail() : data.Pass();
				return;

			case PixelType::Water:
				if (_rng() % 10 == 0)
				{
					data.SetLocal(PixelType::Water);
					return;
				}
				data.Pass();
				return;
			default:
				break;
			}

			// data.SetLocal(PixelType::Water);
		}
		default:
			break;
		}
		data.Fail();
	}
};
