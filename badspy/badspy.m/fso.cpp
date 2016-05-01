#include "fso.h"
#include <Shlobj.h>

bool FSO::file_exists(const char * file_path)
{
	WIN32_FIND_DATAA fdata;
	HANDLE handle = FindFirstFileA(file_path, &fdata);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindClose(handle);
		return true;
	}
	return false;
}

bool FSO::dir_exists(const char * dir_path)
{
	DWORD attr = GetFileAttributesA(dir_path);
	return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

int FSO::make_dir(const char * dir_path)
{
	return SHCreateDirectoryExA(NULL, dir_path, NULL);
}

void FSO::get_exe_path(char * buffer)
{
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
}

void FSO::get_dir_path(char * buffer)
{
	get_exe_path(buffer);
	char * last_file_separator = strrchr(buffer, '\\');
	*(last_file_separator + 1) = '\0';
}

void FSO::copy_file(const char * from, const char * to)
{
	CopyFileA(from, to, false);
}

bool FSO::get_special_dir(int csidl, char * buffer)
{
	return SUCCEEDED(SHGetFolderPathA(NULL, csidl, NULL, 0, buffer));
}

bool FSO::run(const char * file_path, const char * args, int showflag)
{
	return (int)ShellExecuteA(NULL, "open", file_path, args, NULL, showflag) > 32;
}

bool FSO::_run(const char * file_path, const char * args, int showflag)
{
	char * file_dir = new char[MAX_PATH];
	bool ret = (int)ShellExecuteA(NULL, "open", file_path, args, get_parent(file_dir, file_path), showflag) > 32;
	delete[] file_dir;
	return ret;
}

const char * FSO::path_combine(char * dst, const char * dir, const char * fname)
{
	strcpy(dst, dir);
	int last_index = strlen(dir) - 1;
	if (dir[last_index] != FILE_SEPARATOR)
		dst[++last_index] = FILE_SEPARATOR;
	strcpy(dst + last_index + 1, fname);
	return dst;
}

const char * FSO::get_parent(char * parent, const char * path)
{
	const char * last_file_separator = strrchr(path, '\\');
	int count = last_file_separator - path;
	strncpy(parent, path, count);
	parent[count] = '\0';
	return parent;
}
