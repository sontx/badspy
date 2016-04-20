#pragma once
#include "spy.h"

class Dir
{
	char * _dir_path;
	WIN32_FIND_DATAA fdata;
	HANDLE hfind = NULL;
	bool ignore_dir();
public:
	bool next(char * buffer);
	Dir(const char * dir_path);
	~Dir();
};