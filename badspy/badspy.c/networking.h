#pragma once
#include "spy.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX_LOCAL_NAME_LEN 64

typedef void(*WINSOCK_PROC)(void);

class Networking
{
	static bool network_state;
	static void work_with_winsock(WINSOCK_PROC proc);
public:
	static bool is_network_available();
};