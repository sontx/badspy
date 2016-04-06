#pragma once
#include "spy.h"
#include "io.h"

class Screenshot
{
	float scale;
	HookFile * pHookFile;
	bool saveb_bitmap(HBITMAP bmp) const;
public:
	Screenshot(float scale, HookFile * hookfile);
	~Screenshot();
	void capture_to_file() const;
};