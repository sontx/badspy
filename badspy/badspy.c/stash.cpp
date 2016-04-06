#include <stdio.h>
#include "stash.h"

FILE * stashFileHandle = NULL;

EXPORT BOOL init_stash(const char * filePath)
{
	stashFileHandle = fopen(filePath, "a+");
	return stashFileHandle != NULL;
}

VOID push_file(const char * filePath)
{
	LOG("stash >> push file '%s'", filePath);
}

BOOL is_empty()
{
	return FALSE;
}

VOID pop_file(const char * buffer)
{
	LOG("stash >> pop file");
}

VOID close_stash()
{
	LOG("stash >> closed");
}