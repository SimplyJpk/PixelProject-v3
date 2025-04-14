#include "DebugStats.h"

#include "UI/UIManager.h"

DebugStats::DebugStats()
{
	UIManager::GetInstance()->AddGUIDrawCallback(std::bind(&DebugStats::OnDrawGUI, this, std::placeholders::_1));
}

DebugStats::~DebugStats()
{
	UIManager::GetInstance()->RemoveGUIDrawCallback(std::bind(&DebugStats::OnDrawGUI, this, std::placeholders::_1));
}

void DebugStats::OnDrawGUI(float delta_time)
{
	ImGui::Begin("Debug Stats");

	ImGui::Text("FPS");

	_fps_tracker.Update(delta_time);

	ImGui::Text("IMGUI Reported FPS: %0.2f", ImGui::GetIO().Framerate);
	ImGui::Text("Delta Time: %0.4f", delta_time);

	ImGui::Text("Estimated FPS from Delta: %0.2f", _fps_tracker.GetCurrentFps());
	const auto minFPS = _fps_tracker.GetMinFPS();
	const auto maxFPS = _fps_tracker.GetMaxFps();

//	ImGui::PlotLines("",
//		_fps_tracker.GetFpsHistoryData(),
//		FpsTracker::MAX_FPS_HISTORY,
//		0,
//		0,
//		minFPS,
//		maxFPS,
//		ImVec2(ImGui::GetColumnWidth(), 50));

//	ImGui::Text("Approximate History Duration: %0.2f Sec", _fps_tracker.GetApproxHistoryDuration());
	ImGui::Text("Min FPS: %0.2f", minFPS);
	ImGui::Text("Max FPS: %0.2f", maxFPS);
	ImGui::Separator();
	ImGui::Text("Memory Usage");
	_memory_usage.Update();
	ImGui::TextColored(_memory_usage.IsMemoryMore() ? RED_COLOUR : GREEN_COLOUR,
		"Memory Used: %ikb",
		_memory_usage.GetCurrentMemory() / 1000);

//	ImGui::PlotLines("",
//		_memory_usage.GetMemoryHistory()->data(),
//		MemoryUsage::MAX_MEMORY_HISTORY,
//		0,
//		0,
//		50000,
//		100000,
//		ImVec2(ImGui::GetColumnWidth(), 50));

	ImGui::End();
}
