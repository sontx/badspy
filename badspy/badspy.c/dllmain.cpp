#include "dllmain.h"

HINSTANCE hinst_dll = NULL;

EXPORT DWORD load_spy()
{
	return Spy::load(hinst_dll);
}

EXPORT VOID unload_spy()
{
	Spy::unload();
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	hinst_dll = hinstDLL;
	return TRUE;
}
