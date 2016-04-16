#pragma once
#include "storage.h"

class ScrotFile : public Storage
{
public:
	ScrotFile(const char * file_path);
};