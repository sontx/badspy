#include "queue.h"

void Queue::clear()
{
	delete linklist;
	linklist = new LinkList();
}

void Queue::push(const char * what)
{
	linklist->insert_tail(what);
}

const char * Queue::pop(char * buffer)
{
	return linklist->pop_head(buffer);
}

const char * Queue::peek(bool _new) const
{
	return linklist->peek(_new);
}

bool Queue::empty() const
{
	return linklist->empty();
}

Queue::Queue()
{
	linklist = new LinkList();
}

Queue::~Queue()
{
	delete linklist;
}
