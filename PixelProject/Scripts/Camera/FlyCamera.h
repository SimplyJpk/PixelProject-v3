#pragma once
#include "Camera.h"
#include "Input/InputManager.h"
#include "Input/InputContainer.h"

using namespace PixelProject::Input;

class FlyCamera : public Camera
{
 public:
	FlyCamera();
	~FlyCamera();

	void Update(float deltaTime) override;

 private:
	float _default_speed = 0.25f;
	float _shift_multiplier = 5.0f;

	float _move_speed = _default_speed;

	void HandleKeyMovement(SDL_Event& event, bool is_pressed);
	void HandleKeyShift(SDL_Event& event, bool is_pressed);

	const std::string MOVE_KEY_INPUT_KEY = "FlyCamera::Move";
	const std::vector<InputContainer<glm::vec2>> KEY_MOVEMENT = {
		{ MOVE_KEY_INPUT_KEY, KeyCode::W, { 0, 1 }},
		{ MOVE_KEY_INPUT_KEY, KeyCode::S, { 0, -1 }},
		{ MOVE_KEY_INPUT_KEY, KeyCode::A, { -1, 0 }},
		{ MOVE_KEY_INPUT_KEY, KeyCode::D, { 1, 0 }},
	};
	// container for zoom, Q/E for zoom in/out, single value -1 or 1
	const std::vector<InputContainer<float>> KEY_ZOOM = {
		{ MOVE_KEY_INPUT_KEY, KeyCode::Q, -1 },
		{ MOVE_KEY_INPUT_KEY, KeyCode::E, 1 },
	};
	std::unordered_map<KeyCode, bool> _key_pressed;

	const std::string SHIFT_KEY_INPUT_KEY = "FlyCamera::Shift";
	const std::vector<KeyCode> SHIFT_KEYS = { KeyCode::LShift };
	bool _shift_held = false;

};
