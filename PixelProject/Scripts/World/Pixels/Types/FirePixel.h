#pragma once

#include "World/PixelMask.h"
#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class FirePixel final : public BasePixel
{
 public:
	FirePixel() : BasePixel(Pixel::PixelType::Fire)
	{
		SET_PIXEL_NAME("Fire");
		is_updateable = true;

		colour_count = 3;
		// Yellows
		type_colours[0] = 0xE25822FF;
		type_colours[1] = 0x800909FF;
		type_colours[2] = 0xD73502FF;

		_pixel_update_order_count = 2;
		SET_PIXEL_UPDATE_ORDER(0,
			WorldDir::North,
			WorldDir::NorthEast,
			WorldDir::NorthWest,
			WorldDir::SouthWest,
			WorldDir::SouthEast
		);
		SET_PIXEL_UPDATE_ORDER(1,
			WorldDir::North,
			WorldDir::NorthWest,
			WorldDir::NorthEast,
			WorldDir::SouthEast,
			WorldDir::SouthWest
		);

		update_function = static_cast<UpdateFunction>(&FirePixel::PixelUpdate);

		constexpr Uint8 FIRE_LIFETIMES[] = { 10, 14, 20, 12};
		new_pixel_value[0] = PixelMask::Lifetime::SetValue(pixel_index, FIRE_LIFETIMES[0]);
		new_pixel_value[1] = PixelMask::Lifetime::SetValue(pixel_index, FIRE_LIFETIMES[1]);
		new_pixel_value[2] = PixelMask::Lifetime::SetValue(pixel_index, FIRE_LIFETIMES[2]);	
		new_pixel_value[3] = PixelMask::Lifetime::SetValue(pixel_index, FIRE_LIFETIMES[3]);

		new_pixel_count = 4;
	}

	void PixelUpdate(PixelUpdateResult& data, Uint64& pixel_value)
	{
		pixel_value = PixelMask::Lifetime::DecrementValue(pixel_value);

		Uint8 lifetime = PixelMask::Lifetime::GetValue(pixel_value);
		// > 80% of the time, probably not going to be 0, so we can assume it is unlikely
		if (lifetime == 0) [[unlikely]] {
			data.SetLocal(PixelType::Space);
			return;
		}

		switch (data.Dir()) {
		case WorldDir::North:
		case WorldDir::NorthWest:
		case WorldDir::NorthEast:
		case WorldDir::SouthEast:
		case WorldDir::South:
		case WorldDir::SouthWest: {
			switch (data.NeighbourType()) {
			case PixelType::Space:
				_rng() % 2 == 0 ? data.Pass() : data.Fail();
				return;

				// case PixelType::Oil:
				//    data.SetLocalAndNeighbour(PixelType::Fire, PixelType::Fire);
				//    return;

			case PixelType::Wood:
				if (_rng() % 10 == 0) {
					data.SetLocalAndNeighbour(PixelType::Fire, PixelType::Fire);
					return;
				}
				data.Fail();
				return;

			case PixelType::Fire:
				data.SetLocal(PixelType::Fire);
				return;

			case PixelType::Water:
				// TODO : (James) Steam
				data.SetLocalAndNeighbour(PixelType::Space, PixelType::Space);
				return;
			default:
				break;
			}

			data.SetLocal(PixelType::Space);
		}
		default:
			break;
		}
		data.Fail();
	}

};
