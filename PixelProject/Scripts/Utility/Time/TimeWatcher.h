#pragma once
#include <unordered_map>
#include "TimeTracker.h"

template<TimeFormat T = TimeFormat::Nano, typename Key = std::string>
class TimeWatcher
{
 public:
	void AddTimeTracker(const Key& name)
	{
		_time_trackers[name] = TimeTracker<T>();
	}

	void RemoveTimeTracker(const Key& name)
	{
		_time_trackers.erase(name);
	}

	TimeTracker<T>* operator[](const Key& name)
	{
		return &_time_trackers[name];
	}

	std::unordered_map<Key, TimeTracker<T>>& GetTimeTrackerCollection()
	{
		return _time_trackers;
	}

 private:
	std::unordered_map<Key, TimeTracker<T>> _time_trackers;
};
