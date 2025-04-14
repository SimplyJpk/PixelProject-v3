#pragma once

#include <string>

#include "Transform.h"

class GameObject
{
 public:
	std::string name;
	std::string tag;
	int layer;
	Transform transform;

};
