#pragma once

#include <memory>
#include <SDL3/SDL_stdinc.h>
#include <GL/glew.h>

#include "GameObject/Transform.h"

#include "Macros/InlineReturn.h"

class Sprite
{
 public:
	Transform transform;

	GLuint GetTextureID() const
	INLINE_RETURN(_texture_id_)
	std::shared_ptr<Uint32[]> GetSprite() const
	INLINE_RETURN(_pixel_icon_)

	// Setters
	void SetTextureID(const GLuint id);
	void SetSprite(Uint32* sprite);
	void SetSprite(const std::shared_ptr<Uint32[]>& sprite);

 protected:
	GLuint _texture_id_ = -1;
	std::shared_ptr<Uint32[]> _pixel_icon_;

};
