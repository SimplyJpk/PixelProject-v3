#include "MemoryUsage.h"

void MemoryUsage::Update()
{
	GetMemory(true);

	if (_memory_history.size() >= MAX_MEMORY_HISTORY)
		_memory_history.erase(_memory_history.begin());
	_memory_history.push_back(_current_frame_memory);
}