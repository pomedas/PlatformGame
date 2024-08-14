#pragma once

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	int ReadSec() const;
	float ReadMSec() const;

private:
	int startTime;
};