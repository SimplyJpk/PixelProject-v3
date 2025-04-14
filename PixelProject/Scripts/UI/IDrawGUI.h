#pragma once
#include "UIManager.h"

class IDrawGUI
{
 public:
	IDrawGUI();
	virtual ~IDrawGUI();

	// Virtual Method, we add ourself to the UIManager's list of callbacks
	virtual void OnDrawGUI(float delta_time) = 0;
 protected:

};
