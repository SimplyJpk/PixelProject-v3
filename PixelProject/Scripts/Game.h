#pragma once

#include <SDL3/SDL.h>

#include "UI/UIManager.h"
#include "Input/InputManager.h"
#include "Camera/FlyCamera.h"

#include "Config/GameSettings.h"
#include "UI/IDrawGUI.h"
#include "UI/Paint/PaintSelector.h"
#include "Utility/Time/TimeTracker.h"
#include "World/WorldSimulator.h"

using namespace PixelProject::Input;

class Game : IDrawGUI
{
 public:
	~Game() = default;

	Game(SDL_GLContext* gl_context, SDL_Window* gl_window, std::shared_ptr<GameSettings>& settings);
	bool Initialize();

	/// @brief Runs the game loop, this won't return until the application is closed
	void Run();

 protected:
	void InputUpdate();

	void FixedUpdate();
	void Update();

	void Render();
	void RenderUI();

 protected:
	SDL_GLContext* _context;
	SDL_Window* _window;

	UIManager* _ui_manager;
	InputManager* _input_manager;
	PaintSelector* _paint_selector;
	FlyCamera* _camera;

	WorldSimulator* _world_simulator;

	double _delta_time = 0.0;
	double _fixed_time = 0.0;

	double _minimum_delta_time = 1000.0 / 60.0;

	double _fixed_step_step_time = 1000.0 / 60.0;
	uint8_t _fixed_step_max_steps = 10;

 private:
	Game() = default;
	bool _is_running = true;

	std::shared_ptr<GameSettings> _settings;
	TimeTracker<TimeFormat::Milli> _sand_time_tracker = TimeTracker<TimeFormat::Milli>(30);

	virtual void OnDrawGUI(float delta_time) override;
};
