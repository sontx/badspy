#pragma once
#include "spy.h"
#include "storable.h"
#include "queue.h"

class Stash: public Storable
{
	Queue * stg_queue;
	void import_stash();
	void export_stash();
public:
	void push(const char * file_path);
	bool empty() const;
	const char * pop(char * buffer);
	Stash(const char * file_path);
	virtual ~Stash();
};