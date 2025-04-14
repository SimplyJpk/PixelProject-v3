#pragma once

#include "Math/Math.h"
#include "Interfaces/ISerialize.h"

#include "ChunkSupport.h"
#include "World/WorldConstants.h"

class WorldChunk final : public ISerialize
{
 public:
	IVec2 position;
	WorldChunk* neighbour_chunks[Chunk::NUM_DIRECTIONS] = { nullptr };
	time_t last_update_time = 0;

	Uint32 pixel_data[Chunk::TOTAL_SIZE] = { 0 };
	// TODO : (James) This may be pushing it, maybe keep this somewhere else
	uint8_t last_updated[Chunk::TOTAL_SIZE] = { 0 };

	explicit WorldChunk(const IVec2& position);

	// Inherited via ISerializable
	SaveTypes SaveType() override
	INLINE_RETURN(SaveTypes::Binary)
	std::string FilePath() override;
	void Save(cereal::BinaryOutputArchive out_archive) override;
	void Load(cereal::BinaryInputArchive in_archive) override;

};