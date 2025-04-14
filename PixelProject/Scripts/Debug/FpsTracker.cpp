#include "FpsTracker.h"

void FpsTracker::Update(const float delta_time) noexcept
{
	_current_fps = 1.0f / delta_time;

	// Initialize _max_fps and _min_fps with _current_fps
	_max_fps = _current_fps;
	_min_fps = _current_fps;

	_fps_history.erase(_fps_history.begin());
	_fps_history.push_back(_current_fps);
}

float FpsTracker::GetMaxFps() noexcept
{
	for (const float fps : _fps_history) {
		if (fps > _max_fps)
			_max_fps = fps;
	}
	return _max_fps;
}

float FpsTracker::GetMinFPS() noexcept
{
	for (const float fps : _fps_history) {
		if (fps < _min_fps)
			_min_fps = fps;
	}
	return _min_fps;
}
