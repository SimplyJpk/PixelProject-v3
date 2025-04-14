#pragma once
#include <random>
#include <glm/fwd.hpp>

class XorShift
{
 public:
	XorShift() : XorShift(std::random_device{}())
	{
	}

	explicit XorShift(const uint16_t seed) : _state(seed)
	{
		if (_state == 0) {
			_state = 1;
		}
	}

	glm::uint16_t operator()()
	{
		_state ^= _state << 13;
		_state ^= _state >> 9;
		_state ^= _state << 6;
		return _state;
	}

 private:
	uint16_t _state;
};