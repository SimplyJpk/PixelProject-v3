#include "Sprite.h"

void Sprite::SetTextureID(const GLuint id)
{
	_texture_id_ = id;
}

void Sprite::SetSprite(Uint32* sprite)
{
	_pixel_icon_ = std::shared_ptr<Uint32[]>(sprite);
}

void Sprite::SetSprite(const std::shared_ptr<Uint32[]>& sprite)
{
	_pixel_icon_ = sprite;
}
