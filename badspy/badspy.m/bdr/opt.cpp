#include "opt.h"
#include "../proc.h"

extern PATH_COMBINE path_combine;

void Opt::init_packs()
{
	n_pack = 0;
	packs = new Pack[SPY_BDR_OPT_MAX_PACK];
}

void Opt::import_from_file()
{
	init_packs();
	while (!feof(opt_file) && n_pack < SPY_BDR_OPT_MAX_PACK)
	{
		if (fread(&packs[n_pack], sizeof(Pack), 1, opt_file) != 1)
			break;
		n_pack++;
	}
}

void Opt::export_to_file() const
{
	fwrite(packs, sizeof(Pack), n_pack, opt_file);
}

void Opt::push(const Pack * pack)
{
	if (packs == NULL)
		init_packs();
	packs[n_pack++] = *pack;
}

const Pack * Opt::get(int index) const
{
	return &packs[index];
}

int Opt::get_count() const
{
	return n_pack;
}

Opt::Opt(const char * working_dir)
{
	char * file_path = new char[MAX_PATH];
	path_combine(file_path, working_dir, SPY_BDR_OPT_FILENAME);
	
	WIN32_FIND_DATAA fdata;
	HANDLE handle = FindFirstFileA(file_path, &fdata);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindClose(handle);
		opt_file = fopen(file_path, "rb");
		import_from_file();
		fclose(opt_file);
	}

	opt_file = fopen(file_path, "wb");
	delete[] file_path;
}

Opt::~Opt()
{
	if (opt_file != NULL)
	{
		export_to_file();
		fclose(opt_file);
	}
	delete[] packs;
}
