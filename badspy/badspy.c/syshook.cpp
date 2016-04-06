#include "syshook.h"
#include "factory.h"

HHOOK kbHookHandle = NULL;
HINSTANCE dllInstanceHandle = NULL;

// entry point for dll library when LoadLibrary/LoadLibraryEx was been called
BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	dllInstanceHandle = hinstDLL;// just store instance handle for register hook
	return TRUE;
}

// register global keyboard hook
inline HHOOK register_kb_hook()
{
	return SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) kb_hook_proc, dllInstanceHandle, 0);
}

inline VOID unregister_kb_hook()
{
	UnhookWindowsHookEx(kbHookHandle);
	kbHookHandle = NULL;
}

inline DWORD get_register_hook_error()
{
	return GetLastError();
}

EXPORT DWORD register_hook()
{
	if (kbHookHandle == NULL)
	{
		kbHookHandle = register_kb_hook();
		if (kbHookHandle == NULL)
			return get_register_hook_error();
	}
	return 0;		
}

EXPORT VOID unregister_hook()
{
	unregister_kb_hook();
}

// forward keyboard hook data for special function to proccess this data
// wParam is key state(down/up...?), lParam is key data(keycode,...)
EXPORT LRESULT CALLBACK kb_hook_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LOG("has keyboard hook data code %d", nCode);
	if (nCode == HC_ACTION)
		process_kb_hook_data(wParam, lParam);
	return CallNextHookEx(kbHookHandle, nCode, wParam, lParam);
}