#include "WorldDataHandler.h"

#include "Utility/Console.h"

#include <format>

#include "PixelTypes.h"
#include "World/PixelMask.h"

WorldDataHandler *WorldDataHandler::GetInstance()
{
	static WorldDataHandler instance;
	return &instance;
}

WorldDataHandler::WorldDataHandler()
{
	_pixel_types = {
		SpacePixel(),
		DirtPixel(),
		SandPixel(),
		WaterPixel(),
		WoodPixel(),
		FirePixel(),
	};

	int index = 0;
	for (auto &pixel : _pixel_types)
	{
		// pixel.pixel_index = index++; // Handled by constructor of BasePixel now
		for (auto i = 0; i < pixel.colour_count; ++i)
		{
			if (_pixel_colour_map.contains(pixel.type_colours[i]))
				DEBUG_WARNING_LOG("[WorldDataHandler] Duplicate pixel colour {} found for pixel type {}.",
								  pixel.type_colours[i],
								  static_cast<uint32_t>(pixel.pixel_type));
			_pixel_colour_map[pixel.type_colours[i]] = &pixel;
		}
		_pixel_type_map[pixel.pixel_type] = &pixel;
	}
}

Uint32 WorldDataHandler::GetPixelColour(const Pixel::PixelType &pixel_type) const
{
	return _pixel_type_map.at(pixel_type)->GetRandomColour();
}

const std::array<uint8_t, 8> &WorldDataHandler::GetRandomPixelUpdateOrder(Pixel::PixelType pixel_type) const
{
	return _pixel_type_map.at(pixel_type)->GetPixelUpdateOrder();
}

BasePixel *WorldDataHandler::GetPixelFromIndex(const uint8_t index)
{
	return &_pixel_types[index];
}

BasePixel *WorldDataHandler::GetPixelFromType(const Pixel::PixelType &pixel_type) const
{
	return _pixel_type_map.at(pixel_type);
}

void WorldDataHandler::SetUniformData(Shader *shader)
{
	shader->UseProgram();
	const auto programID = shader->GetProgramID();
	GLuint loc;
	for (int i = 0; i < TYPE_COUNT; i++)
	{
		auto pixel = _pixel_types[i];
		std::string locationString = std::format("u_Pixels[{}].colour_count", static_cast<int>(pixel.pixel_index));
		loc = shader->GetUniformLocation(locationString);
		const auto colourCount = static_cast<GLuint>(pixel.colour_count);
		glProgramUniform1ui(programID, loc, colourCount);
		float pixelColour[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		for (short colourIndex = 0; colourIndex < pixel.colour_count; colourIndex++)
		{
			if (colourIndex < pixel.colour_count)
			{
				const auto colourLoc = shader->GetUniformLocation(
					std::format("u_Pixels[{}].colours[{}]",
								pixel.pixel_index,
								static_cast<int>(colourIndex)));
				pixel.GetIndexAs4FColour(colourIndex, pixelColour);
				glProgramUniform4fv(programID, colourLoc, 4, pixelColour);
			}
		}
	}

	// Set MaskData
	loc = shader->GetUniformLocation("u_PixelMask.index");
	glProgramUniform1ui(programID, loc, PixelMask::Index::GPU_SHADER_BITS);
	loc = shader->GetUniformLocation("u_PixelMask.lifetime");
	glProgramUniform1ui(programID, loc, PixelMask::Lifetime::GPU_SHADER_BITS);
	loc = shader->GetUniformLocation("u_PixelMask.behaviour");
	glProgramUniform1ui(programID, loc, PixelMask::Behaviour::GPU_SHADER_BITS);
	loc = shader->GetUniformLocation("u_PixelMask.light");
	glProgramUniform1ui(programID, loc, PixelMask::Light::GPU_SHADER_BITS);

	// Set MaskData
	loc = shader->GetUniformLocation("u_PixelBitOffset.index");
	glProgramUniform1ui(programID, loc, PixelMask::Index::DEPTH);
	loc = shader->GetUniformLocation("u_PixelBitOffset.lifetime");
	glProgramUniform1ui(programID, loc, PixelMask::Lifetime::DEPTH);
	loc = shader->GetUniformLocation("u_PixelBitOffset.behaviour");
	glProgramUniform1ui(programID, loc, PixelMask::Behaviour::DEPTH);
	loc = shader->GetUniformLocation("u_PixelBitOffset.light");
	glProgramUniform1ui(programID, loc, PixelMask::Light::DEPTH);
}