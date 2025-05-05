#pragma once

#include <cstdint>
#include <format>

#include "Math/Vec2/IVec2.h"
#include "Constants/DebugConstants.h"

namespace World
{
	// Target Size ([15x9~]1080p pixels, 1920x1152), v1 was 8x5 (1024x640)
	constexpr short SIZE_X = 4;
	constexpr short SIZE_Y = 3;
}

namespace Chunk
{
	constexpr short ARRAY_ENTRY_INDEX = 0;
	constexpr short INCREMENT_POSITIVE = 1;
	constexpr short INCREMENT_NEGATIVE = -1;

	constexpr short SIZE_X = 128;
	constexpr short HALF_X = SIZE_X / 2;
	constexpr short ARRAY_MAX_X = SIZE_X - 1;

	constexpr short SIZE_Y = 128;
	constexpr short HALF_Y = SIZE_Y / 2;
	constexpr short ARRAY_MAX_Y = SIZE_Y - 1;

	constexpr short TOTAL_SIZE = SIZE_X * SIZE_Y;
	constexpr short ARRAY_MAX_INDEX = TOTAL_SIZE - 1;
	constexpr short MAX_OVERFLOW_SIZE = TOTAL_SIZE * 1.5;

	constexpr short CORNER_NORTH_WEST = 0;
	constexpr short CORNER_NORTH_EAST = SIZE_X - 1;
	constexpr short CORNER_SOUTH_EAST = TOTAL_SIZE - 1;
	constexpr short CORNER_SOUTH_WEST = TOTAL_SIZE - SIZE_X;

	constexpr uint8_t NUM_DIRECTIONS = 8;
	enum class WorldDir : uint8_t
	{
		North = 0,
		NorthEast = 1,
		East = 2,
		SouthEast = 3,
		South = 4,
		SouthWest = 5,
		West = 6,
		NorthWest = 7,
		Undefined = NUM_DIRECTIONS
	};

	constexpr IVec2 WORLD_DIR_DIRECTION_VECTORS[] = {
		IVec2{0, -1},  // North
		IVec2{1, -1},  // NorthEast
		IVec2{1, 0},   // East
		IVec2{1, 1},   // SouthEast
		IVec2{0, 1},   // South
		IVec2{-1, 1},  // SouthWest
		IVec2{-1, 0},  // West
		IVec2{-1, -1}, // NorthWest
		IVec2{0, 0}	   // Undefined
	};

	constexpr int8_t WORLD_DIR_Y_DIRECTIONS[] = {
		/* WorldDir::North     */ -1,
		/* WorldDir::NorthEast */ -1,
		/* WorldDir::East      */ 0,
		/* WorldDir::SouthEast */ 1,
		/* WorldDir::South     */ 1,
		/* WorldDir::SouthWest */ 1,
		/* WorldDir::West      */ 0,
		/* WorldDir::NorthWest */ -1,
		/* WorldDir::Undefined */ 0};

	constexpr const char *WORLD_DIR_NAMES[] = {
		"North",	 // North
		"NorthEast", // NorthEast
		"East",		 // East
		"SouthEast", // SouthEast
		"South",	 // South
		"SouthWest", // SouthWest
		"West",		 // West
		"NorthWest", // NorthWest
		"Undefined"	 // Undefined
	};

	constexpr const char *GetWorldDirName(const WorldDir world_dir)
	{
		return WORLD_DIR_NAMES[static_cast<uint8_t>(world_dir)];
	}

	constexpr IVec2 GetDirVector(const WorldDir world_dir)
	{
		return WORLD_DIR_DIRECTION_VECTORS[static_cast<uint8_t>(world_dir)];
	}

	constexpr int8_t GetYDirFromWorldDir(const WorldDir world_dir)
	{
		return WORLD_DIR_Y_DIRECTIONS[static_cast<uint8_t>(world_dir)];
	}

	constexpr uint8_t GetDirectionIndex(const WorldDir direction)
	{
		return static_cast<uint8_t>(direction);
	}

