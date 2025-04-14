#include "Game.h"

#include <GL/glew.h>
#include <chrono>

#include "imgui_internal.h"
#include <implot.h>
#include "Config/ConfigFile.h"

#include "Rendering/Shaders/ShaderManager.h"

#include "World/Pixels/WorldDataHandler.h"
#include "UI/Paint/PaintSelector.h"

Game::Game(SDL_GLContext* gl_context, SDL_Window* gl_window, std::shared_ptr<GameSettings>& settings) :
	_input_manager(nullptr), _camera(nullptr)
{
	_context = gl_context;
	_window = gl_window;
	_settings = settings;

	_ui_manager = UIManager::GetInstance();

	auto instance = WorldDataHandler::GetInstance();

	_fixed_step_step_time = _settings->target_sand_update_time;
	_fixed_step_max_steps = _settings->max_sand_updates_per_frame;
}

bool Game::Initialize()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, 1280, 720);

	// Un-sync FPS from monitor
	SDL_GL_SetSwapInterval(0);

	// Shaders
	const auto shaderManager = ShaderManager::GetInstance();
	Shader* defaultShader = shaderManager->CreateShaderProgramFromFiles(
		GetShaderMask(ShaderMask::MVertex, ShaderMask::MFragment), "orthoWorld", "shaders/orthoWorld");
	shaderManager->SetDefaultShader(defaultShader);

	glUniform1i(defaultShader->GetUniformLocation("ourTexture"), 0);
	glUniform1i(defaultShader->GetUniformLocation("noiseTextureIndex"), 1);

	// Input System
	_input_manager = InputManager::GetInstance();

	// User Interface
	_ui_manager->Init(_window, _context);

	_paint_selector = PaintSelector::GetInstance();
	_paint_selector->GenerateTextures(_settings.get());

	_camera = new FlyCamera();
	_camera->SetOrthographic(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

	_world_simulator = new WorldSimulator(defaultShader, _settings, _camera);
	WorldDataHandler::GetInstance()->SetUniformData(defaultShader);

	return true;
}

void Game::Run()
{
	typedef std::chrono::steady_clock clock;
	typedef std::chrono::duration<float, std::milli> duration;

	auto deltaClock = clock::now();
	_is_running = true;

	_input_manager->AddKeyListener(KeyCode::U, "Temp_FixedUpdate", [this](SDL_Event& event, bool state)
	{
	  if (state) {
		  _world_simulator->FixedUpdate();
	  }
	});

	static bool didSandUpdate = false;
	_input_manager->AddKeyListener(KeyCode::Zero, "SingleFixedUpdate", [this](SDL_Event& event, bool state)
	{
		if (state && !didSandUpdate) {
			didSandUpdate = true;
		  _world_simulator->FixedUpdate();
	  }
		else if (!state) {
			didSandUpdate = false;
		}
	});

	// TODO : (James) While GameStateManager != IsShuttingDown??
	_minimum_delta_time = 1000.0f / _settings->target_frames_per_second;
	while (_is_running) {
		_delta_time += duration(clock::now() - deltaClock).count();
		deltaClock = clock::now();

		_fixed_time += _delta_time;
		FixedUpdate();

		InputUpdate();

		// Runs once a frame
		Update();
		_camera->Update(_delta_time);

		// Draw
		Render();
		_paint_selector->Draw();

		// UI
		RenderUI();

		SDL_GL_SwapWindow(_window);
		_delta_time = 0.0;
	}
}

void Game::FixedUpdate()
{
	uint8_t fixedUpdateCount = 0;
	while (_fixed_time >= _fixed_step_step_time) {
		_fixed_time -= _fixed_step_step_time;
		_sand_time_tracker.Start();

		// Main Update Logic
		_world_simulator->FixedUpdate();

		// Protective logic to prevent falling into an infinite loop
		fixedUpdateCount++;

		_sand_time_tracker.StopWithoutReturn();

		if (fixedUpdateCount >= _fixed_step_max_steps) {
			LOG_WARNING("[Game] FixedUpdate() Falling behind, {} updates this frame ({}ms), remaining {}ms",
				fixedUpdateCount,
				std::round(_fixed_step_step_time * fixedUpdateCount * 100) / 100.0f,
				std::round(_fixed_time * 100) / 100.0f);
			break;
		}
	}
}

