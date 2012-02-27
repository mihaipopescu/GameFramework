// Sprite.h
// By Frank Luna
// August 24, 2004.
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

	int width(){ return mImageBM.bmWidth; }
	int height(){ return mImageBM.bmHeight; }
	void update(float dt);

	void setBackBuffer(const BackBuffer *pBackBuffer);
	virtual void draw();

public:
	// Keep these public because they need to be
	// modified externally frequently.
	Vec2 mPosition;
	Vec2 mVelocity;

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy Sprites. We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	HBITMAP mhImage;
	HBITMAP mhMask;
	BITMAP mImageBM;
	BITMAP mMaskBM;

	HDC mhSpriteDC;
	const BackBuffer *mpBackBuffer;

	COLORREF mcTransparentColor;
	void drawTransparent();
	void drawMask();
};

// AnimatedSprite
// by Mihai Popescu
// April 5, 2008
class AnimatedSprite : public Sprite
{
public:
	//NOTE: The animation is on a single row.
	AnimatedSprite(const char *szImageFile, const char *szMaskFile, const RECT& rcFirstFrame, int iFrameCount);
	virtual ~AnimatedSprite() { }

public:
	void SetFrame(int iIndex);
	int GetFrameCount() { return miFrameCount; }

	virtual void draw();
	
protected:
	POINT mptFrameStartCrop;// first point of the frame (upper-left corner)
	POINT mptFrameCrop;		// crop point of frame
	int miFrameWidth;		// width
	int miFrameHeight;		// height
	int miFrameCount;		// number of frames
};



#endif // SPRITE_H

