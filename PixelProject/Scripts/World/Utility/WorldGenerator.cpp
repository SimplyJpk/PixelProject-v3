#include "WorldGenerator.h"

#include "World/Pixels/WorldDataHandler.h"
#include "World/Pixels/Base/BasePixel.h"

WorldGenerator::WorldGenerator()
{
	if (noise == nullptr)
		GenerateNoise();
}

void WorldGenerator::GenerateNoise()
{
	// Create a new Simplex noise generator
	auto simplex = FastNoise::New<FastNoise::Simplex>();

	// Create a new Fractal FBm generator
	auto fractal = FastNoise::New<FastNoise::FractalFBm>();
	fractal->SetSource(simplex);
	fractal->SetOctaveCount(4);
	fractal->SetLacunarity(0.5f);
	fractal->SetGain(0.1f);

	// Set the noise generator to the fractal generator
	noise = fractal;
}

bool WorldGenerator::GenerateChunk(WorldChunk* world_chunk)
{
	if (noise == nullptr)
		GenerateNoise();

	static constexpr short CHUNK_SIZE_X = Chunk::SIZE_X;
	static constexpr short CHUNK_SIZE_Y = Chunk::SIZE_Y;

	const auto& worldPos = world_chunk->position;
	std::vector<float> noiseSet(CHUNK_SIZE_X * CHUNK_SIZE_Y);
	noise->GenUniformGrid2D(noiseSet.data(), worldPos.x, worldPos.y, CHUNK_SIZE_X, CHUNK_SIZE_Y, 0.1f, 666);
	short index = 0;

	// INFO_LOG("[WorldGenerator] Generating Chunk at: {0}, {1}", worldPos.x, worldPos.y);

	const auto& worldData = WorldDataHandler::GetInstance();
	auto& pixelData = world_chunk->pixel_data;

	for (short x = 0; x < CHUNK_SIZE_X; x++) {
		for (short y = 0; y < CHUNK_SIZE_Y; y++) {
			const auto noiseValue = noiseSet[index];

			auto pixelType = Pixel::PixelType::Space;
			if (noiseValue <= -0.75f)
				pixelType = Pixel::PixelType::Dirt;
			else if (noiseValue < -0.7f)
				pixelType = Pixel::PixelType::Water;
			else if (noiseValue < -0.65f)
				pixelType = Pixel::PixelType::Sand;

			auto const& pixel = worldData->GetPixelFromType(pixelType);

			// Set Index in Data
			pixelData[index] = pixel->GetNewPixel();

			index++;
		}
	}

	return true;
}

