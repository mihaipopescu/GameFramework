// BackBuffer.h
// By Frank Luna
// August 24, 2004.
#ifndef BACKBUFFER_H
#define BACKBUFFER_H
#include "main.h"

class BackBuffer
{
public:
	BackBuffer(HWND hWnd, int width, int height);
	~BackBuffer();

	void present();
	void reset();

	HDC getDC() const { return mhDC; }
	HWND getHWND() const { return mhWnd; }

	int width() const { return mWidth; }
	int height() const { return mHeight; }

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy BackBuffers. We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	// In addition, most applications will probably only need one
	// BackBuffer anyway.

	BackBuffer(const BackBuffer& rhs);
	BackBuffer& operator=(const BackBuffer& rhs);

private:
	HWND mhWnd;
	HDC mhDC;
	HBITMAP mhSurface;
	HBITMAP mhOldObject;
	int mWidth;
	int mHeight;
};
#endif // BACKBUFFER_H