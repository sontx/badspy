#include "socket.h"
#pragma comment(lib, "Ws2_32.lib")

void Socket::init_winsock() const
{
	WSADATA wsa_data;
	int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error != 0)
	{
		LOG_E("WSAStartup failed: %d\n", error);
		throw error;
	}
}

addrinfo Socket::init_addrinfo() const
{
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	return hints;
}

addrinfo * Socket::resolve_server(addrinfo * hints, const char * server_address, int server_port) const
{
	addrinfo * result;
	char s_port[10];
	itoa(server_port, s_port, 10);
	int error = getaddrinfo(server_address, s_port, hints, &result);
	if (error != 0) {
		LOG_E("getaddrinfo failed: %d", error);
		WSACleanup();
		throw error;
	}
	return result;
}

void Socket::connect_to_server(addrinfo * ai)
{
	int error;
	for (addrinfo * ptr = ai; ptr != NULL; ptr = ptr->ai_next)
	{
		client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (client_socket == INVALID_SOCKET)
		{
			error = WSAGetLastError();
			LOG_E("Socket failed with error: %ld", error);
			WSACleanup();
			throw error;
		}

		error = connect(client_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (error == SOCKET_ERROR) {
			closesocket(client_socket);
			client_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	if (client_socket == INVALID_SOCKET)
	{
		LOG_E("Unable to connect to server!");
		WSACleanup();
		throw 1;
	}
}

void Socket::close()
{
	if (client_socket != INVALID_SOCKET)
	{
		shutdown(client_socket, SD_BOTH);
		closesocket(client_socket);
		client_socket = INVALID_SOCKET;
		LOG_W("Socket closed");
	}
}

int Socket::available() const
{
	u_long bytes_available;
	int result = ioctlsocket(client_socket, FIONREAD, &bytes_available);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		LOG_E("Socket check avaiable failed: %d", error);
		throw error;
	}
	return bytes_available;
}

void Socket::write(const byte * buffer, int offset, int length)
{
	int result = send(client_socket, (const char *)buffer + offset, length, 0);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		LOG_E("Send failed: %d\n", error);
		throw error;
	}
}

int Socket::read(byte * buffer, int offset, int length)
{
	int result = recv(client_socket, (char *)buffer + offset, length, 0);
	if (result == 0)
	{
		LOG_W("Connection closed");
	}
	else
	{
		LOG_E("Receive failed: %d", result);
	}
	return result;
}

Socket::Socket(const char * server_address, int server_port)
{
	init_winsock();
	addrinfo hints = init_addrinfo();
	addrinfo * result = resolve_server(&hints, server_address, server_port);
	connect_to_server(result);
	freeaddrinfo(result);
}

Socket::~Socket()
{
	close();
	WSACleanup();
}
