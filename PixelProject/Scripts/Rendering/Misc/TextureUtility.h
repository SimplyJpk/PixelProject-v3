#pragma once

#include <GL/glew.h>

class TextureUtility
{
 public:
	static void SetTextureParams(
		const GLenum target = GL_TEXTURE_2D,
		const GLint wrap_s = GL_REPEAT,
		const GLint wrap_t = GL_REPEAT,
		const GLint min_filter = GL_NEAREST,
		const GLint mag_filter = GL_NEAREST);
};
