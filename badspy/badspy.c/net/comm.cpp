#include "comm.h"

void Comm::send_header(byte content_type, int content_length) const
{
	LOG_D("Send header: type = %d, length = %d", content_type, content_length);
	socket->write(&content_type, 0, sizeof(byte));
	socket->write((const byte *)&content_length, 0, sizeof(int));
}

void Comm::send_content(const byte * buffer, int offset, int length) const
{
	LOG_D("Send %d content bytes...", length);
	socket->write(buffer, offset, length);
}

void Comm::send_flag(byte flag) const
{
	LOG_D("Send flag: %d", flag);
	socket->write(&flag, 0, 1);
}

void Comm::receive_header(byte * content_type, int * content_length) const
{
	socket->read(content_type, 0, 1);
	socket->read((byte *)content_length, 0, 4);
}

int Comm::receive_content(byte * buffer, int offset, int length) const
{
	return socket->read(buffer, offset, length);
}

void Comm::receive_nbytes(byte * buffer, int offset, int length) const
{
	int received_bytes = 0;
	do
	{
		int chunk = socket->read(buffer, offset + received_bytes, length - received_bytes);
		received_bytes += chunk;
	} while (received_bytes < length);
}

int Comm::receive_flag() const
{
	byte ret;
	socket->read(&ret, 0, 1);
	return ret;
}

in_addr Comm::get_local_address() const
{
	sockaddr sock_addr = socket->get_sock_addr();
	in_addr addr;
	addr.S_un.S_addr = inet_addr(sock_addr.sa_data);
	return addr;
}

Comm::Comm(const char * server_addr, int server_port)
{
	LOG_I("Connect to %s:%d", server_addr, server_port);
	socket = new Socket(server_addr, server_port);
	LOG_I("Connected");
}

Comm::~Comm()
{
	delete socket;
}
