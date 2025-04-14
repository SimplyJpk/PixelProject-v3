#pragma once
#include <vector>
#include <chrono>

#include "Utility/Console.h"

enum class TimeFormat
{
	Nano,
	Micro,
	Milli,
	Second,
	Minute,
	Hour
};

// A class that can be used to track time and store the history of the time in a specified format (Nano, Milli, Second, Minute, Hour)
// Use `Timer` class if you only need 1 time measurement in nanoseconds (Smaller footprint, more performant)
// Note: The history size is limited to the maximum history size specified in the constructor, this space is preallocated on construction to avoid reallocation as it resizes.
template<TimeFormat T = TimeFormat::Nano>
class TimeTracker
{
	static constexpr size_t DEFAULT_MAX_HISTORY_SIZE = 300;
 public:
	explicit TimeTracker(const int calculatePercentilesEveryN = 0,
		const size_t maxHistorySize = DEFAULT_MAX_HISTORY_SIZE);

	// Starts the timer
	void Start()
	{
		_start_time = std::chrono::high_resolution_clock::now();
	}
	// Stops the timer, returns the duration and stores it in the history
	float Stop()
	{
		const auto duration = GetDuration(_start_time);

		_history.erase(_history.begin());
		_history.push_back(duration);

		CalculatePercentiles();

		return duration;
	}

	// Stops the timer without returning the duration as well, tiny performance improvement
	void StopWithoutReturn()
	{
		const auto duration = GetDuration(_start_time);

		_history.erase(_history.begin());
		_history.push_back(duration);

		CalculatePercentiles();
	}

	// Returns the time since the start of the timer, this does not stop the timer
	float GetTimeSinceStart() const;

	const std::vector<float>& GetHistory() const
	{
		return _history;
	}

	const float GetLastTime() const
	{
		return _history.back();
	}

	size_t GetMaxHistorySize() const
	{
		return _max_history_size;
	}

	void CalculatePercentiles();

	float GetMaxTime() const
	{
		return _max_time;
	}
	float GetPercentile_0_001() const
	{
		return _percentile_low_0_5;
	}
	float GetPercentile_0_010() const
	{
		return _percentile_low_2;
	}
	float GetPercentile_0_050() const
	{
		return _percentile_low_5;
	}

	float GetAverageTime() const
	{
		return _average;
	}
	float GetMedianTime() const
	{
		return _median;
	}

 protected:
	static float GetDuration(const std::chrono::high_resolution_clock::time_point start);

 private:
	std::chrono::high_resolution_clock::time_point _start_time;

	std::vector<float> _history;
	size_t _max_history_size;

	float _max_time = 0.0f;
	float _percentile_low_0_5 = 0.0f;
	float _percentile_low_2 = 0.0f;
	float _percentile_low_5 = 0.0f;
	float _average = 0.0f;
	float _median = 0.0f;

	int _calculate_percentiles_every_n = 30;
	int _calculate_percentiles_counter = 0;
};