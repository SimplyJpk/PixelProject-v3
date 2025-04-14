#pragma once

#include <vector>
#include <string>

#include "Misc/TextureFormatData.h"
#include "Macros/InlineReturn.h"

#include "Utility/Console.h"

class Texture
{
 public:
	Texture(const int width, const int height, const TextureFormat format);
	Texture(const char* filePath = nullptr);

	static std::vector<Texture*> CreateTextures(const int width,
		const int height,
		const TextureFormat format,
		const int count);

	~Texture();

	void Bind() const;

	template<typename T = void*>
	void UpdateTextureData(const T& data)
	{
		Bind();

		if (data == nullptr)
			return;

		auto target = FORMAT_DATA_TYPES[static_cast<int>(_format)][static_cast<int>(TextureFormatData::Target)];
		auto internalFormat =
			FORMAT_DATA_TYPES[static_cast<int>(_format)][static_cast<int>(TextureFormatData::InternalFormat)];
		auto format = FORMAT_DATA_TYPES[static_cast<int>(_format)][static_cast<int>(TextureFormatData::Format)];
		auto type = FORMAT_DATA_TYPES[static_cast<int>(_format)][static_cast<int>(TextureFormatData::Type)];

		glTexImage2D(target, 0, internalFormat, _width, _height, 0, format, type, data);
	}

	GLuint GetHandle() const
	INLINE_RETURN(_texture_id)
	int GetHeight() const
	INLINE_RETURN(_height)
	int GetWidth() const
	INLINE_RETURN(_width)
	TextureFormat GetFormat() const
	INLINE_RETURN(_format)
	void SetName(const std::string& name);

 private:
	static constexpr GLenum TEXTURE_TYPE = GL_TEXTURE_2D;

	GLuint _texture_id;
	int _width;
	int _height;

	TextureFormat _format = TextureFormat::UNKNOWN;
	std::string _file_name;
	unsigned char* _data = nullptr;

};