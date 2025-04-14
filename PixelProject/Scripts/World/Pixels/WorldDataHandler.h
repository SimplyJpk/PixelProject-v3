#pragma once

#include <unordered_map>

#include "PixelTypes.h"
#include "Rendering/Shaders/Shader.h"

class WorldDataHandler
{
 public:
	static WorldDataHandler* GetInstance();

	// TODO : (James) Utility functions
	// std::string GetPixelName(const Pixels::PixelType& pixel_type) const;
	// const BasePixel& GetPixelFromColour(const Uint32& colour) const;

	BasePixel* GetPixelFromIndex(const uint8_t index);
	BasePixel* GetPixelFromType(const Pixel::PixelType& pixel_type) const;

	const std::array<uint8_t, 8>& GetRandomPixelUpdateOrder(Pixel::PixelType pixel_type) const;
	Uint32 GetPixelColour(const Pixel::PixelType& pixel_type) const;

	void SetUniformData(Shader* shader);

	WorldDataHandler(const WorldDataHandler&) = delete;
	WorldDataHandler(WorldDataHandler&&) = delete;
	void operator=(const WorldDataHandler&) = delete;
	void operator=(WorldDataHandler&&) = delete;

 protected:
	WorldDataHandler();

 private:
	~WorldDataHandler() = default;

	// Main array holding pixel types in contiguous memory
	std::array<BasePixel, Pixel::TYPE_COUNT> _pixel_types;

	// utility structures for fast lookup and iteration
	std::unordered_map<Uint32, BasePixel*> _pixel_colour_map;
	std::unordered_map<Pixel::PixelType, BasePixel*> _pixel_type_map;
};
