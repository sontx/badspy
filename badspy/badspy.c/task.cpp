#include "task.h"

void Task::run(Runnable * runnable)
{
	DWORD thread_id;
	CreateThread(NULL, 0, [](LPVOID p)->DWORD 
	{
		((Runnable *)p)->run(); 
		return 0; 
	}, runnable, 0, &thread_id);
}
