#pragma once
#include <GL/glew.h>

enum class TextureFormat : unsigned int
{
	UNKNOWN = 0,
	RED_SMALL = 1,
	// Dsd
	RED_LARGE,
	RG,
	RGB,
	RGBA,
	TYPE_COUNT
};

enum class TextureFormatData : unsigned int
{
	Target = 0,
	InternalFormat = 1,
	Format = 2,
	Type = 3,
	VAR_COUNT = 4,
};

namespace TextureData
{
	//TODO Look into naming convensions for defines?
	//TODO Look into a better way to organise this data?
	#define SHORT_TYPE constexpr static GLuint

	typedef TextureFormatData TFD;
	#define SHORT_V static_cast<int>(TFD::VAR_COUNT)

	SHORT_TYPE RED_SMALL[SHORT_V] = { GL_TEXTURE_2D, GL_RED, GL_RED, GL_UNSIGNED_BYTE };
	SHORT_TYPE RED_LARGE[SHORT_V] = { GL_TEXTURE_2D, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT };
	SHORT_TYPE RG[SHORT_V] = { GL_TEXTURE_2D, GL_RG, GL_RG, GL_UNSIGNED_INT_8_8_8_8 };
	SHORT_TYPE RGB[SHORT_V] = { GL_TEXTURE_2D, GL_RGB, GL_RGB, GL_UNSIGNED_INT_8_8_8_8 };
	SHORT_TYPE RGBA[SHORT_V] = { GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8 };

	#undef SHORT_TYPE
	#undef SHORT_V
}

constexpr static const GLuint* FORMAT_DATA_TYPES[static_cast<int>(TextureFormat::TYPE_COUNT)] =
	{
		TextureData::RGBA, // Unknown
		TextureData::RED_SMALL,
		TextureData::RED_LARGE,
		TextureData::RG,
		TextureData::RGB,
		TextureData::RGBA
	};