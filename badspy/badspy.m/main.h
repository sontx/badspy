#pragma once
#include <windows.h>
#include "proc.h"

// a looper to keep app alive
void start_looper();
// load dll library
bool init_library();
// bind dll function which exported
bool init_dll_func();
// release dll libary, when exit
void free_library();

LOAD_SPY load_spy = NULL;
UNLOAD_SPY unload_spy = NULL;