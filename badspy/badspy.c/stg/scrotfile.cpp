#include "scrotfile.h"

ScrotFile::ScrotFile(const char * file_path)
	: Storage(file_path, "SS")
{
}