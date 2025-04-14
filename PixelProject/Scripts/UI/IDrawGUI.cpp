#include "IDrawGUI.h"

IDrawGUI::IDrawGUI()
{
	UIManager::GetInstance()->AddGUIDrawCallback(std::bind(&IDrawGUI::OnDrawGUI, this, std::placeholders::_1));
}

IDrawGUI::~IDrawGUI()
{
	UIManager::GetInstance()->RemoveGUIDrawCallback(std::bind(&IDrawGUI::OnDrawGUI, this, std::placeholders::_1));
}
