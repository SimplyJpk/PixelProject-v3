#pragma once
#include <cstdint>
#include <GL/glew.h>

#include "Shader.h"

enum class ShaderMask : uint8_t
{
	MVertex = 1 << 0,
	MFragment = 1 << 1,
	MGeometry = 1 << 2,
	MTessEval = 1 << 3,
	MTessControl = 1 << 4,
	MCompute = 1 << 5
};
template<typename... ShaderMask>
static constexpr uint8_t GetShaderMask(ShaderMask... args)
{
	return (static_cast<uint8_t>(args) | ...);
}

static constexpr int SHADER_MASK_MAX_STRIDE = static_cast<int>(ShaderMask::MCompute);

enum class ShaderTypes : GLint
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	TessEval = GL_TESS_EVALUATION_SHADER,
	TessControl = GL_TESS_CONTROL_SHADER,
	Compute = GL_COMPUTE_SHADER
};

static constexpr ShaderTypes SHADER_TYPES[6] =
	{ ShaderTypes::Vertex, ShaderTypes::Fragment, ShaderTypes::Geometry, ShaderTypes::TessEval,
		ShaderTypes::TessControl, ShaderTypes::Compute };
static constexpr const char* SHADER_TYPE_NAMES[6] =
	{ "Vertex", "Fragment", "Geometry", "Tessellation Evaluation", "Tessellation Control", "Compute" };

class ShaderManager
{
 public:
	static ShaderManager* GetInstance();

	Shader* CreateShaderProgram(const std::string& shader_name, bool delete_sources = true);
	Shader* CreateShaderProgramFromFiles(uint8_t shader_mask, const std::string& name, const std::string& file_name);

	bool ShaderFromText(ShaderTypes type, const std::string& name, const char* src);
	bool ShaderFromFile(ShaderTypes type, const std::string& name, const std::string& file_name);

	//TODO Implement
	bool ShaderCompiledFile(GLenum type, std::string name, const std::string file_name);

	bool ShaderFromFiles(uint8_t shader_mask, const std::string& name, const std::string& file_name);

	Shader* GetShader(GLint program_id);
	Shader* GetShader(const std::string& program_name) const;

	bool SetDefaultShader(const std::string& shader_name);
	void SetDefaultShader(const Shader* shader);
	Shader* GetDefaultShader() const;

	GLint GetProgramID(const char* program_name);
	std::string GetProgramName(const GLint program_id);

	inline void UseProgram(const char* program_name);
	static inline void UseProgram(const GLint program_id);

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	void operator=(const ShaderManager&) = delete;
	void operator=(ShaderManager&&) = delete;

	static constexpr int GetShaderIndex(const ShaderTypes gl_shader_type);
	static constexpr ShaderMask GetShaderMask(const ShaderTypes gl_shader_type);

 protected:
	static constexpr short SHADER_TYPES_COUNT = 6;
	std::string _default_shader_name;

 private:
	ShaderManager() = default;
	~ShaderManager() = default;

	const std::string SHADER_FILE_EXT[SHADER_TYPES_COUNT] = { "vert", "frag", "geom", "tesse", "tessc", "comp" };

	std::unordered_map<std::string, GLint> _program_id;
	std::unordered_map<GLint, std::string> _program_name;

	std::map<std::string, Shader*> _shaders;

	// Used to simplify
	std::unordered_map<std::string, int[SHADER_TYPES_COUNT]> _shader_contents_map;

};
