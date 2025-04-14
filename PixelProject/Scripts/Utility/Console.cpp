#include "Console.h"

#include <iostream>

void Console::PrintText(const std::string& message, const ConsoleColour colour)
{
	SetConsoleColour(colour);

	std::cout << message << std::endl;

	ResetConsoleColour();
}

#if defined(_WIN64)

HANDLE Console::GetHandle()
{
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	return handle;
}

#endif

void Console::SetConsoleColour(const ConsoleColour colour)
{
#if defined(_WIN64)
	SetConsoleTextAttribute(GetHandle(), colour);
#endif
}

void Console::ResetConsoleColour()
{
#if defined(_WIN64)
	SetConsoleTextAttribute(GetHandle(), DEFAULT_COLOUR);
#endif
}
