#pragma once

#include "Interfaces/ISerialize.h"

struct ConfigFile final : public ISerialize
{
	ConfigFile()
	{
		StartLoad(true);
	}

	short window_size_x = 1280;
	short window_size_y = 720;

	float target_frames_per_second = 120.0f;

	// Number of pixel updates that will run per second, regardless of the target frames per second. (Fixed timestep)
	uint8_t fixed_pixel_updates_per_second = 60;
	// Pixel updates that can run in a single frame, this is important as pixels update in a fixed time step which can lock up the game should frames drop below the target for a long time
	uint8_t max_pixel_updates_per_frame = 10;

	SaveTypes SaveType() override
	{
		return SaveTypes::Json;
	}

	std::string FilePath() override
	{
		return "config";
	}

	void Save(cereal::JSONOutputArchive out_archive) override
	{
		out_archive(CEREAL_NVP(window_size_x));
		out_archive(CEREAL_NVP(window_size_y));
		out_archive(CEREAL_NVP(target_frames_per_second));
		out_archive(CEREAL_NVP(fixed_pixel_updates_per_second));
		out_archive(CEREAL_NVP(max_pixel_updates_per_frame));
	}

	void Load(cereal::JSONInputArchive in_archive) override
	{
		in_archive(CEREAL_NVP(window_size_x));
		in_archive(CEREAL_NVP(window_size_y));
		in_archive(CEREAL_NVP(target_frames_per_second));
		in_archive(CEREAL_NVP(fixed_pixel_updates_per_second));
		in_archive(CEREAL_NVP(max_pixel_updates_per_frame));
	}
};
