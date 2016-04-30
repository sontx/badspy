#pragma once
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define SPY_BDR_OPT_FILENAME "opt"
#define SPY_BDR_OPT_MAX_PACK 254

#pragma pack(1)

struct Pack
{
	byte id;
};


class Opt
{
	FILE * opt_file = NULL;
	Pack * packs = NULL;
	int n_pack = 0;
	void init_packs();
	void import_from_file();
	void export_to_file() const;
public:
	void push(const Pack * pack);
	const Pack * get(int index) const;
	int get_count() const;
	Opt(const char * working_dir);
	~Opt();
};