	enum ChunkPixelBorderMask : uint8_t
	{
		/* 1  0001 */ North = 1 << 0,
		/* 2  0010 */ East = 1 << 1,
		/* 4  0100 */ South = 1 << 2,
		/* 8  1000 */ West = 1 << 3,
		/* 3  0011 */ NorthEast = North | East,
		/* 6  0110 */ SouthEast = South | East,
		/* 12 1100 */ SouthWest = South | West,
		/* 9  1001 */ NorthWest = North | West,
		/* 0  0000 */ Undefined = 0
	};

	constexpr uint8_t BORDER_SOUTHERLY_MASK =
		ChunkPixelBorderMask::South | ChunkPixelBorderMask::SouthEast | ChunkPixelBorderMask::SouthWest;

	constexpr ChunkPixelBorderMask WORLD_DIR_TO_BORDER_MASK[] = {
		ChunkPixelBorderMask::North,	 // North
		ChunkPixelBorderMask::NorthEast, // NorthEast
		ChunkPixelBorderMask::East,		 // East
		ChunkPixelBorderMask::SouthEast, // SouthEast
		ChunkPixelBorderMask::South,	 // South
		ChunkPixelBorderMask::SouthWest, // SouthWest
		ChunkPixelBorderMask::West,		 // West
		ChunkPixelBorderMask::NorthWest, // NorthWest
		ChunkPixelBorderMask::Undefined	 // Undefined
	};

	constexpr ChunkPixelBorderMask WorldDirToChunkBorderMask(const WorldDir world_dir)
	{
		return WORLD_DIR_TO_BORDER_MASK[static_cast<uint8_t>(world_dir)];
	}

	constexpr bool IsWorldDirSoutherly(const WorldDir world_dir)
	{
		const auto chunk_border_mask = WorldDirToChunkBorderMask(world_dir);
		return (chunk_border_mask & BORDER_SOUTHERLY_MASK) != 0;
	}

	constexpr WorldDir BORDER_MASK_TO_WORLD_DIR[] = {
		WorldDir::Undefined,
		WorldDir::North,	 // North
		WorldDir::East,		 // East
		WorldDir::NorthEast, // NorthEast
		WorldDir::South,	 // South
		WorldDir::Undefined,
		WorldDir::SouthEast, // SouthEast
		WorldDir::Undefined,
		WorldDir::West,		 // West
		WorldDir::NorthWest, // NorthWest
		WorldDir::Undefined,
		WorldDir::Undefined,
		WorldDir::SouthWest, // SouthWest
		WorldDir::Undefined,
		WorldDir::Undefined,
		WorldDir::Undefined};

	constexpr WorldDir ChunkBorderMaskToWorldDir(const ChunkPixelBorderMask chunk_border_mask)
	{
		return BORDER_MASK_TO_WORLD_DIR[chunk_border_mask];
	}

#pragma region Static Assertions
	// A bunch of easy assertions to make sure very broken values aren't used

	static_assert(ARRAY_ENTRY_INDEX == 0, "ARRAY_ENTRY_INDEX must be 0");
	static_assert(INCREMENT_POSITIVE >= 1, "INCREMENT_POSITIVE must be greater than 0");
	static_assert(INCREMENT_NEGATIVE <= -1, "INCREMENT_NEGATIVE must be less than 0");

	static_assert(SIZE_X >= 1, "SIZE_X must be greater than 0");
	static_assert(SIZE_Y >= 1, "SIZE_Y must be greater than 0");

	static_assert(SIZE_X % 2 == 0, "SIZE_X must be even");
	static_assert(SIZE_Y % 2 == 0, "SIZE_Y must be even");

	static_assert(ARRAY_MAX_X == SIZE_X - 1, "ARRAY_MAX_X must be SIZE_X - 1");
	static_assert(ARRAY_MAX_Y == SIZE_Y - 1, "ARRAY_MAX_Y must be SIZE_Y - 1");

	static_assert(TOTAL_SIZE == SIZE_X * SIZE_Y, "TOTAL_SIZE must be SIZE_X * SIZE_Y");
	static_assert(ARRAY_MAX_INDEX == TOTAL_SIZE - 1, "ARRAY_MAX_INDEX must be TOTAL_SIZE - 1");

