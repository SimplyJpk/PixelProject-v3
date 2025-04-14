#include "GLErrorCallback.h"

void GLErrorCallback::LinkCallback(const bool use_colour, const GLenum ignore_below)
{
	colour_logs = use_colour;
	ignore_message_type = ignore_below;
	// GL_DEBUG_OUTPUT - much faster
	// GL_DEBUG_OUTPUT_SYNCHRONOUS - can be stacktraced
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);
}

void GLErrorCallback::MessageCallback(const GLenum source, const GLenum type, GLuint id, const GLenum severity,
	GLsizei length, const GLchar* message, const void* user_param)
{
	if (severity <= ignore_message_type)
		return;

	const std::string errorSource = GetErrorName(source);
	const std::string errorType = GetErrorType(type);
	const std::string errorSeverity = GetErrorSeverity(severity);

	static char errorMessage[400];

	sprintf_s(errorMessage,
		sizeof(errorMessage),
		"GL CALLBACK: %s type = %s, severity = %s, message = %s\n",
		errorSource.c_str(),
		errorType.c_str(),
		errorSeverity.c_str(),
		message);
	if (colour_logs)
		Console::PrintText(errorMessage, GetErrorColour(severity));
	else
		Console::PrintInfo(errorMessage);
}

std::string GLErrorCallback::GetErrorName(const GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "WINDOW SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "SHADER COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "THIRD PARTY";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "APPLICATION";

	case GL_DEBUG_SOURCE_OTHER:
	default:
		return "UNKNOWN";
	}
}

std::string GLErrorCallback::GetErrorType(const GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "DEPRECATED BEHAVIOUR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "UNDEFINED BEHAVIOUR";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "PERFORMANCE";
	case GL_DEBUG_TYPE_OTHER:
		return "OTHER";
	case GL_DEBUG_TYPE_MARKER:
		return "MARKER";
	default:
		return "UNKNOWN";
	}
}

std::string GLErrorCallback::GetErrorSeverity(const GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "NOTIFICATION";
	case GL_DEBUG_SEVERITY_LOW:
		return "LOW";
	case GL_DEBUG_SEVERITY_MEDIUM:
		return "MEDIUM";
	case GL_DEBUG_SEVERITY_HIGH:
		return "HIGH";
	default:
		return "UNKNOWN";
	}
}

ConsoleColour GLErrorCallback::GetErrorColour(const GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return ConsoleColour::GREY;
	case GL_DEBUG_SEVERITY_LOW:
		return ConsoleColour::LIGHT_YELLOW;
	case GL_DEBUG_SEVERITY_MEDIUM:
		return ConsoleColour::YELLOW;
	case GL_DEBUG_SEVERITY_HIGH:
		return ConsoleColour::RED;
	default:
		return ConsoleColour::LIGHT_RED;
	}
}
