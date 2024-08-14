// ----------------------------------------------------
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"

PerfTimer::PerfTimer()
{
	Start();
}

void PerfTimer::Start()
{
	frequency = SDL_GetPerformanceFrequency();
	startTime = SDL_GetPerformanceCounter();
}

double PerfTimer::ReadMs() const
{
	return ((double)(SDL_GetPerformanceCounter() - startTime) / frequency * 1000);
}

Uint64 PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - startTime;
}
