#ifndef _TIMERCLASS_H_
#define _TIMERCLASS_H_

#include <windows.h>

class FTimer
{
public:
	FTimer();
	FTimer(const FTimer&);
	~FTimer();

	bool Initialize();
	void Frame();

	float GetTime();
private:
	INT64 Frequency;
	float TicksPerMs;
	INT64 StartTime;
	float FrameTime;
};

#endif