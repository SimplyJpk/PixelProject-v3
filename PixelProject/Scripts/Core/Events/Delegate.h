#pragma once

#include <functional>
#include <vector>

template<typename... Args>
class Delegate
{
 public:
	using DelegateHandler = std::function<void(Args...)>;
	using DelegateHandlers = std::vector<DelegateHandler>;

	void AddListener(DelegateHandler handler)
	{
		_handlers.push_back(handler);
	}

	void RemoveListener(DelegateHandler handler)
	{
		for (auto it = _handlers.begin(); it != _handlers.end(); ++it) {
			// Check if the handler is the same without using ==
			if (it->target_type() == handler.target_type()
				&& it->template target<void>() == handler.template target<void>()) {
				_handlers.erase(it);
				break;
			}
		}
	}

	void Invoke(Args... args)
	{
		for (auto& handler : _handlers) {
			handler(args...);
		}
	}

 private:
	DelegateHandlers _handlers;

};
