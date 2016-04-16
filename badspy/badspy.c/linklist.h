#pragma once
#include <stdio.h>
#include <string>

struct Node
{
	char * data;
	Node * next;
	~Node();
};

class LinkList
{
	Node * head = NULL;
	Node * tail = NULL;
public:
	void insert_tail(const char * data);
	const char * pop_head(char * buffer);
	const char * peek(bool _new = false) const;
	bool empty() const;
	~LinkList();
};