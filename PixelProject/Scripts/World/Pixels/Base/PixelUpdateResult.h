#pragma once

#include <SDL3/SDL_stdinc.h>
#include "World/WorldConstants.h"
#include <array>

/**
 Container Type\n
 [0] = E_LogicResults\n
 [1] = NeighbourType\n
 [2] = NewPixel = E_PixelType (Local)\n
 [3] = NewPixel = E_PixelType (Neighbour)\n
 [4] = E_ChunkDirection (Pixel Update Direction)
 
 Mess of a container class that is updates during pixel updates. This was made to pass only 1 reference around instead of 3 value types and 1 reference.
 This proves to be faster, if only because most updates only require setting the first value of the container (Fail/Pass) before continuing.
 Readability however, gets thrown out the window.
 */
class PixelUpdateResult
{
	// Typedef Pixel::LogicResult to LogicResult
	typedef Pixel::LogicResult LogicResult;
	typedef Pixel::PixelType PixelType;
	typedef Chunk::WorldDir WorldDir;

	enum ContainerIndex
	{
		Results = 0,
		NeighbourPixelType = 1,
		NewSelfType = 2,
		NewNeighbourType = 3,
		DirectionIndex = 4
	};

 private:
	std::array<uint8_t, 5> _results = { 0 };

 public:

	// Result
	constexpr LogicResult Result() const noexcept
	{
		return static_cast<LogicResult>(_results[static_cast<uint8_t>(ContainerIndex::Results)]);
	}
	constexpr void SetResult(const LogicResult result) noexcept
	{
		_results[static_cast<uint8_t>(ContainerIndex::Results)] = static_cast<uint8_t>(result);
	}

	// Neighbour
	constexpr PixelType NeighbourType() const noexcept
	{
		return static_cast<PixelType>(_results[ContainerIndex::NeighbourPixelType]);
	}
	constexpr void SetNeighbour(const PixelType type) noexcept
	{
		_results[ContainerIndex::NeighbourPixelType] = static_cast<int8_t>(type);
	}

	// Local NewPixel
	constexpr PixelType NewLocal() const noexcept
	{
		return static_cast<PixelType>(_results[static_cast<uint8_t>(ContainerIndex::NewSelfType)]);
	}
	constexpr void SetLocal(const PixelType type) noexcept
	{
		_results[static_cast<uint8_t>(ContainerIndex::NewSelfType)] = static_cast<uint8_t>(type);
		_results[static_cast<uint8_t>(ContainerIndex::Results)] = static_cast<uint8_t>(LogicResult::FirstReturnPixel);
	}

	// Neighbour NewPixel
	PixelType NewNeighbour() const noexcept
	{
		return static_cast<PixelType>(_results[ContainerIndex::NewNeighbourType]);
	}
	void SetNewNeighbour(const PixelType type) noexcept
	{
		_results[ContainerIndex::NewNeighbourType] = static_cast<int8_t>(type);
		_results[ContainerIndex::Results] = static_cast<int8_t>(LogicResult::SecondReturnPixel);
	}

	// Direction
	constexpr WorldDir Dir() const noexcept
	{
		return static_cast<WorldDir>(_results[static_cast<uint8_t>(ContainerIndex::DirectionIndex)]);
	}
	constexpr void SetDirection(const WorldDir dir) noexcept
	{
		_results[static_cast<uint8_t>(ContainerIndex::DirectionIndex)] = static_cast<uint8_t>(dir);
	}

	// Set Local & Neighbour Type
	void SetLocalAndNeighbour(const PixelType local, const PixelType neighbour) noexcept
	{
		_results[ContainerIndex::NewSelfType] = static_cast<int8_t>(local);
		_results[ContainerIndex::NewNeighbourType] = static_cast<int8_t>(neighbour);
		_results[ContainerIndex::Results] = static_cast<int8_t>(LogicResult::DualReturnPixel);
	}

	// Simple one liner
	constexpr void Fail() noexcept
	{
		_results[static_cast<uint8_t>(ContainerIndex::Results)] = static_cast<uint8_t>(LogicResult::FailedUpdate);
	}
	constexpr void Pass() noexcept
	{
		_results[static_cast<uint8_t>(ContainerIndex::Results)] = static_cast<uint8_t>(LogicResult::SuccessUpdate);
	}
};