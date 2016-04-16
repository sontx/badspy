#include "storage.h"
#include "stash.h"

void Storage::push_to_stash() const
{
	Spy::get_stash()->push(file_path);
}

void Storage::write(const void * chunk, size_t n)
{
	LOG("Write %d element to storage file", n);
	fwrite(chunk, sizeof(byte), n, file_handle);
#ifdef SPY_AUTO_FFLUSH
	fflush(file_handle);
	LOG("Flush data!");
#endif
	position += n;
}

Storage::Storage(const char * file_path, const char * magic)
	:Storable(file_path, "w+b")
{
	this->write(magic, 2);
	this->position = 0;
}

Storage::~Storage()
{
	if (position > 2)
		push_to_stash();
}
