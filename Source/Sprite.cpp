//-----------------------------------------------------------------------------
// File: Sprite.cpp
//
// Desc: Class used for rendering and handling sprites
//
// Author: Mihai Popescu
//-----------------------------------------------------------------------------

#include "Sprite.h"
#include "CGameApp.h"


extern HINSTANCE g_hInst;

#define INVALID_TRANSPARENT_COLOR RGB(0xBA, 0xDB, 0xAD)

Sprite::Sprite(int imageID, int maskID)
{
    Init();

	// Load the bitmap resources.
	myImage = LoadBitmap(g_hInst, MAKEINTRESOURCE(imageID));
	myImageMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(maskID));

	// Get the BITMAP structure for each of the bitmaps.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);
	GetObject(myImageMask, sizeof(BITMAP), &myBitmapMask);

	// Image and Mask should be the same dimensions.
	assert(myBitmap.bmWidth == myBitmapMask.bmWidth);
	assert(myBitmap.bmHeight == myBitmapMask.bmHeight);

    drawInternal = &Sprite::drawMask;
}

Sprite::Sprite(const char *szImageFile)
{
    Init();

    myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

    // Get the BITMAP structure for the bitmap.
    GetObject(myImage, sizeof(BITMAP), &myBitmap);

    drawInternal = &Sprite::drawBitmap;
}

