#pragma once

#include <SDL3/SDL.h>
#include "Core/Events/Delegate.h"

#include "Debug/DebugStats.h"
#include "Utility/XorShift.h"

class UIManager
{
 public:
	static UIManager* GetInstance()
	{
		static UIManager instance;
		return &instance;
	}

	void Init(SDL_Window* window, SDL_GLContext* glContext);

	void DrawGUI();
	void AddGUIDrawCallback(Delegate<float>::DelegateHandler callback)
	{
		_onGUIDraw.AddListener(callback);
	}
	void RemoveGUIDrawCallback(Delegate<float>::DelegateHandler callback)
	{
		_onGUIDraw.RemoveListener(callback);
	}

	void BeginFrame() const;
	void EndFrame();

 protected:
	XorShift _rng;

	SDL_Window* _window = nullptr;
	Delegate<float> _onGUIDraw;

	DebugStats* _debug_stats;

 private:
	UIManager() = default;
	~UIManager();

};
