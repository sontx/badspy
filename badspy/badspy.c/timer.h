#pragma once
#include "spy.h"

typedef void(*timer_callback)(void);

class Timer
{
	HANDLE thread_handle;
	DWORD interval = 0;
	timer_callback callback;
	bool running = false;
	bool pending_stop = false;
	void start_timer_loop();
public:
	void start();
	void stop(bool force = false);
	Timer(DWORD interval, timer_callback callback);
	~Timer();
};