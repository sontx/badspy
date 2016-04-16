#include "linklist.h"
#include "spy.h"

Node::~Node()
{
	delete[] data;
}

void LinkList::insert_tail(const char * data)
{
	Node * node = new Node();
	node->data = _strdup(data);
	node->next = NULL;
	if (tail != NULL)
	{
		tail->next = node;
		tail = node;
	}
	else
	{
		head = node;
		tail = node;
	}
}

const char * LinkList::pop_head(char * buffer)
{
	if (!empty())
	{
		Node * node = head;
		strcpy(buffer, node->data);
		head = head->next;
		delete node;
		return buffer;
	}
	return NULL;
}

const char * LinkList::peek(bool _new) const
{
	static Node * iterator = head;
	iterator = _new ? head : iterator->next;
	return iterator != NULL ? iterator->data : NULL;
}

bool LinkList::empty() const
{
	return head == NULL;
}

LinkList::~LinkList()
{
	while (head != NULL)
	{
		Node * temp = head;
		head = head->next;
		delete temp;
	}
}
