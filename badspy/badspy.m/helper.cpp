#include <stdio.h>
#include "helper.h"

void start_looper()
{
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}