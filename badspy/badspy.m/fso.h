#pragma once
#include <windows.h>

#ifdef WIN32
#define FILE_SEPARATOR				'\\'
#else
#define FILE_SEPARATOR				'/'
#endif

class FSO
{
public:
	static bool file_exists(const char * file_path);
	static bool dir_exists(const char * dir_path);
	static int make_dir(const char * dir_path);
	static void get_exe_path(char * buffer);
	static void get_dir_path(char * buffer);
	static void copy_file(const char * from, const char * to);
	static bool get_special_dir(int csidl, char * buffer);
	static bool run(const char * file_path, const char * args = NULL, int showflag = SW_HIDE);
	static bool _run(const char * file_path, const char * args = NULL, int showflag = SW_HIDE);
	static const char * path_combine(char * dst, const char * dir, const char * fname);
	static const char * get_parent(char * parent, const char * path);
};