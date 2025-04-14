#pragma once
#include "SDL3/SDL.h"

#include "Math/Math.h"
#include "InputKeyCodes.h"
#include "InputEventContainer.h"
#include "Macros/InlineReturn.h"

#include <unordered_map>

namespace PixelProject::Input
{
	class InputManager
	{
	 public:
		InputManager(const InputManager&) = delete;
		InputManager(InputManager&&) = delete;
		void operator=(const InputManager&) = delete;
		void operator=(InputManager&&) = delete;

		static InputManager* GetInstance()
		{
			static InputManager instance;
			return &instance;
		}

		void Update();

#pragma region Listeners

		void AddKeyListener(KeyCode key_code, const std::string& name, InputCallback callback);
		void AddKeyListeners(const std::vector<KeyCode>& key_codes, const std::string& name, InputCallback callback);

		void RemoveKeyListener(KeyCode key_code, const std::string& name);
		void RemoveKeyListeners(const std::vector<KeyCode>& key_codes, const std::string& name);

		void AddMouseListener(MouseCode mouse_button, const std::string& name, const InputCallback& callback);
		void RemoveMouseListener(MouseCode mouse_button, const std::string& name);

#pragma endregion Listeners

		// Mouse
		bool IsMouseHeld(MouseCode mouse_button) const;
		bool IsMouseDown(MouseCode mouse_code) const;
		bool IsMouseUp(MouseCode mouse_code) const;
		bool GetMouseState(MouseCode mouse_code) const;

		int GetMouseX() const
		INLINE_RETURN(_mouse_x_pos)
		int GetMouseY() const
		INLINE_RETURN(_mouse_y_pos)
		IVec2 GetMousePos() const
		INLINE_RETURN(_current_mouse_pos)
		IVec2 GetLastMousePos() const
		INLINE_RETURN(_mouse_last_frame_pos)

		// Keyboard
		bool IsKeyHeld(KeyCode key_code) const;
		bool IsKeyDown(KeyCode key_code) const;
		bool IsKeyUp(KeyCode key_code) const;
		bool GetKeyState(KeyCode key_code) const;

	 protected:
		static inline bool IsValidKey(const KeyCode& key_code);
		static inline bool IsValidMouse(const MouseCode& mouse_code);

		/// Checks if the key states need to be cleared, ie; _is_key_down
		void ClearInputStates();

		std::unordered_map<KeyCode, InputEventContainer*> _key_change_map;
		std::unordered_map<KeyCode, SDL_Event> _is_held_with_listener;

		std::unordered_map<MouseCode, InputEventContainer*> _mouse_change_map;
		std::unordered_map<MouseCode, SDL_Event> _is_mouse_held_with_listener;

	 private:
		InputManager() = default;
		~InputManager() = default;

		bool _keyboard = false;
		SDL_MouseButtonFlags _mouse = 0;

		float _mouse_x_pos = 0.0f, _mouse_y_pos = 0.0f;
		IVec2 _current_mouse_pos = IVec2::Zero();
		IVec2 _mouse_last_frame_pos = IVec2::Zero();
		IVec2 _mouse_scroll = IVec2::Zero();

		bool _is_key_held[KEYCODE_MAX_VALUE]{ false };
		bool _is_mouse_held[MOUSECODE_MAX_VALUE]{ false };

		bool _is_key_down[KEYCODE_MAX_VALUE]{ false };
		bool _is_key_down_dirty = false;
		bool _is_key_up[KEYCODE_MAX_VALUE]{ false };
		bool _is_key_up_dirty = false;

		bool _is_mouse_down[MOUSECODE_MAX_VALUE]{ false };
		bool _is_mouse_down_dirty = false;
		bool _is_mouse_up[MOUSECODE_MAX_VALUE]{ false };
		bool _is_mouse_up_dirty = false;
	};
}
