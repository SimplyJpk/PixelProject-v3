#pragma once
#include <limits>
#include <vector>

class FpsTracker
{
 public:
	void Update(const float delta_time) noexcept;

	float GetCurrentFps() const noexcept
	{
		return _current_fps;
	}
	// Returns the max fps from the history of fps, calculated every call. [expensive]
	float GetMaxFps() noexcept;
	// Returns the min fps from the history of fps, calculated every call. [expensive]
	float GetMinFPS() noexcept;

	float GetApproxHistoryDuration() const noexcept
	{
		return MAX_FPS_HISTORY / _current_fps;
	}

	const std::vector<float>* GetFpsHistory() const noexcept
	{
		return &_fps_history;
	}
	const float* GetFpsHistoryData() const noexcept
	{
		return _fps_history.data();
	}

	static constexpr int MAX_FPS_HISTORY = 500;

 private:
	float _current_fps = 0;

	float _max_fps = std::numeric_limits<float>::min();
	float _min_fps = std::numeric_limits<float>::max();

	std::vector<float> _fps_history = std::vector<float>(MAX_FPS_HISTORY, 0);
};
