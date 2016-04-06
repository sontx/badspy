#pragma once
#include <windows.h>
#include "proc.h"

// a looper to keep app alive
void start_looper();
// load dll library
bool init_library();
// bind dll function which exported
bool init_dll_func();
// start hook
bool init_hook();
// release dll libary, when exit
void free_library();

REGISTER_HOOK_PROC register_hook = NULL;
UNREGISTER_HOOK_PROC unregister_hook = NULL;
KB_HOOK_PROC kb_hook_proc = NULL;
QUIT_PROC quit = NULL;
INIT_STASH_PROC init_stash = NULL;
TAKE_SCREENSHOT_PROC take_screenshot = NULL;