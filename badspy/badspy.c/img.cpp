#include "img.h"
#include <ole2.h>
#include <olectl.h>

// reference from http://stackoverflow.com/questions/9524393/how-to-capture-part-of-the-screen-and-save-it-to-a-bmp

Screenshot::Screenshot(float scale, HookFile * hookfile)
{
	this->scale = scale;
	this->pHookFile = hookfile;
}

Screenshot::~Screenshot()
{
}

bool Screenshot::saveb_bitmap(HBITMAP bmp) const
{
	// contains parameters to create a picture object through the OleCreatePictureIndirect function. (MSDN)
	PICTDESC pictureDesc;
	pictureDesc.cbSizeofstruct = sizeof(PICTDESC);
	pictureDesc.picType = PICTYPE_BITMAP;
	pictureDesc.bmp.hbitmap = bmp;
	pictureDesc.bmp.hpal = NULL;

	// creates a new picture object initialized according to a PICTDESC structure. (MSDN)
	LPPICTURE picture;
	HRESULT res = OleCreatePictureIndirect(&pictureDesc, IID_IPicture, false, (void**)(&picture));

	if (!SUCCEEDED(res))
		return false;

	// creates a stream object that uses an HGLOBAL memory handle to store the stream contents. (MSDN)
	LPSTREAM stream;
	res = CreateStreamOnHGlobal(0, true, &stream);

	if (!SUCCEEDED(res))
	{
		picture->Release();
		return false;
	}

	// save picture content to stream which just was created above
	LONG bytes_streamed;
	res = picture->SaveAsFile(stream, true, &bytes_streamed);

	if (!SUCCEEDED(res))
	{
		stream->Release();
		picture->Release();
		return false;
	}

	// retrieves a pointer to first byte of stream memory
	HGLOBAL mem = 0;
	GetHGlobalFromStream(stream, &mem);// retrieves the global memory handle to a stream
	LPVOID data = GlobalLock(mem);// then get the first byte pointer of stream memory by call this api

	// write bitmap bytes to file in disk
	pHookFile->write(data, bytes_streamed);

	GlobalUnlock(mem);

	stream->Release();
	picture->Release();

	return true;
}

void Screenshot::capture_to_file() const
{
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMemoryDC = CreateCompatibleDC(hdcScreen);
	int width = GetDeviceCaps(hdcScreen, HORZRES);
	int height = GetDeviceCaps(hdcScreen, VERTRES);
	int scaleWidth = (int) (width * scale);
	int scaleHeight = (int) (height * scale);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, scaleWidth, scaleHeight);
	HGDIOBJ oldObject = SelectObject(hdcMemoryDC, hBitmap);
	bool ok = false;

	if (oldObject != NULL)
	{
		SetStretchBltMode(hdcMemoryDC, HALFTONE);
		if (!StretchBlt(
			hdcMemoryDC, 0, 0, scaleWidth, scaleHeight,
			hdcScreen, 0, 0, width, height, SRCCOPY))
			goto done;
	}

	hBitmap = (HBITMAP) SelectObject(hdcMemoryDC, oldObject);

	saveb_bitmap(hBitmap);

done:
	DeleteObject(hBitmap);
	DeleteDC(hdcScreen);
	DeleteDC(hdcMemoryDC);
}
