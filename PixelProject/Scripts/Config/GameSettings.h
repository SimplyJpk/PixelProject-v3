#pragma once
#include "ConfigFile.h"
#include "Math/Math.h"

#include "Utility/Console.h"

struct GameSettings
{

	void LoadSettings(std::unique_ptr<ConfigFile>& config)
	{
		_config = std::move(config);

		Init();
	}

	/* Game Settings that will be global */
	IVec2 screen_size = IVec2(1280, 720);
	float aspect_ratio = static_cast<float>(screen_size.x) / static_cast<float>(screen_size.y);

	// Game Fixed TimeStep
	uint8_t target_sand_updates_per_seconds = 60;
	double target_sand_update_time = 1000.0 / target_sand_updates_per_seconds;

	void IncreaseFixedTimeStep(const int8_t count)
	{
		target_sand_updates_per_seconds += count;
		target_sand_update_time = 1000.0 / target_sand_updates_per_seconds;
	}

	float target_frames_per_second = 60.0f;
	float calculated_frame_delay = 1000.0f / target_frames_per_second;
	// Max number of updates we want the fixed update to run, this prevents complete lockup.
	uint8_t max_sand_updates_per_frame = 10;

 protected:
	std::unique_ptr<ConfigFile> _config;

	void Init()
	{
		screen_size.x = _config->window_size_x;
		screen_size.y = _config->window_size_y;
		// Set aspect ratio for future use
		aspect_ratio = static_cast<float>(screen_size.x) / static_cast<float>(screen_size.y);
		// Target updates from config (FPS)
		target_frames_per_second = _config->target_frames_per_second;

		calculated_frame_delay = 1000.0f / target_frames_per_second;

		target_sand_updates_per_seconds = _config->fixed_pixel_updates_per_second;
		target_sand_update_time = 1000.0 / target_sand_updates_per_seconds;

		max_sand_updates_per_frame = _config->max_pixel_updates_per_frame;

		INFO_LOG("Settings\n{0}x{1}\n{2} FPS\n{3} Fixed Updates Per Second\n{4} Max Fixed Updates Per Frame",
			screen_size.x,
			screen_size.y,
			target_frames_per_second,
			target_sand_updates_per_seconds,
			max_sand_updates_per_frame);
	}

};
