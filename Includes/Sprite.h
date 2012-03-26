/*! 
 *  \brief     Sprite class.
 *  \details   This class is used to handle simple sprites.
 *  \author    Frank Luna, Mihai Popescu
 *  \date      24/08/2004
 *  \copyright GNU Public License.
 */
#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"
#include "Vec2.h"
#include "BackBuffer.h"

class Sprite
{
public:
	Sprite(int imageID, int maskID);
	Sprite(const char *szImageFile, const char *szMaskFile);
	Sprite(const char *szImageFile, COLORREF crTransparentColor);

	virtual ~Sprite();

	int GetWidth() { return myBitmap.bmWidth; }
	int GetHeight() { return myBitmap.bmHeight; }

	virtual void Update(float dt);
	virtual void Draw();

	void setBackBuffer(const BackBuffer *pBackBuffer);
	
public:
	// Keep these public because they need to be
	// modified externally frequently.
	Vec2 myPosition;
	Vec2 myVelocity;

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy Sprites. We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	void drawTransparent();
	void drawMask();

protected:
	const BackBuffer *myBackBuffer;

	HBITMAP myImage;
	HBITMAP myImageMask;
	BITMAP myBitmap;
	BITMAP myBitmapMask;
	COLORREF myTransparentColor;
	HDC mySpriteDC;

	int myFrameCropX;
	int myFrameCropY;
	int myFrameWidth;
	int myFrameHeight;
};

#endif // SPRITE_H