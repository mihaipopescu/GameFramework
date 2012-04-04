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
#include "CGameObject.h"


class Sprite : public CGameObject
{
public:
	Sprite(int imageID, int maskID);
	Sprite(const char *szImageFile, const char *szMaskFile);
	Sprite(const char *szImageFile, COLORREF crTransparentColor);
	virtual ~Sprite();

	void Setup(HDC hdc);

	virtual void Draw(HDC hBackBufferDC) const;
	virtual int GetWidth() const { return myFrameWidth; }
	virtual int GetHeight() const { return myFrameHeight; }

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy Sprites. We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	void drawTransparent(HDC hBackBufferDC) const;
	void drawMask(HDC hBackBufferDC) const;

protected:
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