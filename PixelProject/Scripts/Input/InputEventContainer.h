#pragma once
#include <functional>

#include <SDL3/SDL_events.h>
#include <string>

namespace PixelProject::Input
{

	using InputCallback = std::function<void(SDL_Event&, bool)>;

	struct InputEvent
	{
		InputCallback callback;
		std::string name;

		InputEvent(InputCallback callback, std::string name) : callback(std::move(callback)), name(std::move(name))
		{
		}
	};

	class InputEventContainer
	{
	 public:
		InputEventContainer() = default;

		void AddListener(const std::string& name, const InputCallback& callback)
		{
			_callbacks.emplace_back(callback, name);
		}

		void RemoveListener(std::string name)
		{
			// Find callback in vector with same name
			if (const auto it = std::find_if(_callbacks.begin(), _callbacks.end(), [&name](const InputEvent& callback)
				{ return callback.name == name; }); it != _callbacks.end()) {
				_callbacks.erase(it);
			}

			// TODO : (James) Should probably manage this better, reducing the size of the vector is likely good enough for now given limited use.
			if (_callbacks.empty())
				_callbacks.shrink_to_fit();
		}

		void Invoke(SDL_Event& event, const bool pressed) const
		{
			for (auto& callbackWrapper : _callbacks) {
				callbackWrapper.callback(event, pressed);
			}
		}

		uint8_t GetListenerCount() const
		{
			return _callbacks.size();
		}

	 private:
		std::vector<InputEvent> _callbacks;
	};

}