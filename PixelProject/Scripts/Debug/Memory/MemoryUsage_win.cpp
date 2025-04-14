#include "MemoryUsage.h"

#ifdef _WIN32

#include <Windows.h>
#include <Psapi.h>
#include <SDL3/SDL_timer.h>

int MemoryUsage::GetMemory(const bool use_accurate)
{
	// Get the handle to this process
	const HANDLE myHandle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(myHandle, &pmc, sizeof(pmc))) {
		_current_frame_memory = static_cast<int>(pmc.WorkingSetSize) / (use_accurate ? 1 : 1024);
		const auto sdlTicks = SDL_GetTicks();
		if (sdlTicks - _last_check_time > 2000) {
			_reference_frame_memory = _current_frame_memory;
			_last_check_time = sdlTicks;
		}
		return _current_frame_memory;
	}
	else
		return -1;
}

#endif