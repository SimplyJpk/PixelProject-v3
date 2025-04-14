#include "WorldSimulator.h"

#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ranges>

#include "Pixels/WorldDataHandler.h"
#include "Pixels/Base/BasePixel.h"
#include "PixelMask.h"
#include "Utility/WorldGenerator.h"

#include <algorithm>

#include "Input/InputManager.h"
#include "UI/Paint/PaintSelector.h"
#include <implot.h>

#include "Debug/DebugAssertion.h"

WorldSimulator::WorldSimulator(Shader* draw_shader, const std::shared_ptr<GameSettings>& game_settings, Camera* camera)
	: _game_settings(game_settings),
	  _camera(camera)
{
	_draw_shader = draw_shader;
	_vbo = new VertexBufferObject(VBOShape::Square);

	// Initialize chunks
	_chunks.reserve(WORLD_SIZE.x * WORLD_SIZE.y);
	for (int x = 0; x < WORLD_SIZE.x; x++) {
		for (int y = 0; y < WORLD_SIZE.y; y++) {
			auto chunkIndex = IVec2(x, y);
			auto chunk = new WorldChunk(chunkIndex);

			_time_watcher.AddTimeTracker(chunkIndex);
			_chunks.insert({ chunkIndex, chunk });
		}
	}

	// Loop through and set neighbours
	for (auto& [chunkIndex, chunk] : _chunks) {
		for (int i = 0; i < Chunk::NUM_DIRECTIONS; i++) {
			IVec2 neighbourIndex = chunkIndex + Chunk::GetDirVector(static_cast<WorldDir>(i));
			if (_chunks.contains(neighbourIndex)) {
				chunk->neighbour_chunks[i] = _chunks[neighbourIndex];
			}
		}
	}

	// Initialize map texture
	_map_texture = new Texture(Chunk::SIZE_X, Chunk::SIZE_Y, TextureFormat::RED_LARGE);

	// Set chunk data
	for (int x = 0; x < WORLD_SIZE.x; x++) {
		for (int y = 0; y < WORLD_SIZE.y; y++) {
			// WorldGenerator::GenerateChunk(_chunks[IVec2(x, y)]);
		}
	}

	draw_shader->UseProgram();
	_noise_texture = new Texture(Chunk::SIZE_X, Chunk::SIZE_Y, TextureFormat::RED_SMALL);
	_map_noise_texture_data = new Uint8[Chunk::TOTAL_SIZE]{ 0 };

	for (int i = 0; i < Chunk::TOTAL_SIZE; i++) {
		_map_noise_texture_data[i] = _rng() % MAX_COLOUR_COUNT;
	}
	_noise_texture->UpdateTextureData(_map_noise_texture_data);

	InputManager::GetInstance()->AddMouseListener(
		MouseCode::MouseMiddle, "MousePenDraw",
		[this](const SDL_Event& event, bool is_down)
		{
		  if (is_down) {
			  auto pos = IVec2(event.motion.x, event.motion.y);
			  DrawOnWorld(pos, _camera->GetPosition(), _camera->GetZoom(), PEN_SIZE, true);
		  }
		});

	InputManager::GetInstance()->AddKeyListener(KeyCode::K, "SaveWorld", [this](SDL_Event& event, bool state)
	{
	  if (state) {
		  SaveWorld();
	  }
	});

	InputManager::GetInstance()->AddKeyListener(KeyCode::L, "LoadWorld", [this](SDL_Event& event, bool state)
	{
	  if (state) {
		  if (_sim_state == WorldSimulatorState::Paused) {
			  return;
		  }
		  SetSimState(WorldSimulatorState::Paused);
		  LoadWorld();
	  }
	  else {
		  SetSimState(WorldSimulatorState::Running);
	  }
	});
}

