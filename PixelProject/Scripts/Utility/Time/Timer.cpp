#include "Timer.h"

void Timer::Start()
{
	_start_time = std::chrono::high_resolution_clock::now();
}

uint64_t Timer::Stop() const
{
	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - _start_time);
	return duration.count();
}