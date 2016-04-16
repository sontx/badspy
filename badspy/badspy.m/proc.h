#pragma once
#include <windows.h>

/* just define function pointers which has been exported in dll libary */

typedef DWORD (* LOAD_SPY)(VOID);
typedef VOID (* UNLOAD_SPY)(VOID);
typedef VOID(*TAKE_SCROT)(VOID);