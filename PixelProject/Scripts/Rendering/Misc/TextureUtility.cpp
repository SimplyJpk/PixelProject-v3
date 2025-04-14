#include "TextureUtility.h"

void TextureUtility::SetTextureParams(const GLenum target, const GLint wrap_s, const GLint wrap_t,
	const GLint min_filter, const GLint mag_filter)
{
	// set the texture wrapping parameters
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
	// set texture filtering parameters
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter);
}
