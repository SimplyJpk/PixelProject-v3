#pragma once
#include <string>

#include "InputKeyCodes.h"
#include "InputEventContainer.h"

#include "Macros/InlineReturn.h"

using namespace PixelProject::Input;

template<class T>
class InputContainer
{
 public:
	std::string name;
	T data = T();
	int key;

	size_t GetHash() const
	INLINE_RETURN(_hash)

	InputContainer(std::string name, int key, T data)
	{
		this->name = name;
		this->key = key;
		this->data = data;

		_hash = std::hash<std::string>()(name + std::to_string(key));
	}

	InputContainer(std::string name, KeyCode key, T data) : InputContainer(name, static_cast<int>(key), data)
	{
	}
	InputContainer(std::string name, MouseCode mouse, T data) : InputContainer(name, static_cast<int>(mouse), data)
	{
	}

	KeyCode GetKeyAsKeyCode()
	INLINE_RETURN(static_cast<KeyCode>(key))
	MouseCode GetAsMouseCode()
	INLINE_RETURN(static_cast<MouseCode>(key))

 protected:
	size_t _hash;

};