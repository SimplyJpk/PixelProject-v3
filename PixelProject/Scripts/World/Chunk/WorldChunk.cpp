#include "WorldChunk.h"

WorldChunk::WorldChunk(const IVec2& position)
{
	this->position = position;
}

std::string WorldChunk::FilePath()
{
	return "worldData_x" + std::to_string(position.x) + "-y" + std::to_string(position.y);
}

void WorldChunk::Save(cereal::BinaryOutputArchive out_archive)
{
	out_archive(CEREAL_NVP(position));

	// std::vector<Uint32> pixelData(Chunk::CHUNK_TOTAL_SIZE);
	// std::copy_n(pixel_data, Chunk::CHUNK_TOTAL_SIZE, pixelData.begin());

	out_archive(cereal::make_nvp("ChunkPixels", pixel_data));
}

void WorldChunk::Load(cereal::BinaryInputArchive in_archive)
{
	in_archive(position);
	// std::vector<Uint32> pixelData(Chunk::CHUNK_TOTAL_SIZE);
	in_archive(cereal::make_nvp("ChunkPixels", pixel_data));

	// std::ranges::copy(pixelData, pixel_data);
}