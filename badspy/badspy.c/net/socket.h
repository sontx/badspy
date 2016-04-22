#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "../spy.h"

class Socket
{
	SOCKET client_socket = INVALID_SOCKET;
	void init_winsock() const;
	addrinfo init_addrinfo() const;
	addrinfo * resolve_server(addrinfo * hints, const char * server_address, int server_port) const;
	void connect_to_server(addrinfo * ai);
	void close();
public:
	sockaddr get_sock_addr() const;
	int available() const;
	void write(const byte * buffer, int offset, int length);
	int read(byte * buffer, int offset, int length);
	Socket(const char * server_address, int server_port);
	~Socket();
};