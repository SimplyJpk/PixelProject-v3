#include "MemoryUsage.h"

#ifdef __linux__

#include <unistd.h>
#include <sys/resource.h>
#include <SDL_timer.h>

int MemoryUsage::GetMemory(const bool use_accurate)
{
   // Get the memory usage details for this process
   struct rusage usage;
   getrusage(RUSAGE_SELF, &usage);
   _current_frame_memory = static_cast<int>(usage.ru_maxrss) / (use_accurate ? 1 : 1024);

   const auto sdlTicks = SDL_GetTicks();
   if (sdlTicks - _last_check_time > 2000)
   {
	  _reference_frame_memory = _current_frame_memory;
	  _last_check_time = sdlTicks;
   }

   return _current_frame_memory * 1024;
}

#endif