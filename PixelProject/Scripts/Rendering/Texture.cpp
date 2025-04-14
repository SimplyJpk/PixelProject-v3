#include "Texture.h"

#include <stb_image.h>

#include "Misc/TextureUtility.h"
#include "Utility/Console.h"

Texture::Texture(const int width, const int height, const TextureFormat format)
{
	_width = width;
	_height = height;
	if (format == TextureFormat::UNKNOWN)
		_format = TextureFormat::RGBA;
	else
		_format = format;

	glGenTextures(1, &_texture_id);
	glBindTexture(TEXTURE_TYPE, _texture_id);

	// Bind and Set Data
	UpdateTextureData(nullptr);

	TextureUtility::SetTextureParams(TEXTURE_TYPE);
}

Texture::Texture(const char* filePath)
{
	int comp;
	unsigned char* texture = stbi_load(filePath, &_width, &_height, &comp, STBI_default);

	if (!texture) {
		LOG_ERROR("Failed to load texture: {}", filePath);
		if (stbi_failure_reason()) {
			DEBUG_WARNING_LOG("{}", stbi_failure_reason());
		}
	}
	glGenTextures(1, &_texture_id);
	glBindTexture(TEXTURE_TYPE, _texture_id);

	//TODO Will this work with my INT?
	//TODO Test this with loading images
	switch (comp) {
	case STBI_grey:
		_format = TextureFormat::RED_SMALL;
		glTexImage2D(TEXTURE_TYPE, 0, GL_RED, _width, _height,
			0, GL_RED, GL_UNSIGNED_BYTE, texture);
		break;
	case STBI_grey_alpha:
		_format = TextureFormat::RG;
		glTexImage2D(TEXTURE_TYPE, 0, GL_RG, _width, _height,
			0, GL_RG, GL_UNSIGNED_BYTE, texture);
		break;
	case STBI_rgb:
		_format = TextureFormat::RGB;
		glTexImage2D(TEXTURE_TYPE, 0, GL_RGB, _width, _height,
			0, GL_RGB, GL_UNSIGNED_BYTE, texture);
		break;
	case STBI_rgb_alpha:
		_format = TextureFormat::RGBA;
		glTexImage2D(TEXTURE_TYPE, 0, GL_RGBA, _width, _height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
		break;
	default:
		_format = TextureFormat::UNKNOWN;
		break;
	}
	_file_name = filePath;

	_data = texture;
	TextureUtility::SetTextureParams(TEXTURE_TYPE);
}

std::vector<Texture*> Texture::CreateTextures(const int width, const int height, const TextureFormat format,
	const int count)
{
	std::vector<Texture*> textures;
	textures.reserve(count);
	for (int i = 0; i < count; i++) {
		textures.push_back(new Texture(width, height, format));
	}
	return textures;
}

Texture::~Texture()
{
	if (_texture_id != 0)
		glDeleteTextures(1, &_texture_id);
	if (_data != nullptr)
		stbi_image_free(_data);
}

void Texture::Bind() const
{
	glBindTexture(TEXTURE_TYPE, _texture_id);
}

void Texture::SetName(const std::string& name)
{
	_file_name = name;
}
