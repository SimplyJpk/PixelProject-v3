#include "UIManager.h"

#include <cstdlib>

#include "imgui.h"
#include <implot.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

void UIManager::Init(SDL_Window* window, SDL_GLContext* glContext)
{
	_window = window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 130");

	// ImGui::SetWindowSize("Debug Window", ImVec2(240, 240));
	// ImGui::SetWindowPos("Debug Window", ImVec2(static_cast<float>(245), 15));

	_debug_stats = new DebugStats();
}

void UIManager::DrawGUI()
{
	ImGui::Begin("Debug Window");

	static int unique_values[10] = { 0 };
	const auto random_value = _rng() % 10;

	unique_values[random_value]++;

	ImGui::Text("Random Number 0-9: %d", random_value);
	// count standard deviation
	float sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += unique_values[i];
	}
	const float mean = sum / 10;
	ImGui::Text("Mean: %f", mean);
	ImGui::Text("Standard Deviation: ");
	float standard_deviation = 0;
	int largest = 0;
	int smallest = int(1e9);
	for (int i = 0; i < 10; i++) {
		standard_deviation += (unique_values[i] - mean) * (unique_values[i] - mean);
		if (unique_values[i] > largest) {
			largest = unique_values[i];
		}
		if (unique_values[i] < smallest) {
			smallest = unique_values[i];
		}
	}
	standard_deviation = sqrt(standard_deviation / 10);
	ImGui::Text("%f", standard_deviation);
	ImGui::Text("Largest: %d", largest);
	ImGui::Text("Smallest: %d", smallest);
	ImGui::Text("Range: %d", largest - smallest);

	ImGui::End();

	_onGUIDraw.Invoke(ImGui::GetIO().DeltaTime);
}

void UIManager::BeginFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void UIManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}
