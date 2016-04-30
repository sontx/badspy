#pragma once
#include <windows.h>
#include "../../../log-cpp/log.h"

class Reg
{
private:
	HKEY hkey;
public:
	bool write_value_sz(const char * key, const char * value);
	bool read_value_sz(const char * key, char * buffer);
	Reg(HKEY root, const char * subkey, bool writable = true);
	~Reg();
};