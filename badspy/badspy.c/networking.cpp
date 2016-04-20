#include "networking.h"
#pragma comment(lib, "Ws2_32.lib")

static in_addr BIN_IPV4_ADDR_LOOPBACK = { 127, 0, 0, 1 };

bool Networking::network_state = false;

void Networking::work_with_winsock(WINSOCK_PROC proc)
{
	WSADATA ws_data;
	WSAStartup(MAKEWORD(2, 2), &ws_data);
	proc();
	WSACleanup();
}

bool Networking::is_network_available()
{
	work_with_winsock([] 
	{
		network_state = false;
		char hostname[MAX_LOCAL_NAME_LEN];
		if (gethostname(hostname, MAX_LOCAL_NAME_LEN - 1))
			return;
		
		addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;

		addrinfo * ai;
		if (getaddrinfo(hostname, "10", &hints, &ai))
			return;

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

		network_state = state;
	});

	return network_state;
}
