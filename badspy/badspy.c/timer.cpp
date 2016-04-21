#include "timer.h"

void Timer::start_timer_loop()
{
	pending_stop = false;
	running = true;
	DWORD startMillis = GetTickCount();
	do
	{
		Sleep(100);
		DWORD currentMillis = GetTickCount();
		if (currentMillis < startMillis)
		{
			startMillis = currentMillis;
		}
		else if (currentMillis - startMillis >= interval)
		{
			if (callback != NULL)
				callback();
			startMillis = currentMillis;
		}
	} while (!pending_stop);
	running = false;
}

void Timer::start()
{
	if (!running)
	{
		DWORD thread_id;
		thread_handle = CreateThread(NULL, 0, [](LPVOID p)-> DWORD 
		{
			((Timer*)p)->start_timer_loop();
			return 0;
		}, this, 0, &thread_id);
		SetThreadPriority(thread_handle, THREAD_PRIORITY_LOWEST);
	}
}

void Timer::stop(bool force)
{
	pending_stop = true;
	if (force && thread_handle != NULL && running)
	{
		TerminateThread(thread_handle, 0);
		CloseHandle(thread_handle);
		thread_handle = NULL;
		running = false;
	}
}

Timer::Timer(DWORD interval, timer_callback callback)
{
	this->interval = interval;
	this->callback = callback;
}

Timer::~Timer()
{
	if (!pending_stop)
		stop(true);
}
