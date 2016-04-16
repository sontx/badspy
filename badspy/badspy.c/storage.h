#pragma once
#include "spy.h"
#include "storable.h"

class Storage: public Storable
{
	void push_to_stash() const;
protected:
	unsigned int position = 0;
public:
	unsigned int get_position() const { return position; }
	void write(const void * chunk, size_t n);
	Storage(const char * file_path, const char * magic);
	virtual ~Storage();
};