void Game::Update()
{
}

void Game::InputUpdate()
{
	_input_manager->Update();
	if (_input_manager->GetKeyState(KeyCode::Escape))
		_is_running = false;
}

void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_world_simulator->Draw(_camera);
}

void Game::RenderUI()
{
	_ui_manager->BeginFrame();

	_ui_manager->DrawGUI();

	_ui_manager->EndFrame();
}

void Game::OnDrawGUI(float delta_time)
{
	ImGui::Begin("Game");

	const auto maxHistory = _sand_time_tracker.GetMaxHistorySize();
	// FIXME: What
	ImGui::Text("Duration in ms of each sand update, over roughly %zu updates or (%.2f seconds)",
		maxHistory,
		maxHistory * (1 / _fixed_step_step_time));
	ImGui::Text("Sand Updates occur in fixed time steps of %.2f ms, roughly %.2f updates per second",
		_fixed_step_step_time,
		1000 / _fixed_step_step_time);

	// Start the table
	const float max_time = _sand_time_tracker.GetMaxTime();
	const float average = _sand_time_tracker.GetAverageTime();
	const float median = _sand_time_tracker.GetMedianTime();
	const float percentile_0_001 = _sand_time_tracker.GetPercentile_0_001();
	const float percentile_0_010 = _sand_time_tracker.GetPercentile_0_010();
	const float percentile_0_050 = _sand_time_tracker.GetPercentile_0_050();
	if (ImGui::BeginTable("Statistics", 2)) {
		// Create the headers
		ImGui::TableSetupColumn("Statistic");
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();

		// Row 1: Average Time
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Average Time");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f ms", average);

		// Row 2: Median Time
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Median Time");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f ms", median);

		// Row 3: 0.1 Low
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("0.1 Low");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f ms", percentile_0_001);

		// Row 4: 1 Low
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("1 Low");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f ms", percentile_0_010);

		// Row 5: 5 Low
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("5 Low");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f ms", percentile_0_050);

		// Row 6: Max Time
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Max Time");
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("%.2f ms", max_time);

		// End the table
		ImGui::EndTable();
	}

	if (ImPlot::BeginPlot("Sand Update Duration")) {
		float max_time_graph = 4;
		float min_time_graph = 0;
		if (max_time > max_time_graph) {
			max_time_graph = 8;
			min_time_graph = 2;
			if (max_time > max_time_graph) {
				max_time_graph = 16;
				min_time_graph = 4;
				if (max_time > max_time_graph) {
					max_time_graph = max_time + 1.0f;
					min_time_graph = max_time / 2.0f;
				}
			}
		}

		ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		ImPlot::SetupAxes("Sand Updates",
			"Time in Millisecond",
			ImPlotAxisFlags_Lock,
			ImPlotAxisFlags_Lock | ImPlotAxisFlags_RangeFit | ImPlotAxisFlags_Opposite);
		ImPlot::SetupAxesLimits(0, maxHistory, min_time_graph, max_time_graph, ImPlotCond_Always);
		ImPlot::PlotLine<float>("Sand Update Time", _sand_time_tracker.GetHistory().data(), maxHistory);

		// Average
		ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5f, 0.5f, 0.5f, 0.7f));
		ImPlot::PlotInfLines<float>("Average", &average, 1, ImPlotInfLinesFlags_Horizontal, 0);

		// Median
		ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5f, 0.8f, 0.5f, 0.7f));
		ImPlot::PlotInfLines<float>("Median", &median, 1, ImPlotInfLinesFlags_Horizontal, 0);

		// 5% Low
		ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.5f, 0.0f, 0.0f, 0.5f));
		ImPlot::PlotInfLines<float>("5 Low", &percentile_0_050, 1, ImPlotInfLinesFlags_Horizontal, 0);

		// 1% Low
		ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(0.75f, 0.0f, 0.0f, 0.5f));
		ImPlot::PlotInfLines<float>("1 Low", &percentile_0_010, 1, ImPlotInfLinesFlags_Horizontal, 0);

		// 0.1% Low
		ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImPlot::PlotInfLines<float>("0.1 Low", &percentile_0_001, 1, ImPlotInfLinesFlags_Horizontal, 0);

		ImPlot::EndPlot();
	}

	ImGui::End();
}
