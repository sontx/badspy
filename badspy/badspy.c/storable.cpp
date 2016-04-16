#include "storable.h"

void Storable::open(const char * mode)
{
	this->file_handle = fopen(file_path, mode);
}

void Storable::close()
{
	SAFE_FREE(file_handle);
}

void Storable::reopen(const char * mode)
{
	close();
	open(mode);
}

Storable::Storable(const char * file_path, const char * mode)
{
	this->file_path = strdup(file_path);
	this->open(mode);
}

Storable::~Storable()
{
	close();
}
