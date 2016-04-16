#pragma once
#include "linklist.h"

class Queue
{
	LinkList * linklist = NULL;
public:
	void clear();
	void push(const char * what);
	const char * pop(char * buffer);
	const char * peek(bool _new = false) const;
	bool empty() const;
	Queue();
	~Queue();
};