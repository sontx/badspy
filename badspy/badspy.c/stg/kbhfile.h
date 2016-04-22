#pragma once
#include "storage.h"

class KBHFile : public Storage
{
public:
	bool full() const;
	KBHFile(const char * file_path);
};