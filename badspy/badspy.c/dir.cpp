#include "dir.h"

bool Dir::ignore_dir()
{
	int ret = 1;
	while ((fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && ((ret = FindNextFileA(hfind, &fdata)) != 0));
	return ret != 0;
}

bool Dir::next(char * buffer)
{
	if (hfind == NULL)
	{
		if ((hfind = FindFirstFileA(_dir_path, &fdata)) == INVALID_HANDLE_VALUE)
			return false;
	}
	else if (FindNextFileA(hfind, &fdata) == 0)
	{
		return false;
	}
	if (ignore_dir())
		strcpy(buffer, fdata.cFileName);
	else
		return false;
	return true;
}

Dir::Dir(const char * dir_path)
{
	_dir_path = new char[strlen(dir_path) + 3];
	strcpy(_dir_path, dir_path);
	strcat(_dir_path, "\\*");
}

Dir::~Dir()
{
	delete[] _dir_path;
	if (hfind != INVALID_HANDLE_VALUE && hfind != NULL)
		FindClose(hfind);
}
