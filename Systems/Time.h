#pragma once
class Time
{
private:
	Time();
	~Time();

public:
	inline static Time* Get(void)   { assert(instance != NULL); return instance; }
	inline static void Create(void) { assert(instance == NULL); instance = new Time(); }
	inline static void Delete(void) { SAFE_DELETE(instance); }

	inline static bool Stopped(void) { return isTimerStopped; }
	inline static float Delta(void) { return isTimerStopped ? 0.0f : timeElapsed; }

	void Update(void);
	void Print(void);

	void Start(void);
	void Stop(void);

	inline float FPS(void) const { return framePerSecond; }
	inline float RunningTime(void) const { return runningTime; }

private:
	static Time* instance;
	static bool isTimerStopped;
	static float timeElapsed;

	INT64 frequency;
	INT64 currentCount;
	INT64 lastCount;
	INT64 lastFPSUpdate;
	INT64 fpsUpdateInterval;

	UINT frameCount;
	float runningTime;
	float framePerSecond;
};

