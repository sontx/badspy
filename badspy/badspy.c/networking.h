#pragma once
#include "spy.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#define MAX_LOCAL_NAME_LEN 64

class Networking
{
	static void init_winsock();
	static void free_winsock();
public:
	static bool is_network_available();
	static byte * get_mac_addr(byte * buffer, in_addr ipv4);
};