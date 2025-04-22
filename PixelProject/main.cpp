#include <GL/glew.h>
#include <SDL3/SDL.h>

#include <iostream>

#include "Game.h"
#include "Config/GameSettings.h"
#include "Config/ConfigFile.h"
#include "Utility/Console.h"
#include "Utility/FileIO.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Scripts/Debug/GLErrorCallback.h"

#include <version>

void PrintInGameCommands();
bool CreateWindowAndContext();

SDL_GLContext g_context;
SDL_Window* g_window;

std::unique_ptr<ConfigFile> g_config;

int main(int argc, char* argv[])
{
   bool success = true;

   // Create directory for data
   PixelProject::IO::CreateDirIfNotExists("data");

   Console::PrintInfo("Loading configuration file");

   g_config = std::make_unique<ConfigFile>();

   if (!CreateWindowAndContext())
      success = false;

   // Once window is prepared, load game settings
   auto gameSettings = std::make_shared<GameSettings>();
   gameSettings->LoadSettings(g_config);
   Game game(&g_context, g_window, gameSettings);
   // TODO: (James) Do Game
   if (!success || !game.Initialize())
   {
      Console::PrintError("PixelProject failed to initialize!\n");
      success = false;
   }

   GLErrorCallback::LinkCallback(true);
   game.Run();

   SDL_DestroyWindow(g_window);
   SDL_Quit();
   return 0;
}

bool CreateWindowAndContext()
{
   bool success = true;

   SDL_Init(SDL_INIT_VIDEO);

   const char* glsl_version = "#version 130";
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
   SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
   constexpr auto windowFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
   g_window = SDL_CreateWindow("Pixel Project v2", g_config.get()->window_size_x, g_config.get()->window_size_y, windowFlags);
   if (g_window == nullptr)
   {
      Console::PrintError(std::string("Window could not be created!\nSDL Err: %s\n", SDL_GetError()));
      success = false;
   }

   g_context = SDL_GL_CreateContext(g_window);
   if (g_context == nullptr)
   {
      Console::PrintError(std::string("OpenGL context could not be created! SDL Err: %s\n", SDL_GetError()));
      success = false;
   }
   else
   {
      const GLenum err = glewInit();
      if (GLEW_OK != err)
      {
         // Convert GLubyte * to char *
         const auto errStr = reinterpret_cast<const char*>(glewGetErrorString(err));
         Console::PrintError(std::string("GLEW failed to initialize! GLEW Err: %s\n", errStr));
         success = false;
      }
      else
      {
         Console::PrintSuccess("OpenGL context created successfully!\n");
      }

      if (glewIsSupported("GL_ARB_gpu_shader_int64")) {
         Console::PrintSuccess("GL_ARB_gpu_shader_int64 is supported!\n");
      } else {
         Console::PrintWarning("GL_ARB_gpu_shader_int64 is not supported by your GPU, rip.\n");
      }

      SDL_GL_MakeCurrent(g_window, g_context);
      if (SDL_GL_SetSwapInterval(1) == false)
      {
         Console::PrintWarning(std::string("Unable to set VSync! SDL Error: %s\n", SDL_GetError()));
      }
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   }
   return success;
}
