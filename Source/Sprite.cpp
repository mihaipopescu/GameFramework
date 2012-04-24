//-----------------------------------------------------------------------------
// File: Sprite.cpp
//
// Desc: Class used for rendering and handling sprites
//
// Author: Mihai Popescu
//-----------------------------------------------------------------------------

#include "Sprite.h"


extern HINSTANCE g_hInst;

#define INVALID_TRANSPARENT_COLOR RGB(0xBA, 0xDB, 0xAD)

Sprite::Sprite(int imageID, int maskID)
{
	// Load the bitmap resources.
	myImage = LoadBitmap(g_hInst, MAKEINTRESOURCE(imageID));
	myImageMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(maskID));

	// Get the BITMAP structure for each of the bitmaps.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);
	GetObject(myImageMask, sizeof(BITMAP), &myBitmapMask);

	// Image and Mask should be the same dimensions.
	assert(myBitmap.bmWidth == myBitmapMask.bmWidth);
	assert(myBitmap.bmHeight == myBitmapMask.bmHeight);	

	myTransparentColor = 0;
	mySpriteDC = 0;
}

Sprite::Sprite(const char *szImageFile)
{
    myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

    myImageMask = 0;
    mySpriteDC = 0;
    myTransparentColor = INVALID_TRANSPARENT_COLOR;

    // Get the BITMAP structure for the bitmap.
    GetObject(myImage, sizeof(BITMAP), &myBitmap);
}

Sprite::Sprite(const char *szImageFile, const char *szMaskFile)
{
	myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	myImageMask = (HBITMAP)LoadImage(g_hInst, szMaskFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// Get the BITMAP structure for each of the bitmaps.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);
	GetObject(myImageMask, sizeof(BITMAP), &myBitmapMask);

	// Image and Mask should be the same dimensions.
	assert(myBitmap.bmWidth == myBitmapMask.bmWidth);
	assert(myBitmap.bmHeight == myBitmapMask.bmHeight);

	myTransparentColor = INVALID_TRANSPARENT_COLOR;
	mySpriteDC = 0;
}

Sprite::Sprite(const char *szImageFile, COLORREF crTransparentColor)
{
	myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	myImageMask = 0;
	mySpriteDC = 0;
	myTransparentColor = crTransparentColor;

	// Get the BITMAP structure for the bitmap.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);
}

Sprite::~Sprite()
{
	// Free the resources we created in the constructor.
	DeleteObject(myImage);
	DeleteObject(myImageMask);

	DeleteDC(mySpriteDC);
}

void Sprite::Initialize(HDC hBackBufferDC)
{
	if( mySpriteDC == 0 )
	{
		mySpriteDC = CreateCompatibleDC(hBackBufferDC);
	}
}

void Sprite::Draw(HDC hBackBufferDC) const
{
	DrawWithOffset(hBackBufferDC, 0, 0);
}

void Sprite::DrawWithOffset(HDC hBackBufferDC, int dx, int dy) const
{
    if( myImageMask != 0 )
        drawMask(hBackBufferDC, dx, dy);
    else if ( myTransparentColor != INVALID_TRANSPARENT_COLOR )
        drawTransparent(hBackBufferDC, dx, dy);
    else
        drawBitmap(hBackBufferDC, dx, dy);
}

void Sprite::drawMask(HDC hBackBufferDC, int dx, int dy) const
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
	BitBlt(hBackBufferDC, x+dx, y+dx, w, h, mySpriteDC, cx, cy, SRCAND);

	// Now select the image bitmap.
	SelectObject(mySpriteDC, myImage);

	// Draw the image to the backbuffer with SRCPAINT. This
	// will only draw the image onto the pixels that where previously
	// marked black by the mask.
	BitBlt(hBackBufferDC, x+dx, y+dx, w, h, mySpriteDC, cx, cy, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(mySpriteDC, oldObj);
}

void Sprite::drawTransparent(HDC hBackBufferDC, int dx, int dy) const
{
	// Upper-left corner.
    int w = GetWidth();
    int h = GetHeight();
    int cx = GetFrameCropX();
    int cy = GetFrameCropY();

    int x = (int)myPosition.x - (w / 2);
    int y = (int)myPosition.y - (h / 2);

	COLORREF crOldBack = SetBkColor(hBackBufferDC, RGB(255, 255, 255));
	COLORREF crOldText = SetTextColor(hBackBufferDC, RGB(0, 0, 0));
	HDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage=CreateCompatibleDC(hBackBufferDC);
	dcTrans=CreateCompatibleDC(hBackBufferDC);

	// Select the image into the appropriate dc
	SelectObject(dcImage, myImage);

	// Create the mask bitmap
	BITMAP bitmap;
	GetObject(myImage, sizeof(BITMAP), &bitmap);
	HBITMAP bitmapTrans = CreateBitmap(w, h, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	SelectObject(dcTrans, bitmapTrans);

	// Build mask based on transparent color
	SetBkColor(dcImage, myTransparentColor);
	BitBlt(dcTrans, 0, 0, bitmap.bmWidth, bitmap.bmHeight, dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	BitBlt(hBackBufferDC, x+dx, y+dy, w, h, dcImage, cx, cy, SRCINVERT);
	BitBlt(hBackBufferDC, x+dx, y+dy, w, h, dcTrans, cx, cy, SRCAND);
	BitBlt(hBackBufferDC, x+dx, y+dy, w, h, dcImage, cx, cy, SRCINVERT);

	// free memory	
	DeleteDC(dcImage);
	DeleteDC(dcTrans);
	DeleteObject(bitmapTrans);

	// Restore settings
	SetBkColor(hBackBufferDC, crOldBack);
	SetTextColor(hBackBufferDC, crOldText);
}

void Sprite::drawBitmap(HDC hBackBufferDC, int dx, int dy) const
{
    int w = GetWidth();
    int h = GetHeight();
    int cx = GetFrameCropX();
    int cy = GetFrameCropY();

    int x = (int)myPosition.x - (w / 2);
    int y = (int)myPosition.y - (h / 2);

    HGDIOBJ oldObj = SelectObject(mySpriteDC, myImage);

    // Draw the image to the backbuffer with SRCCOPY
    BitBlt(hBackBufferDC, x+dx, y+dy, w, h, mySpriteDC, cx, cy, SRCCOPY);

    // Restore the original bitmap object.
    SelectObject(mySpriteDC, oldObj);
}
