#pragma once

#include "World/PixelMask.h"
#include "World/Pixels/Base/BasePixel.h"

using namespace Chunk;
using namespace Pixel;

class FirePixel final : public BasePixel
{
 public:
	FirePixel()
	{
		pixel_type = Pixel::PixelType::Fire;
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
			WorldDir::SouthEast);
		SET_PIXEL_UPDATE_ORDER(1,
			WorldDir::North,
			WorldDir::NorthWest,
			WorldDir::NorthEast,
			WorldDir::SouthEast,
			WorldDir::SouthWest);

		distribution = std::uniform_int_distribution<int>(0, colour_count - 1);

		update_function = static_cast<UpdateFunction>(&FirePixel::PixelUpdate);
	}

	void PixelUpdate(PixelUpdateResult& data, Uint64& pixel_value)
	{
		// auto lifetime = PixelMask::Lifetime::GetValue(pixel_value);
		// lifetime -= 1;


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
