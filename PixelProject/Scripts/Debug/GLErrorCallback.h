#pragma once

#include <cstdio>

#include "Utility/Console.h"
#include "GL/glew.h"

class GLErrorCallback
{
 public:

	inline static bool colour_logs = false;
	inline static GLenum ignore_message_type = GL_DEBUG_SEVERITY_NOTIFICATION;

	static void LinkCallback(const bool use_colour = false, const GLenum ignore_below = GL_DEBUG_SEVERITY_NOTIFICATION);

	static void GLAPIENTRY MessageCallback(const GLenum source,
		const GLenum type,
		GLuint id,
		const GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* user_param);

	static std::string GetErrorName(const GLenum source);
	static std::string GetErrorType(const GLenum type);
	static std::string GetErrorSeverity(const GLenum severity);
	static ConsoleColour GetErrorColour(const GLenum severity);
};
