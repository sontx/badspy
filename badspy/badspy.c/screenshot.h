#pragma once
#include "scrotfile.h"

class Screenshot
{
	float scale;
	ScrotFile * scrotFile;
	bool save_bitmap(HBITMAP bmp) const;
public:
	Screenshot(float scale, const char * file_path);
	~Screenshot();
	void capture_to_file() const;
};