void WorldSimulator::FixedUpdate()
{
	current_frame_id++;

	if (_sim_state == WorldSimulatorState::Paused) {
		return;
	}

	std::unique_lock<std::mutex> lock(_chunk_mutex);
	// Update order of update for chunks
	_current_update_order = (_current_update_order + 1) % 2;

	FillPixelUpdateDirections();

	// Spaced chunk updates so that a chunk can update and reach into a neighbour without risk of conflict
	for (int iteration = 0; iteration < 4; iteration++) {
		for (int yChunkIndex = iteration % 2; yChunkIndex < WORLD_SIZE.y; yChunkIndex += 2) {
			for (int xChunkIndex = (iteration / 2) % 2; xChunkIndex < WORLD_SIZE.x; xChunkIndex += 2) {
				UpdateChunk(IVec2{ xChunkIndex, yChunkIndex });
			}
		}
		_chunk_condition_variable.wait(lock, [this]()
		{ return _thread_pool_tasks == 0; });
	}

}

void WorldSimulator::UpdateChunk(const IVec2& chunk_index)
{
	++_thread_pool_tasks;

	asio::post(_thread_pool, [this, chunk_index]()
	{
	  auto chunkTimer = _time_watcher[chunk_index];
	  chunkTimer->Start();

	  // Helpers
	  const auto& worldData = WorldDataHandler::GetInstance();

	  // Chunk
	  auto& localChunk = _chunks[chunk_index];
	  auto* localPixels = localChunk->pixel_data;
	  auto* localLastUpdated = localChunk->last_updated;

	  const auto& chunkNeighbours = localChunk->neighbour_chunks;

	  // Pixel Update
	  for (auto xIndex = X_UPDATE_ORDER[_current_update_order][0]; xIndex != X_UPDATE_ORDER[_current_update_order][1];
		   xIndex += X_UPDATE_ORDER[_current_update_order][2]) {
		  DEBUG_ASSERT_OBJ(xIndex >= 0 && xIndex <= Chunk::ARRAY_MAX_X, xIndex)

		  uint8_t pixelBorderMask = ChunkPixelBorderMask::Undefined;
		  // Branch-less Calculate X Borders
		  pixelBorderMask |= (xIndex == 0) * ChunkPixelBorderMask::West;
		  pixelBorderMask |= (xIndex == Chunk::ARRAY_MAX_X) * ChunkPixelBorderMask::East;

		  for (auto yIndex = Y_UPDATE_ORDER[_current_update_order][0]; yIndex != Y_UPDATE_ORDER[_current_update_order][1];
			   yIndex += Y_UPDATE_ORDER[_current_update_order][2]) {
			  DEBUG_ASSERT_OBJ(yIndex >= 0 && yIndex <= Chunk::ARRAY_MAX_Y, yIndex)
			  const uint16_t localPixelIndex = xIndex + yIndex * Chunk::SIZE_X;
			  DEBUG_ASSERT_OBJ(localPixelIndex <= Chunk::ARRAY_MAX_INDEX, localPixelIndex)

			  auto& localPixelValue = localPixels[localPixelIndex];

			  // If air, skip
			  if (localPixelValue == 0)
				  continue;

			  if (localLastUpdated[localPixelIndex] == current_frame_id)
				  continue;

			  const auto& pixel = worldData->GetPixelFromIndex(PixelMask::Index::GetValue(localPixelValue));
			  if (!pixel->is_updateable)
				  continue;

			  // Branch-less Calculate Y Borders
			  pixelBorderMask &= ~(ChunkPixelBorderMask::North | ChunkPixelBorderMask::South);
			  pixelBorderMask |= (yIndex == 0) * ChunkPixelBorderMask::North;
			  pixelBorderMask |= (yIndex == Chunk::ARRAY_MAX_Y) * ChunkPixelBorderMask::South;

			  const auto& pixelTypeUpdateDirections = _pixel_update_dirs[pixel->pixel_index];
			  for (const uint8_t& updateDir : pixelTypeUpdateDirections) {
				  PixelUpdateResult pixelUpdateResult;
				  // If the update direction is Undefined, we have reached the end of the list and break out
				  if (static_cast<Chunk::WorldDir>(updateDir) == Chunk::WorldDir::Undefined)
					  break;

				  uint16_t neighbourPixelIndex;
				  const bool pixelWillTraverse = Pixel::WillTraverseToNewChunk(localPixelIndex, updateDir, neighbourPixelIndex, pixelBorderMask);

				  auto* externalPixels = localPixels;
				  auto* externalLastUpdated = localLastUpdated;

				  // We check if the pixel will traverse to a new chunk
				  if (pixelWillTraverse) {
					  const auto dirBorderMask = Chunk::WorldDirToChunkBorderMask(static_cast<Chunk::WorldDir>(updateDir));
					  // If the borderMask and the dirBorderMask share any bits, then we know that the chunk we are looking for is a neighbour
					  const auto sharedBorderBits = (pixelBorderMask & dirBorderMask);

					  // Use the chunk corresponding to the sharedBorderBits
					  const auto chunkBorderDir = static_cast<int>(Chunk::ChunkBorderMaskToWorldDir(static_cast<ChunkPixelBorderMask>(sharedBorderBits)));
					  //! If the chunk doesn't exist, we skip the update
					  if (chunkNeighbours[chunkBorderDir] == nullptr) {
						  continue;
					  }

					  externalPixels = chunkNeighbours[chunkBorderDir]->pixel_data;
					  externalLastUpdated = chunkNeighbours[chunkBorderDir]->last_updated;
				  }

				  // Ensure the neighbourPixelIndex is within valid bounds
				  if (neighbourPixelIndex >= Chunk::TOTAL_SIZE) {
					  neighbourPixelIndex %= Chunk::TOTAL_SIZE;
				  }

				  // Additional boundary checks to prevent underflow/overflow
				  if (neighbourPixelIndex < 0) {
					  neighbourPixelIndex = 0;
				  } else if (neighbourPixelIndex >= Chunk::TOTAL_SIZE) {
					  neighbourPixelIndex = Chunk::TOTAL_SIZE - 1;
				  }

				  auto& neighbourPixelValue = externalPixels[neighbourPixelIndex];
				  const auto& neighbourPixel = worldData->GetPixelFromIndex(PixelMask::Index::GetValue(neighbourPixelValue));

				  pixelUpdateResult.SetNeighbour(neighbourPixel->pixel_type);
				  pixelUpdateResult.SetDirection(static_cast<Chunk::WorldDir>(updateDir));

				  pixel->UpdatePixel(pixelUpdateResult, localPixelValue);

				  switch (pixelUpdateResult.Result()) {
				  case LogicResult::SuccessUpdate:
					  std::swap(localPixels[localPixelIndex], externalPixels[neighbourPixelIndex]);
					  externalLastUpdated[neighbourPixelIndex] = current_frame_id;
					  break;

				  case LogicResult::FirstReturnPixel:
					  localPixels[localPixelIndex] = worldData->GetPixelFromType(pixelUpdateResult.NewLocal())->GetNewPixel();
					  localLastUpdated[localPixelIndex] = current_frame_id;
					  break;
				  case LogicResult::SecondReturnPixel:
					  externalPixels[neighbourPixelIndex] = worldData->GetPixelFromType(pixelUpdateResult.NewNeighbour())->GetNewPixel();
					  externalLastUpdated[neighbourPixelIndex] = current_frame_id;
					  break;
				  case LogicResult::DualReturnPixel:
					  localPixels[localPixelIndex] = worldData->GetPixelFromType(pixelUpdateResult.NewLocal())->GetNewPixel();
					  localLastUpdated[localPixelIndex] = current_frame_id;

					  externalPixels[neighbourPixelIndex] = worldData->GetPixelFromType(pixelUpdateResult.NewNeighbour())->GetNewPixel();
					  externalLastUpdated[neighbourPixelIndex] = current_frame_id;
					  break;
				  case LogicResult::NoChange:
					  break;

				  case LogicResult::FailedUpdate:
				  case LogicResult::None:
				  default:
					  continue;
				  }
				  break;
			  }
		  }
	  }

	  chunkTimer->StopWithoutReturn();

	  --_thread_pool_tasks;
	  if (_thread_pool_tasks == 0)
		  _chunk_condition_variable.notify_one();
	});
}

