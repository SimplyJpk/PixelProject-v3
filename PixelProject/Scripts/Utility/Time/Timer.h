#pragma once
#include <chrono>

// Timer class that can be used to measure time in nanoseconds
class Timer
{
 public:
	void Start();
	uint64_t Stop() const;

 private:
	std::chrono::high_resolution_clock::time_point _start_time;
};