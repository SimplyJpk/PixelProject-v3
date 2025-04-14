#pragma once
#include <vector>

class MemoryUsage
{
 public:
	void Update();

	int GetCurrentMemory() const
	{
		return _current_frame_memory;
	}
	int GetReferenceMemory() const
	{
		return _reference_frame_memory;
	}

	int GetMemory(const bool use_accurate = false);
	const std::vector<float>* GetMemoryHistory() const
	{
		return &_memory_history;
	}

	bool IsMemoryMore() const
	{
		if (_reference_frame_memory >= _current_frame_memory)
			return false;
		return true;
	}

	static constexpr int MAX_MEMORY_HISTORY = 100;

 private:
	int _current_frame_memory = 0;
	int _reference_frame_memory = 0;

	int _last_check_time = 0;

	std::vector<float> _memory_history = std::vector<float>(MAX_MEMORY_HISTORY, 0);

};
