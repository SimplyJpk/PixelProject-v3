#pragma once
#include "WorldConstants.h"

#pragma region Boost

#include <boost/lockfree/queue.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/thread/mutex.hpp>

#pragma endregion Boost

#include "Camera/Camera.h"
#include "Rendering/VertexBufferObject.h"

#include "Chunk/WorldChunk.h"
#include "Config/GameSettings.h"
#include "Math/Vec2/IVec2.h"
#include "Rendering/Texture.h"
#include "Rendering/Shaders/Shader.h"

#include <array>
#include <condition_variable>

#include "UI/IDrawGUI.h"
#include "Utility/XorShift.h"
#include "Utility/Time/TimeWatcher.h"

using namespace boost;

enum class WorldSimulatorState : uint8_t
{
	Paused,
	Running,
};

class WorldSimulator : IDrawGUI
{
 public:
	WorldSimulator(Shader* draw_shader, const std::shared_ptr<GameSettings>& game_settings, Camera* camera);
	static constexpr IVec2 WORLD_SIZE = IVec2(World::SIZE_X, World::SIZE_Y);

	void Update();
	void FixedUpdate();

	// InputUpdate
	void DrawOnWorld(const IVec2& mousePos, const glm::vec3& cameraPos, float zoom, int size = 5, const bool override_pixels = false);

	void Draw(const Camera* camera);

	uint8_t current_frame_id = 0;

	void SaveWorld();
	void LoadWorld();

	void SetSimState(const WorldSimulatorState state)
	{
		_sim_state = state;
	}
	WorldSimulatorState GetSimState() const
	{
		return _sim_state;
	}

 protected:
	void UpdateChunk(const IVec2& chunk_index);
	void FillPixelUpdateDirections();

	std::unordered_map<IVec2, WorldChunk*> _chunks;

	std::array<std::array<uint8_t, 8>, Pixel::TYPE_COUNT> _pixel_update_dirs;

	// Used to render the chunks, TODO : (James) should try optimize this
	Texture* _map_texture;

	Texture* _noise_texture;
	Uint8* _map_noise_texture_data;

	const std::shared_ptr<GameSettings> _game_settings;
	Shader* _draw_shader;
	static constexpr int PEN_SIZE = 6;

	// Creates more noise between frame updates so that chunk bleeding is less noticeable
	uint8_t _current_update_order = 0;
	static constexpr short X_UPDATE_ORDER[2][3] = {
		{ Chunk::ARRAY_ENTRY_INDEX, Chunk::ARRAY_MAX_X, Chunk::INCREMENT_POSITIVE },
		{ Chunk::ARRAY_MAX_X, Chunk::ARRAY_ENTRY_INDEX, Chunk::INCREMENT_NEGATIVE }
	};
	static constexpr short Y_UPDATE_ORDER[2][3] = {
		{ Chunk::ARRAY_ENTRY_INDEX, Chunk::ARRAY_MAX_Y, Chunk::INCREMENT_POSITIVE },
		{ Chunk::ARRAY_MAX_Y, Chunk::ARRAY_ENTRY_INDEX, Chunk::INCREMENT_NEGATIVE }
	};

	WorldSimulatorState _sim_state = WorldSimulatorState::Running;

 protected:
	virtual void OnDrawGUI(float delta_time) override;

 private:
	std::atomic<int> _thread_pool_tasks = 0;
	asio::thread_pool _thread_pool = asio::thread_pool(32);
	Camera* _camera;

	VertexBufferObject* _vbo;

	std::mutex _chunk_mutex;
	std::condition_variable _chunk_condition_variable;

	TimeWatcher<TimeFormat::Micro, IVec2> _time_watcher;

	XorShift _rng;
};
