#include "ShaderManager.h"
#include <vector>

#include "Utility/Console.h"
#include "Utility/FileIO.h"

using namespace PixelProject;

ShaderManager* ShaderManager::GetInstance()
{
	static ShaderManager instance;
	return &instance;
}

Shader* ShaderManager::CreateShaderProgram(const std::string& shader_name, const bool delete_sources)
{
	const GLuint program = glCreateProgram();
	const auto shaderArray = _shader_contents_map[shader_name];
	int shadersAdded = 0;
	for (int i = 0; i < SHADER_TYPES_COUNT; i++) {
		if (shaderArray[i] == 0)
			continue;
		glAttachShader(program, shaderArray[i]);
		shadersAdded++;
	}
	// Link our program
	glLinkProgram(program);
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, static_cast<int*>(&isLinked));
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// Clear Program, and shaders if we're doing that here.
		glDeleteProgram(program);
		//TODO Tidy this with below #1
		if (delete_sources) {
			for (int i = 0; i < SHADER_TYPES_COUNT; i++) {
				if (shaderArray[i] != 0)
					glDeleteShader(shaderArray[i]);
			}
			_shader_contents_map.erase(shader_name);
		}
		return nullptr;
	}
	printf("Shader Program '%s' Generated using %i modules\n", shader_name.c_str(), shadersAdded);
	_program_id[shader_name.c_str()] = static_cast<int>(program);
	_program_name[static_cast<int>(program)] = shader_name;

	// Add to linked Shaders
	Shader* shader = new Shader(program, shader_name);
	_shaders[shader_name] = shader;

	//TODO Tidy this with above #1
	if (!delete_sources)
		return shader;

	for (int i = 0; i < SHADER_TYPES_COUNT; i++) {
		if (shaderArray[i] != 0)
			glDeleteShader(shaderArray[i]);
	}
	_shader_contents_map.erase(shader_name);
	return shader;
}

Shader* ShaderManager::CreateShaderProgramFromFiles(const uint8_t shader_mask, const std::string& name,
	const std::string& file_name)
{
	if (ShaderFromFiles(shader_mask, name, file_name)) {
		return CreateShaderProgram(name, false);
	}
	printf("Failed to Create shader '%s'\n", name.c_str());
	return nullptr;
}

bool ShaderManager::ShaderFromText(ShaderTypes type, const std::string& name, const char* src)
{
	const GLuint shader = glCreateShader(static_cast<GLenum>(type));
	const GLchar* codeArray[] = { src };
	glShaderSource(shader, 1, codeArray, nullptr);
	// Compile
	glCompileShader(shader);
	// Check to confirm
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		printf("Shader compilation failed!\nSource: %s", name.c_str());
		// Get and print the info log
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			std::string log(logLen, ' ');
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, &log[0]);
			printf("Shader log: %s", log.c_str());
		}
		glDeleteShader(shader);
	}
	else {
		_shader_contents_map[name][GetShaderIndex(type)] = shader;
		return true;
	}
	return false;
}

bool ShaderManager::ShaderFromFile(const ShaderTypes type, const std::string& name, const std::string& file_name)
{
	// Get Shader Code so we can compile it
	// TODO : (James)
	const std::string shaderCode = IO::GetFileContents(file_name);
	return ShaderFromText(type, name, shaderCode.c_str());
}

GLint ShaderManager::GetProgramID(const char* program_name)
{
	return _program_id[program_name];
}

std::string ShaderManager::GetProgramName(const GLint program_id)
{
	return _program_name[program_id];
}

inline void ShaderManager::UseProgram(const char* program_name)
{
	glUseProgram(_program_id[program_name]);
}
inline void ShaderManager::UseProgram(const GLint program_id)
{
	glUseProgram(program_id);
}

bool ShaderManager::ShaderFromFiles(const uint8_t shader_mask, const std::string& name, const std::string& file_name)
{
	int shaders_loaded = 0;
	int index = 5;
	int stride = SHADER_MASK_MAX_STRIDE;
	// We loop through our possible shaders and load any that we've been asked to load
	while (stride > 0) {
		if (stride & shader_mask) {
			if (ShaderFromFile(SHADER_TYPES[index], name, file_name + "." + SHADER_FILE_EXT[index])) {
				shaders_loaded++;
				DEBUG_SUCCESS_LOG("ShaderManager::ShaderFromFiles() : Loaded {} Shader '{}' from file '{}'",
					SHADER_TYPE_NAMES[index],
					name,
					file_name + "." + SHADER_FILE_EXT[index]);
			}
		}
		stride >>= 1;
		index--;
	}
	if (shaders_loaded > 0)
		return true;
	return false;
}

Shader* ShaderManager::GetShader(const GLint program_id)
{
	std::unordered_map<int, std::string>::iterator i;
	if ((i = _program_name.find(program_id)) != _program_name.end()) {
		return GetShader(i->second);
	}
	throw std::runtime_error("ShaderManager::GetShader() : Shader '" + std::to_string(program_id) + "' not found");
}

Shader* ShaderManager::GetShader(const std::string& program_name) const
{
	if (const auto value = _shaders.find(program_name); value != _shaders.end())
		return value->second;
	throw std::runtime_error("ShaderManager::GetShader() : Shader '" + program_name + "' not found");
}

bool ShaderManager::SetDefaultShader(const std::string& shader_name)
{
	_default_shader_name = shader_name;
	if (_shaders.contains(shader_name))
		return true;
	return false;
}

void ShaderManager::SetDefaultShader(const Shader* shader)
{
	SetDefaultShader(shader->GetName());
}

Shader* ShaderManager::GetDefaultShader() const
{
	return GetShader(_default_shader_name);
}

constexpr int ShaderManager::GetShaderIndex(const ShaderTypes gl_shader_type)
{
	switch (static_cast<int>(gl_shader_type)) {
	case GL_VERTEX_SHADER:
		return 0;
	case GL_FRAGMENT_SHADER:
		return 1;
	case GL_GEOMETRY_SHADER:
		return 2;
	case GL_TESS_EVALUATION_SHADER:
		return 3;
	case GL_TESS_CONTROL_SHADER:
		return 4;
	case GL_COMPUTE_SHADER:
		return 5;
	default:
		throw std::runtime_error("[Shader Manager] Invalid shader type");
	}
}

constexpr ShaderMask ShaderManager::GetShaderMask(const ShaderTypes gl_shader_type)
{
	switch (static_cast<int>(gl_shader_type)) {
	case GL_VERTEX_SHADER:
		return ShaderMask::MVertex;
	case GL_FRAGMENT_SHADER:
		return ShaderMask::MFragment;
	case GL_GEOMETRY_SHADER:
		return ShaderMask::MGeometry;
	case GL_TESS_EVALUATION_SHADER:
		return ShaderMask::MTessEval;
	case GL_TESS_CONTROL_SHADER:
		return ShaderMask::MTessControl;
	case GL_COMPUTE_SHADER:
		return ShaderMask::MCompute;
	default:
		throw std::runtime_error("[Shader Manager] Invalid shader type");
	}
}