	static_assert(WorldDirToChunkBorderMask(WorldDir::North) == ChunkPixelBorderMask::North,
				  "WorldDirToChunkBorderMask(WorldDir::North) must be ChunkPixelBorderMask::North");
	static_assert(WorldDirToChunkBorderMask(WorldDir::South) == ChunkPixelBorderMask::South,
				  "WorldDirToChunkBorderMask(WorldDir::South) must be ChunkPixelBorderMask::South");

#pragma endregion Static Assertions

}

namespace Pixel
{
	constexpr int8_t MAX_NAME_LENGTH = 16;
	constexpr char NAME_UNDEFINED[MAX_NAME_LENGTH] = "N/A";

	// Max colours a pixel can reference
	constexpr int8_t MAX_COLOUR_COUNT = 8;
	// Max pixel direction orders a PixelType can have
	constexpr int8_t MAX_UPDATE_ORDER_COUNT = 4;

	constexpr int8_t MAX_NEW_VALUE_COUNT = 4;

	enum class PixelType : int8_t
	{
		UNDEF = -1,
		Space = 0,
		Dirt = 1,
		Sand = 2,
		Water = 3,
		Wood = 4,
		Fire = 5,
		Oil = 6,
		Acid = 7,
		Gold = 8,
		Steam = 9,
		COUNT,
	};

	// TODO : (James) Unsure if I like this as it means index has to be uniform across all PixelTypes
	constexpr int8_t TYPE_COUNT = static_cast<int8_t>(PixelType::COUNT);

	constexpr int16_t INDEX_DIRECTION_CHANGE[] = {
		/* N  */ -Chunk::SIZE_X,
		/* NE */ -Chunk::SIZE_X + 1,
		/* E  */ 1,
		/* SE */ Chunk::SIZE_X + 1,
		/* S  */ Chunk::SIZE_X,
		/* SW */ Chunk::SIZE_X - 1,
		/* W  */ -1,
		/* NW */ -Chunk::SIZE_X - 1};

	// Logic used to update Pixels.
	enum LogicResult : int8_t
	{
		None = 0,
		FailedUpdate = 1 << 0,
		SuccessUpdate = 1 << 1,
		FirstReturnPixel = 1 << 2,
		SecondReturnPixel = 1 << 3,
		DualReturnPixel = 1 << 4,
		NoChange = 1 << 5
	};

	constexpr int16_t INDEX_ADJUSTMENTS[] = {
		Chunk::TOTAL_SIZE - Chunk::SIZE_X,		// North
		Chunk::TOTAL_SIZE - Chunk::SIZE_X - 1,	// NorthEast
		-Chunk::SIZE_X + 1,						// East
		-Chunk::TOTAL_SIZE + Chunk::SIZE_X + 1, // SouthEast
		-Chunk::TOTAL_SIZE + Chunk::SIZE_X,		// South
		-Chunk::TOTAL_SIZE + Chunk::SIZE_X - 1, // SouthWest
		Chunk::SIZE_X - 1,						// West
		Chunk::TOTAL_SIZE - Chunk::SIZE_X + 1	// NorthWest
	};

	constexpr bool WillTraverseToNewChunk(
		uint16_t currentIndex,
		uint8_t direction,
		uint16_t &outNewIndex,
		uint8_t pixelBorderMask)
	{
		outNewIndex = currentIndex + INDEX_DIRECTION_CHANGE[direction];
		const auto isTraversingChunk = (pixelBorderMask & Chunk::WORLD_DIR_TO_BORDER_MASK[direction]) != 0;

		if (isTraversingChunk)
		{
			outNewIndex += INDEX_ADJUSTMENTS[direction];
		}

		// Ensure the new index is within valid bounds
		if (outNewIndex >= Chunk::TOTAL_SIZE)
		{
			outNewIndex %= Chunk::TOTAL_SIZE;
		}

		return isTraversingChunk;
	}
}