Sprite::Sprite(const char *szImageFile, const char *szMaskFile)
{
    Init();

	myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	myImageMask = (HBITMAP)LoadImage(g_hInst, szMaskFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// Get the BITMAP structure for each of the bitmaps.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);
	GetObject(myImageMask, sizeof(BITMAP), &myBitmapMask);

	// Image and Mask should be the same dimensions.
	assert(myBitmap.bmWidth == myBitmapMask.bmWidth);
	assert(myBitmap.bmHeight == myBitmapMask.bmHeight);

    drawInternal = &Sprite::drawMask;
}

Sprite::Sprite(const char *szImageFile, COLORREF crTransparentColor)
{
    Init();

	myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    mySpriteMaskDC = CreateCompatibleDC(myBackBufferDC);
	myTransparentColor = crTransparentColor;

	// Get the BITMAP structure for the bitmap.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);

    // create the mask
    myImageMask = CreateBitmap(myBitmap.bmWidth, myBitmap.bmHeight, 1, 1, NULL);

    drawInternal = &Sprite::drawTransparent;
}

Sprite::Sprite( const char *szImageFile, unsigned char uAlpha )
{
    Init();

    myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    myAlpha = uAlpha;

    // Get the BITMAP structure for the bitmap.
    GetObject(myImage, sizeof(BITMAP), &myBitmap);

    drawInternal = &Sprite::drawAlphaBlend;
}

Sprite::~Sprite()
{
	// Free the resources we created in the constructor.
	DeleteObject(myImage);
	DeleteObject(myImageMask);

	DeleteDC(mySpriteDC);
    DeleteDC(mySpriteMaskDC);
}

void Sprite::Init()
{
    myBackBufferDC = CGameApp::Get()->GetBackBufferDC();
    myImage = 0;
    myImageMask = 0;
    memset(&myBitmap, 0, sizeof(BITMAP));
    memset(&myBitmapMask, 0, sizeof(BITMAP));
    mySpriteDC = CreateCompatibleDC(myBackBufferDC);
    mySpriteMaskDC = 0;
    myAlpha = -1;
    myTransparentColor = INVALID_TRANSPARENT_COLOR;

    drawInternal = NULL;
}

void Sprite::Draw() const
{
	DrawWithOffset(0, 0);
}

void Sprite::DrawWithOffset(int dx, int dy) const
{
    (this->*drawInternal)(dx, dy);
}

void Sprite::drawMask(int dx, int dy) const
{
	// The position BitBlt wants is not the sprite's center
	// position; rather, it wants the upper-left position,
	// so compute that.
    int w = GetWidth();
    int h = GetHeight();
    int cx = GetFrameCropX();
    int cy = GetFrameCropY();

	int x = (int)myPosition.x - (w / 2);
	int y = (int)myPosition.y - (h / 2);

	// Note: For this masking technique to work, it is assumed
	// the backbuffer bitmap has been cleared to some
	// non-zero value.
	// Select the mask bitmap.
	HGDIOBJ oldObj = SelectObject(mySpriteDC, myImageMask);

	// Draw the mask to the backbuffer with SRCAND. This
	// only draws the black pixels in the mask to the backbuffer,
	// thereby marking the pixels we want to draw the sprite
	// image onto.
	BitBlt(myBackBufferDC, x+dx, y+dx, w, h, mySpriteDC, cx, cy, SRCAND);

	// Now select the image bitmap.
	SelectObject(mySpriteDC, myImage);

	// Draw the image to the backbuffer with SRCPAINT. This
	// will only draw the image onto the pixels that where previously
	// marked black by the mask.
	BitBlt(myBackBufferDC, x+dx, y+dx, w, h, mySpriteDC, cx, cy, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(mySpriteDC, oldObj);
}

void Sprite::drawTransparent(int dx, int dy) const
{
	// Upper-left corner.
    int w = GetWidth();
    int h = GetHeight();
    int cx = GetFrameCropX();
    int cy = GetFrameCropY();

    int x = (int)myPosition.x - (w / 2);
    int y = (int)myPosition.y - (h / 2);

	COLORREF crOldBack = SetBkColor(myBackBufferDC, RGB(255, 255, 255));
	COLORREF crOldText = SetTextColor(myBackBufferDC, RGB(0, 0, 0));

	// Select the image into the appropriate DC
	SelectObject(mySpriteDC, myImage);

	// Select the mask bitmap into the appropriate DC
	SelectObject(mySpriteMaskDC, myImageMask);

	// Build mask based on transparent color
	SetBkColor(mySpriteDC, myTransparentColor);
	BitBlt(mySpriteMaskDC, 0, 0, w, h, mySpriteDC, cx, cy, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	BitBlt(myBackBufferDC, x+dx, y+dy, w, h, mySpriteDC, cx, cy, SRCINVERT);
	BitBlt(myBackBufferDC, x+dx, y+dy, w, h, mySpriteMaskDC, 0, 0, SRCAND);
	BitBlt(myBackBufferDC, x+dx, y+dy, w, h, mySpriteDC, cx, cy, SRCINVERT);

	// Restore settings
	SetBkColor(myBackBufferDC, crOldBack);
	SetTextColor(myBackBufferDC, crOldText);
}

void Sprite::drawBitmap(int dx, int dy) const
{
    int w = GetWidth();
    int h = GetHeight();
    int cx = GetFrameCropX();
    int cy = GetFrameCropY();

    int x = (int)myPosition.x - (w / 2);
    int y = (int)myPosition.y - (h / 2);

    HGDIOBJ oldObj = SelectObject(mySpriteDC, myImage);

    // Draw the image to the backbuffer with SRCCOPY
    BitBlt(myBackBufferDC, x+dx, y+dy, w, h, mySpriteDC, cx, cy, SRCCOPY);

    // Restore the original bitmap object.
    SelectObject(mySpriteDC, oldObj);
}

void Sprite::drawAlphaBlend( int dx, int dy ) const
{
    int w = GetWidth();
    int h = GetHeight();
    int cx = GetFrameCropX();
    int cy = GetFrameCropY();

    int x = (int)myPosition.x - (w / 2);
    int y = (int)myPosition.y - (h / 2);

    HGDIOBJ oldObj = SelectObject(mySpriteDC, myImage);

    BLENDFUNCTION blend;
    blend.SourceConstantAlpha = myAlpha;
    blend.BlendFlags = 0;
    blend.AlphaFormat = 0;
    blend.BlendOp = AC_SRC_OVER;
    
    AlphaBlend(myBackBufferDC, x+dx, y+dy, w, h, mySpriteDC, cx, cy, w, h, blend);

    SelectObject(mySpriteDC, oldObj);
}
