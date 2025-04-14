#include "PaintSelector.h"

#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.inl>

#include "Config/GameSettings.h"
#include "Input/InputManager.h"
#include "Rendering/Shaders/ShaderManager.h"
#include "Utility/Console.h"

PaintSelector::PaintSelector()
{
	selected_pixel = _world_data->GetPixelFromIndex(1);

	constexpr int textureSize = SelectorConstants::PIXEL_TEXTURE_SIZE;
	constexpr int halfSize = textureSize / 2;

	for (int index = 0; index < Pixel::TYPE_COUNT; index++) {
		const auto textureInfo = new Uint32[textureSize * textureSize]{ 0x00000000 };
		BasePixel* pixel = _world_data->GetPixelFromIndex(static_cast<int8_t>(index));

		constexpr auto radius = static_cast<float>(halfSize * halfSize);

		for (int y = 0; y < textureSize; ++y) {
			for (int x = 0; x < textureSize; ++x) {
				auto dx = x - halfSize;
				auto dy = y - halfSize;
				auto dist = static_cast<float>(dx * dx + dy * dy);

				if (dist <= radius) {
					textureInfo[y * textureSize + x] = pixel->GetRandomColour();
				}
			}
		}

		_pixel_sprites[index].SetSprite(textureInfo);
	}

	_vbo = new VertexBufferObject(VBOShape::Square);

	const auto inputManager = InputManager::GetInstance();

	const auto handleKeyFunction =
		std::bind(&PaintSelector::HandleMouseEvent, this, std::placeholders::_1, std::placeholders::_2);
	inputManager->AddMouseListener(MouseCode::MouseLeft, MOUSE_EVENT_NAME, handleKeyFunction);
}

PaintSelector* PaintSelector::GetInstance()
{
	static PaintSelector instance;
	return &instance;
}

void PaintSelector::GenerateTextures(const GameSettings* game_settings)
{
	static constexpr int TEXTURE_SIZE = SelectorConstants::PIXEL_TEXTURE_SIZE;
	const auto shaderManager = ShaderManager::GetInstance();
	if (shaderManager->CreateShaderProgramFromFiles(GetShaderMask(ShaderMask::MVertex, ShaderMask::MFragment),
		SHADER_NAME,
		"shaders/orthoUI") == nullptr) {
		LOG_ERROR("[PaintSelector] Failed to load shader");
		return;
	}
	_used_shader = shaderManager->GetShader(SHADER_NAME);

	_pixel_textures = Texture::CreateTextures(TEXTURE_SIZE, TEXTURE_SIZE, TextureFormat::RGBA, Pixel::TYPE_COUNT);

	const int iconOffset = TEXTURE_SIZE;
	const int textuers_per_row = 8;
	for (int i = 0; i < Pixel::TYPE_COUNT; i++) {
		_pixel_textures[i]->UpdateTextureData(_pixel_sprites[i].GetSprite().get());
		// Setup structure for future use
		_pixel_sprites[i].SetTextureID(_pixel_textures[i]->GetHandle());

		// 8 pixels per row
		int x = (i % textuers_per_row) + 1;
		int y = (i / textuers_per_row) + 1;

		//? Is this trash way to do this?
		Transform& trans = _pixel_sprites[i].transform;
		const glm::vec3 position = glm::vec3(
			iconOffset + (TEXTURE_SIZE * x) + (iconOffset * x),
			(TEXTURE_SIZE * y) + (iconOffset * (y - 1)),
			0
		);
		trans.SetPosition(position);
		trans.SetScale(glm::vec3(TEXTURE_SIZE, TEXTURE_SIZE, 1.0f));
	}

	_projection_transform = glm::ortho(0.0f,
		static_cast<float>(game_settings->screen_size.x),
		static_cast<float>(game_settings->screen_size.y),
		0.0f,
		-1.0f,
		1.0f);
}

void PaintSelector::HandleMouseEvent(SDL_Event& event, const bool is_pressed)
{
	if (!is_pressed)
		return;

	auto mousePos = IVec2(event.button.x, event.button.y);

	for (short index = 0; index < Pixel::TYPE_COUNT; index++) {
		if (selected_pixel->pixel_index == index)
			continue;

		auto& sprite = _pixel_sprites[index];
		auto const& bounds = sprite.transform.GetBounds();
		// If we're moused over
		if (mousePos.x > bounds.x && mousePos.x < bounds.z &&
			mousePos.y > bounds.y && mousePos.y < bounds.w) {
			// Reset previous scale of selected pixel
			_pixel_sprites[selected_pixel->pixel_index].transform.SetScale(UNSELECTED_PIXEL_SCALE_VEC);
			// Set new, apply scale
			selected_pixel = _world_data->GetPixelFromIndex(index);
			sprite.transform.SetScale(SELECTED_PIXEL_SCALE_VEC);

			DEBUG_LOG("[PaintSelector] Selected pixel: {}", index);
			break;
		}
	}
}

void PaintSelector::Draw()
{
	_used_shader->UseProgram();

	int modelLoc = _used_shader->GetUniformLocation("model");
	int projLoc = _used_shader->GetUniformLocation("projection");

	for (int i = 0; i < Pixel::TYPE_COUNT; i++) {
		glActiveTexture(GL_TEXTURE0);
		_pixel_textures[i]->Bind();
		if (i == 0) {
			_vbo->Bind();
		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(_pixel_sprites[i].transform.GetModelMatrix()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_projection_transform));

		_vbo->Draw();
	}
}

PaintSelector::~PaintSelector()
{
	InputManager::GetInstance()->RemoveMouseListener(MouseCode::MouseLeft, MOUSE_EVENT_NAME);
}
