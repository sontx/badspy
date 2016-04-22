#include "kbhfile.h"

bool KBHFile::full() const
{
	return position >= SPY_TMP_KBH_MSZ;
}

KBHFile::KBHFile(const char * file_path)
	:Storage(file_path, "KB")
{
}
