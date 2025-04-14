#pragma once

#include <FastNoise/FastNoise.h>
#include "World/Chunk/WorldChunk.h"

class WorldGenerator
{
 public:
	WorldGenerator();

	static inline FastNoise::SmartNode<FastNoise::FractalFBm> noise = nullptr;

	static bool GenerateChunk(WorldChunk* world_chunk);

	static void GenerateNoise();

};
