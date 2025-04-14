#include "InputManager.h"

#include <imgui_impl_sdl3.h>

using namespace PixelProject::Input;

bool InputManager::IsValidKey(const KeyCode& key_code)
{
	const int code = static_cast<int>(key_code);
	if (code < 0 || code >= KEYCODE_MAX_VALUE)
		return false;
	return true;
}

bool InputManager::IsValidMouse(const MouseCode& mouse_code)
{
	const int code = static_cast<int>(mouse_code);
	if (code < 0 || code >= MOUSECODE_MAX_VALUE)
		return false;
	return true;
}

void InputManager::Update()
{
	ClearInputStates();

	if (SDL_PollEvent(nullptr) == 0) {
		// _is_held_with_listener
		for (auto& [key, event] : _is_held_with_listener) {
			event.motion.x = _mouse_x_pos;
			event.motion.y = _mouse_y_pos;
			_key_change_map[key]->Invoke(event, true);
		}

		for (auto& [mouse, event] : _is_mouse_held_with_listener) {
			event.motion.x = _mouse_x_pos;
			event.motion.y = _mouse_y_pos;
			_mouse_change_map[mouse]->Invoke(event, true);
		}

		return;
	}

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);

		_keyboard = SDL_GetKeyboardState(nullptr);
		_mouse = SDL_GetMouseState(&(_mouse_x_pos), &(this->_mouse_y_pos));

		auto keyCode = static_cast<KeyCode>(event.key.scancode);
		auto mouseCode = static_cast<MouseCode>(event.button.button);

		switch (event.type) {
		case SDL_EVENT_KEY_DOWN:
			_is_key_down[static_cast<int>(keyCode)] = true;
			_is_key_down_dirty = true;
			_is_key_held[static_cast<int>(keyCode)] = true;

			if (_key_change_map.contains(keyCode)) {
				_key_change_map[keyCode]->Invoke(event, true);
				_is_held_with_listener[keyCode] = event;
			}
			break;
		case SDL_EVENT_KEY_UP:
			_is_key_up[static_cast<int>(keyCode)] = true;
			_is_key_up_dirty = true;
			_is_key_held[static_cast<int>(keyCode)] = false;

			if (_key_change_map.contains(keyCode)) {
				_key_change_map[keyCode]->Invoke(event, false);
				_is_held_with_listener.erase(keyCode);
			}
			break;
		case SDL_EVENT_MOUSE_MOTION:
			_mouse_x_pos = event.motion.x;
			_mouse_y_pos = event.motion.y;
			_mouse_last_frame_pos = _current_mouse_pos;
			_current_mouse_pos = IVec2(_mouse_x_pos, _mouse_y_pos);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			_is_mouse_down[mouseCode] = true;
			_is_mouse_held[mouseCode] = true;
			_is_mouse_down_dirty = true;

			if (_mouse_change_map.contains(mouseCode)) {
				_mouse_change_map[mouseCode]->Invoke(event, true);
				_is_mouse_held_with_listener[mouseCode] = event;
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			_is_mouse_up[event.button.button] = true;
			_is_mouse_held[event.button.button] = false;
			_is_mouse_up_dirty = true;

			if (_mouse_change_map.contains(mouseCode)) {
				_mouse_change_map[mouseCode]->Invoke(event, false);
				_is_mouse_held_with_listener.erase(mouseCode);
			}
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			//TODO Does this work?
			_mouse_scroll.x = event.wheel.x;
			_mouse_scroll.y = event.wheel.y;
			break;
		case SDL_EVENT_QUIT:
			//TODO Should do something for this
			break;
		default:
			break;
		}
	}
}

#pragma region Mouse

bool InputManager::IsMouseHeld(const MouseCode mouse_code) const
{
	if (!IsValidMouse(mouse_code))
		return false;
	return _is_mouse_held[mouse_code];
}

bool InputManager::IsMouseDown(const MouseCode mouse_code) const
{
	if (!IsValidMouse(mouse_code))
		return false;
	return _is_mouse_down[mouse_code];
}

