#include "stdafx.h"
#include "Time.h"

Time* Time::instance = NULL;
bool  Time::isTimerStopped = true;
float Time::timeElapsed = 0.0f;


Time::Time()
	: frequency(0)
	, currentCount(0)
	, lastCount(0)
	, lastFPSUpdate(0)
	, fpsUpdateInterval(0)
	, frameCount(0)
	, runningTime(0.0f)
	, framePerSecond(0.0f)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	fpsUpdateInterval = frequency >> 1;
}


Time::~Time()
{
}


void Time::Update(void)
{
	if (isTimerStopped) return;

	//1. 현재시간을 가져와 시간 간격 및 진행시간을 계산한다.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
	timeElapsed = (float)(currentCount - lastCount) / (float)frequency;

	while (timeElapsed < 1 / 60.0f)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
		timeElapsed = (float)(currentCount - lastCount) / (float)frequency;

	}
	runningTime += timeElapsed;

	//2. FPS Update
	frameCount++;

	//if (currentCount - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = (float)currentCount / (float)frequency;
		float tempLastTime = (float)lastFPSUpdate / (float)frequency;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = (INT64)currentCount;
		//cout << "Time::Update()->frameCount : " << frameCount << ", timeElpased : " << timeElapsed << endl;
		frameCount = 0;
	}

	lastCount = currentCount;
}

void Time::Print(void)
{
}

void Time::Start(void)
{
	assert(isTimerStopped);

	QueryPerformanceCounter((LARGE_INTEGER*)&lastCount);
	isTimerStopped = false;
}

void Time::Stop(void)
{
	assert(!isTimerStopped);

	INT64 stopCount = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopCount);
	runningTime += (float)(stopCount - lastCount) / (float)frequency;
	isTimerStopped = true;
}
