#include "networking.h"

#pragma comment(lib , "iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")

static in_addr BIN_IPV4_ADDR_LOOPBACK = { 127, 0, 0, 1 };

void Networking::init_winsock()
{
	WSADATA ws_data;
	WSAStartup(MAKEWORD(2, 2), &ws_data);
}

void Networking::free_winsock()
{
	WSACleanup();
}

bool Networking::is_network_available()
{
	init_winsock();

	char hostname[MAX_LOCAL_NAME_LEN];
	if (gethostname(hostname, MAX_LOCAL_NAME_LEN - 1))
	{
		free_winsock();
		return false;
	}
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;

	addrinfo * ai;
	if (getaddrinfo(hostname, "10", &hints, &ai))
	{
		free_winsock();
		return false;
	}

	bool state = true;
	for (addrinfo * ptr = ai; ptr != NULL && state; ptr = ptr->ai_next)
	{
		if (ptr->ai_family == PF_INET)
		{
			if (memcmp(&(((SOCKADDR_IN *)(ptr->ai_addr))->sin_addr), &BIN_IPV4_ADDR_LOOPBACK, sizeof(BIN_IPV4_ADDR_LOOPBACK)) == 0)
				state = false;
		}
	}

	freeaddrinfo(ai);
	free_winsock();

	return state;
}

byte * Networking::get_mac_addr(byte * buffer, in_addr ipv4)
{
	init_winsock();
	ULONG mac_addr[2];
	ULONG phy_addr_len = 6;
	if (SendARP(ipv4.S_un.S_addr, 0, mac_addr, &phy_addr_len) == NO_ERROR)
	{
		if (phy_addr_len == 6)
		{
			memcpy(buffer, mac_addr, phy_addr_len);
			free_winsock();
			return buffer;
		}
	}
	free_winsock();
	return NULL;
}