bool InputManager::IsMouseUp(const MouseCode mouse_code) const
{
	if (!IsValidMouse(mouse_code))
		return false;
	return _is_mouse_up[mouse_code];
}

bool InputManager::GetMouseState(const MouseCode mouse_code) const
{
	if (!IsValidMouse(mouse_code))
		return false;
	return _mouse & SDL_BUTTON_MASK(mouse_code);
}

#pragma endregion Mouse

#pragma region Keyboard

bool InputManager::IsKeyHeld(KeyCode key_code) const
{
	if (!IsValidKey(key_code))
		return false;
	return _is_key_held[static_cast<int>(key_code)];
}

bool InputManager::IsKeyDown(KeyCode key_code) const
{
	if (!IsValidKey(key_code))
		return false;
	return _is_key_down[static_cast<int>(key_code)];
}

bool InputManager::IsKeyUp(KeyCode key_code) const
{
	if (!IsValidKey(key_code))
		return false;
	return _is_key_up[static_cast<int>(key_code)];
}

bool InputManager::GetKeyState(KeyCode key_code) const
{
	if (!_keyboard)
		return false;
	// if (_keyboard[static_cast<int>(key_code)])
	// 	return true;
    return _is_key_down[static_cast<int>(key_code)] || _is_key_held[static_cast<int>(key_code)];
}

#pragma endregion Keyboard

#pragma region Listeners

void InputManager::AddKeyListener(const KeyCode key_code, const std::string& name, InputCallback callback)
{
	if (!_key_change_map.contains(key_code))
		_key_change_map[key_code] = new InputEventContainer();
	_key_change_map[key_code]->AddListener(name, callback);
}

void InputManager::AddKeyListeners(const std::vector<KeyCode>& key_codes, const std::string& name,
	InputCallback callback)
{
	for (auto& keyCode : key_codes)
		AddKeyListener(keyCode, name, callback);
}

void InputManager::RemoveKeyListener(const KeyCode key_code, const std::string& name)
{
	if (_key_change_map.contains(key_code))
		_key_change_map[key_code]->RemoveListener(name);

	// If there are no listeners for this key, remove it from the held map to improve performance
	if (_key_change_map[key_code]->GetListenerCount() == 0)
		_is_held_with_listener.erase(key_code);
}

void InputManager::RemoveKeyListeners(const std::vector<KeyCode>& key_codes, const std::string& name)
{
	for (auto& keyCode : key_codes)
		RemoveKeyListener(keyCode, name);
}

void InputManager::AddMouseListener(const MouseCode mouse_button,
	const std::string& name,
	const InputCallback& callback)
{
	if (!_mouse_change_map.contains(mouse_button))
		_mouse_change_map[mouse_button] = new InputEventContainer();
	_mouse_change_map[mouse_button]->AddListener(name, callback);
}

void InputManager::RemoveMouseListener(const MouseCode mouse_button, const std::string& name)
{
	if (_mouse_change_map.contains(mouse_button))
		_mouse_change_map[mouse_button]->RemoveListener(name);

	// If there are no listeners for this key, remove it from the held map to improve performance
	if (_mouse_change_map[mouse_button]->GetListenerCount() == 0)
		_is_mouse_held_with_listener.erase(mouse_button);
}

#pragma endregion Listeners

void InputManager::ClearInputStates()
{
	// Keyboard
	if (_is_key_down_dirty) {
		SDL_memset(_is_key_down, false, KEYCODE_MAX_VALUE * sizeof(bool));
		_is_key_down_dirty = false;
	}
	if (_is_key_up_dirty) {
		SDL_memset(_is_key_up, false, KEYCODE_MAX_VALUE * sizeof(bool));
		_is_key_up_dirty = false;
	}

	// Mouse
	if (_is_mouse_down_dirty) {
		SDL_memset(_is_mouse_down, false, MOUSECODE_MAX_VALUE * sizeof(bool));
		_is_mouse_down_dirty = false;
	}
	if (_is_mouse_up_dirty) {
		SDL_memset(_is_mouse_up, false, MOUSECODE_MAX_VALUE * sizeof(bool));
		_is_mouse_up_dirty = false;
	}
}
