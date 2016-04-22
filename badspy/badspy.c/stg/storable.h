#pragma once
#include "../spy.h"

class Storable
{
	void open(const char * mode);
	void close();
protected:
	void reopen(const char * mode);
	FILE * file_handle;
	char * file_path;
public:
	Storable(const char * file_path, const char * mode);
	virtual ~Storable();
};
