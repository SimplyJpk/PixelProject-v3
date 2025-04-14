#include "FlyCamera.h"
#include "Utility/Console.h"

FlyCamera::FlyCamera()
{
	const auto input = InputManager::GetInstance();

	const auto handleKeyFunction =
		std::bind(&FlyCamera::HandleKeyMovement, this, std::placeholders::_1, std::placeholders::_2);
	for (auto keyMovement : KEY_MOVEMENT) {
		input->AddKeyListener(keyMovement.GetKeyAsKeyCode(), std::to_string(keyMovement.GetHash()), handleKeyFunction);
		_key_pressed[keyMovement.GetKeyAsKeyCode()] = false;
	}
	// same as above but for _zoom
	for (auto keyZoom : KEY_ZOOM) {
		input->AddKeyListener(keyZoom.GetKeyAsKeyCode(), std::to_string(keyZoom.GetHash()), handleKeyFunction);
		_key_pressed[keyZoom.GetKeyAsKeyCode()] = false;
	}

	const auto shiftKeyFunc = std::bind(&FlyCamera::HandleKeyShift, this, std::placeholders::_1, std::placeholders::_2);
	for (auto shiftKey : SHIFT_KEYS) {
		input->AddKeyListener(shiftKey
			, SHIFT_KEY_INPUT_KEY, shiftKeyFunc);
		_key_pressed[shiftKey] = false;
	}
}

FlyCamera::~FlyCamera()
{
	auto input = InputManager::GetInstance();

	for (auto keyMovement : KEY_MOVEMENT) {
		input->RemoveKeyListener(keyMovement.GetKeyAsKeyCode(), std::to_string(keyMovement.GetHash()));
	}
	for (auto shiftKey : SHIFT_KEYS) {
		input->RemoveKeyListener(shiftKey, SHIFT_KEY_INPUT_KEY);
	}
	// zoom
	for (auto keyZoom : KEY_ZOOM) {
		input->RemoveKeyListener(keyZoom.GetKeyAsKeyCode(), std::to_string(keyZoom.GetHash()));
	}
}

void FlyCamera::Update(float deltaTime)
{
	Camera::Update(deltaTime);

	glm::vec2 dir = glm::vec2();
	for (auto keyMovement : KEY_MOVEMENT) {
		if (_key_pressed[keyMovement.GetKeyAsKeyCode()]) {
			dir += keyMovement.data;
		}
	}
	// zoom KEY_ZOOM
	for (auto keyZoom : KEY_ZOOM) {
		if (_key_pressed[keyZoom.GetKeyAsKeyCode()]) {
			_zoom += keyZoom.data * (_default_speed / 256) * deltaTime * _zoom;
		}
	}
	// Get length of direction vector
	if (const float length = glm::length(dir); length > 0) {
		dir = glm::normalize(dir);
		const glm::vec3 movement = glm::vec3(dir.x, 0, dir.y);

		const auto shiftMult = _shift_held ? _shift_multiplier : 1.0f;
		_position += movement * _default_speed * shiftMult * deltaTime;

		SetPosition(_position);
	}
}


void FlyCamera::HandleKeyMovement(SDL_Event& event, const bool is_pressed)
{
	_key_pressed[static_cast<KeyCode>(event.key.scancode)] = is_pressed;
}

void FlyCamera::HandleKeyShift(SDL_Event& event, const bool is_pressed)
{
	_shift_held = is_pressed;
}
