#pragma once
#include <windows.h>

/* just define function pointers which has been exported in dll libary */

typedef DWORD (* REGISTER_HOOK_PROC)(VOID);
typedef VOID (* UNREGISTER_HOOK_PROC)(VOID);
typedef LRESULT (* KB_HOOK_PROC)(int, WPARAM, LPARAM);
typedef VOID (* QUIT_PROC)(VOID);
typedef BOOL (* INIT_STASH_PROC)(const char *);
typedef VOID (*TAKE_SCREENSHOT_PROC)(VOID);