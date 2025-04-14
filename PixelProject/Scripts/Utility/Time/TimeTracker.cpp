#include "TimeTracker.h"

#include <numeric>
#include <algorithm>

#include "Utility/Console.h"

template<TimeFormat T>
TimeTracker<T>::TimeTracker(const int calculatePercentilesEveryN, const size_t maxHistorySize)
{
	_max_history_size = maxHistorySize;
	if (_max_history_size == 0)
		_max_history_size = DEFAULT_MAX_HISTORY_SIZE;
	_history = std::vector<float>(_max_history_size);
	_calculate_percentiles_every_n = calculatePercentilesEveryN;
}

template<TimeFormat T>
void TimeTracker<T>::CalculatePercentiles()
{
	if (_calculate_percentiles_every_n == 0)
		return;

	_calculate_percentiles_counter++;
	if (_calculate_percentiles_counter < _calculate_percentiles_every_n)
		return;

	_calculate_percentiles_counter = 0;
	std::vector<float> sorted_history = _history;
	std::ranges::sort(sorted_history, std::ranges::greater{});

	_max_time = sorted_history.back();

	size_t index_0_5 = static_cast<size_t>(0.005f * sorted_history.size());
	size_t index_2 = static_cast<size_t>(0.02f * sorted_history.size());
	size_t index_5 = static_cast<size_t>(0.05f * sorted_history.size());

	_percentile_low_0_5 = sorted_history[index_0_5];
	_percentile_low_2 = sorted_history[index_2];
	_percentile_low_5 = sorted_history[index_5];

	// calculate average
	_average = std::accumulate(_history.begin(), _history.end(), 0.0f) / _history.size();
	_median = sorted_history[sorted_history.size() / 2];
}

template<TimeFormat T>
float TimeTracker<T>::GetTimeSinceStart() const
{
	const auto duration = GetDuration(_start_time);
	return duration;
}

template<TimeFormat T>
float TimeTracker<T>::GetDuration(const std::chrono::high_resolution_clock::time_point start)
{
	const auto end_time = std::chrono::high_resolution_clock::now();
	const auto duration = end_time - start;

	switch (T) {
	case TimeFormat::Nano:
		return std::chrono::duration<float, std::nano>(duration).count();
	case TimeFormat::Micro:
		return std::chrono::duration<float, std::micro>(duration).count();
	case TimeFormat::Milli:
		return std::chrono::duration<float, std::milli>(duration).count();
	case TimeFormat::Second:
		return std::chrono::duration<float>(duration).count();
	case TimeFormat::Minute:
		return std::chrono::duration<float, std::ratio<60>>(duration).count();
	case TimeFormat::Hour:
		return std::chrono::duration<float, std::ratio<3600>>(duration).count();
	}
	return 0.0f;
}

template
class TimeTracker<TimeFormat::Nano>;

template
class TimeTracker<TimeFormat::Micro>;

template
class TimeTracker<TimeFormat::Milli>;

template
class TimeTracker<TimeFormat::Second>;

template
class TimeTracker<TimeFormat::Minute>;

template
class TimeTracker<TimeFormat::Hour>;