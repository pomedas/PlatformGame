#pragma once
#include <SDL3\SDL_timer.h>

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	Uint64 ReadTicks() const;

private:
	Uint64 startTime;
	Uint64 frequency;
};