void WorldSimulator::Draw(const Camera* camera)
{
	_draw_shader->UseProgram();

	const auto modelLoc = _draw_shader->GetUniformLocation("model");
	const auto projLoc = _draw_shader->GetUniformLocation("projection");

	const auto& screenSize = _game_settings->screen_size;

	glActiveTexture(GL_TEXTURE1);
	_noise_texture->Bind();

	glActiveTexture(GL_TEXTURE0);
	_map_texture->Bind();

	// Find the sampler2D location of noiseTextureIndex
	constexpr GLuint noiseTextureIndex = 1;
	const auto noiseTextureLoc = _draw_shader->GetUniformLocation("noiseTextureIndex");
	glUniform1i(noiseTextureLoc, noiseTextureIndex);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	// TODO : (James) Debugging, Remove or improve this (Zoom in)
	const float extra_scale = camera->GetZoom();
	const glm::vec3& cameraPos = camera->GetPosition();

	int chunksDrawn = 0;
	for (int xVal = 0; xVal < World::SIZE_X; xVal++) {
		for (int yVal = 0; yVal < World::SIZE_Y; yVal++) {
			auto worldChunk = _chunks.find(IVec2(xVal, yVal));
			// Chunk doesn't exist, we don't render
			if (worldChunk == _chunks.end())
				continue;

			_map_texture->UpdateTextureData(&worldChunk->second->pixel_data);

			auto model = glm::mat4(1.0f);

			// Set model position
			auto modelPosition = glm::vec3(
				(((xVal) + 1) * Chunk::SIZE_X * extra_scale) - (Chunk::HALF_X * extra_scale) + xVal + cameraPos.x,
				// + xVal is for fake grid TODO make a proper visual grid
				(((yVal) + 1) * Chunk::SIZE_Y * extra_scale) - (Chunk::HALF_Y * extra_scale) + yVal + cameraPos.z,
				1.0f
			);

			// Don't render if it's off-screen
			if (modelPosition.x + Chunk::SIZE_X < 0 || modelPosition.y + Chunk::SIZE_Y < 0)
				continue;
			if (modelPosition.x - Chunk::SIZE_X > screenSize.x * 1.0f || modelPosition.y - Chunk::SIZE_Y > screenSize.y
				* 1.0f)
				continue;

			model = glm::translate(model, modelPosition);

			model = glm::scale(model, glm::vec3(Chunk::SIZE_X * extra_scale, Chunk::SIZE_Y * extra_scale, 1.0f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			_vbo->Bind();
			_vbo->Draw();

			chunksDrawn++;
		}
	}
}

void WorldSimulator::FillPixelUpdateDirections()
{
	for (int i = 0; i < Pixel::TYPE_COUNT; i++) {
		auto dirs = WorldDataHandler::GetInstance()->GetPixelFromIndex(i)->GetPixelUpdateOrder();
		std::copy(dirs.begin(), dirs.end(), _pixel_update_dirs[i].begin());
	}
}

void WorldSimulator::DrawOnWorld(const IVec2& mousePos, const glm::vec3& cameraPos, float zoom, const int size, const bool override_pixels)
{
	// Calculate the zoomed size of the pen
	const auto zoomedSize = size * zoom;
	const auto halfSize = zoomedSize / 2;
	const auto halfSizeVec = IVec2((int)halfSize,(int)halfSize);

	// Calculate the position of the pen in the world
	const auto position = IVec2(
		static_cast<int>((mousePos.x - cameraPos.x) / zoom),
		static_cast<int>((mousePos.y - cameraPos.z) / zoom)
	);

	// Calculate the top left and bottom right positions of the pen in the world
	IVec2 penTopLeft = (position) - halfSizeVec;
	IVec2 penBottomRight = (position) + halfSizeVec;

	// Make sure the pen is within the bounds of the world
	penTopLeft.x = glm::clamp(penTopLeft.x, 0, World::SIZE_X * Chunk::ARRAY_MAX_X);
	penTopLeft.y = glm::clamp(penTopLeft.y, 0, World::SIZE_Y * Chunk::ARRAY_MAX_Y);

	penBottomRight.x = glm::clamp(penBottomRight.x, 0, World::SIZE_X * Chunk::ARRAY_MAX_X);
	penBottomRight.y = glm::clamp(penBottomRight.y, 0, World::SIZE_Y * Chunk::ARRAY_MAX_Y);

	const auto& pixelType = PaintSelector::GetInstance()->selected_pixel;

	for (int yIndex = penTopLeft.y; yIndex < penBottomRight.y; yIndex++) {
		const auto yFloor = static_cast<int>(std::floor(yIndex / Chunk::SIZE_Y));
		auto yPixelIndex = (yIndex % Chunk::SIZE_Y) * Chunk::SIZE_X;

		for (int xIndex = penTopLeft.x; xIndex < penBottomRight.x; xIndex++) {
			const auto dist = pow(position.x - xIndex, 2.0) + pow(position.y - yIndex, 2.0);
			if (dist <= size) {
				// calculate the index of the pixel in the chunk
				const auto xFloor = static_cast<int>(std::floor(xIndex / Chunk::SIZE_X));
				const auto pixelIndex = yPixelIndex + (xIndex - (xFloor * Chunk::SIZE_X));

				if (!override_pixels && _chunks[IVec2(xFloor, yFloor)]->pixel_data[pixelIndex] != 0)
					continue;

				_chunks[IVec2(xFloor, yFloor)]->pixel_data[pixelIndex] = pixelType->GetNewPixel();
			}
		}
	}
}

void WorldSimulator::SaveWorld()
{
	for (const auto& chunk : _chunks | std::views::values) {
		chunk->StartSave();
	}
}

void WorldSimulator::LoadWorld()
{
	for (const auto& chunk : _chunks | std::views::values) {
		chunk->StartLoad();
	}
}

void WorldSimulator::OnDrawGUI(float delta_time)
{
	ImGui::Begin("World Simulator");

	if (ImPlot::BeginPlot("Chunk Updates")) {
		// array of random ImVec4 colours to use for plots
		static ImVec4 colours[30];
		if (colours[0].x == 0) {
			for (int i = 0; i < 30; i++) {
				colours[i] = ImVec4((_rng() % 256) / 255.0f, (_rng() % 256) / 255.0f, (_rng() % 256) / 255.0f, 0.75f);
			}
		}

		const auto chunkTimerHistory = _time_watcher[IVec2(0, 0)]->GetHistory();
		const auto maxHistory = chunkTimerHistory.size();
		ImPlot::SetupAxes("Chunk Update", "Time in Microseconds", ImPlotAxisFlags_RangeFit | ImPlotAxisFlags_Lock,
			ImPlotAxisFlags_RangeFit | ImPlotAxisFlags_AutoFit);
		// Set x limits, but keep y limits auto-fitting
		ImPlot::SetupAxesLimits(0, maxHistory, 0, 1000);

		for (const auto& [chunkIndex, timer] : _time_watcher.GetTimeTrackerCollection()) {
			const int colourIndex = (chunkIndex.x + (chunkIndex.y * WORLD_SIZE.x)) % 30;
			ImPlot::PushStyleColor(ImPlotCol_Line, colours[colourIndex]);

			const auto& history = timer.GetHistory();
			const auto chunk_index_str = std::format("Chunk: ({0}, {1})", chunkIndex.x, chunkIndex.y);
			ImPlot::PlotLine<float>(chunk_index_str.c_str(), history.data(), maxHistory);
		}

		ImPlot::EndPlot();

		ImGui::Text("Current Frame: %d", current_frame_id);
		#ifdef DEBUG_EXPENSIVE_COUNT_PIXELS
		int sandUpdates = 0;
		for (const auto& chunk : _chunks | std::views::values) {
			for (unsigned int i : chunk->pixel_data) {
				if (i != 0) {
					sandUpdates++;
				}
			}
		}
		ImGui::Text("Sand Updates: %d", sandUpdates);
		#endif
		}

	ImGui::End();
}