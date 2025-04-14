#pragma once
#include <cstdint>

#include "Config/GameSettings.h"
#include "Rendering/Sprite.h"
#include "Rendering/Texture.h"
#include "World/WorldConstants.h"
#include "World/Pixels/Base/BasePixel.h"

#include "Rendering/VertexBufferObject.h"
#include "Rendering/Shaders/Shader.h"
#include "World/Pixels/WorldDataHandler.h"

namespace SelectorConstants
{
	static constexpr int8_t PIXEL_TEXTURE_SIZE = 32;

	static constexpr int8_t UI_OFFSET_DISTANCE = 8;

	static constexpr int8_t ELEMENTS_PER_ROW = 16;
}

class PaintSelector
{
 public:
	static PaintSelector* GetInstance();

	uint8_t selected_pixel_type = 0;
	BasePixel* selected_pixel;

	void GenerateTextures(const GameSettings* game_settings);

	void Draw();

 protected:
	PaintSelector();
	VertexBufferObject* _vbo;
	static constexpr char SHADER_NAME[] = "orthoUI";

	static constexpr char MOUSE_EVENT_NAME[] = "PaintSelected_M1";
	void HandleMouseEvent(SDL_Event& event, const bool is_pressed);

 private:
	~PaintSelector();
	std::string _selected_pixel_name;

	Sprite _pixel_sprites[static_cast<int>(Pixel::TYPE_COUNT)];

	WorldDataHandler* _world_data = WorldDataHandler::GetInstance();
	Shader* _used_shader = nullptr;

	std::vector<Texture*> _pixel_textures;

	glm::mat4 _projection_transform;

	static constexpr glm::vec3 UNSELECTED_PIXEL_SCALE_VEC =
		{ SelectorConstants::PIXEL_TEXTURE_SIZE,
			SelectorConstants::PIXEL_TEXTURE_SIZE, 1 };
	static constexpr float SELECTED_PIXEL_SCALE = 1.5f;
	static constexpr glm::vec3 SELECTED_PIXEL_SCALE_VEC =
		{ SelectorConstants::PIXEL_TEXTURE_SIZE * SELECTED_PIXEL_SCALE,
			SelectorConstants::PIXEL_TEXTURE_SIZE * SELECTED_PIXEL_SCALE, 1 };
};
