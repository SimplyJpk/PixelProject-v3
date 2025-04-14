#pragma once

#include "imgui.h"

#include "Debug/Memory/MemoryUsage.h"
#include "Debug/FpsTracker.h"

class DebugStats
{
 public:
	DebugStats();
	virtual ~DebugStats();

	void OnDrawGUI(float delta_time);

 protected:
	static const inline ImVec4 RED_COLOUR = ImVec4(1, 0, 0, 1);
	static const inline ImVec4 GREEN_COLOUR = ImVec4(0, 1, 0, 1);

 private:
	MemoryUsage _memory_usage;
	FpsTracker _fps_tracker;

};
