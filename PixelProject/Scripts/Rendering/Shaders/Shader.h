#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <GL/glew.h>

class Shader
{
 public:
	Shader(GLuint program_id);
	Shader(GLuint program_id, const std::string& name);
	~Shader();

	void UseProgram() const;

	GLuint GetProgramID() const;
	GLuint GetUniformLocation(const std::string& uniform_name);
	std::string GetName() const;

 protected:
	static constexpr char DEFAULT_SHADER_NAME[] = "_UNNAMED_SHADER_";

 private:
	GLuint _program;
	std::string _name;
	std::unordered_map<std::string, GLuint> _uniform_locations;

};