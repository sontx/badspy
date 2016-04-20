#include "storage.h"

unsigned int Storage::get_current_time_seconds()
{
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	// h[5] m[6] s[6] d[5] M[4] y[6] (last two numbers)
	return stime.wHour | (stime.wMinute << 5) | (stime.wSecond << 11) |
		(stime.wDay << 17) | (stime.wMonth << 22) | ((stime.wYear % 2000) << 26);
}

void Storage::write(const void * chunk, size_t n)
{
	//LOG("Write %d element to storage file", n);
	fwrite(chunk, sizeof(byte), n, file_handle);
#ifdef SPY_AUTO_FFLUSH
	fflush(file_handle);
	//LOG("Flush data!");
#endif
	position += n;
}

Storage::Storage(const char * file_path, const char * magic)
	:Storable(file_path, "w+b")
{
	this->write(magic, 2);
	unsigned int seconds = get_current_time_seconds();
	this->write(&seconds, sizeof(unsigned int));
	this->position = 0;
}

Storage::~Storage()
{
	if (position > 0)
		Spy::notify_upload();
}
