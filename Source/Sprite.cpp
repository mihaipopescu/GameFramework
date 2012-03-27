//-----------------------------------------------------------------------------
// File: Sprite.cpp
//
// Desc: Class used for rendering and handling sprites
//
// Author: Mihai Popescu
//-----------------------------------------------------------------------------

#include "Sprite.h"

extern HINSTANCE g_hInst;

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
	myFrameCropX = myFrameCropY = 0;
	myFrameWidth = myBitmap.bmWidth;
	myFrameHeight = myBitmap.bmHeight;
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

	myTransparentColor = 0;
	mySpriteDC = 0;
	myFrameCropX = myFrameCropY = 0;
	myFrameWidth = myBitmap.bmWidth;
	myFrameHeight = myBitmap.bmHeight;
}

Sprite::Sprite(const char *szImageFile, COLORREF crTransparentColor)
{
	myImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	myImageMask = 0;
	mySpriteDC = 0;
	myFrameCropX = myFrameCropY = 0;
	myTransparentColor = crTransparentColor;

	// Get the BITMAP structure for the bitmap.
	GetObject(myImage, sizeof(BITMAP), &myBitmap);

	myFrameWidth = myBitmap.bmWidth;
	myFrameHeight = myBitmap.bmHeight;
}

Sprite::~Sprite()
{
	// Free the resources we created in the constructor.
	DeleteObject(myImage);
	DeleteObject(myImageMask);

	DeleteDC(mySpriteDC);
}

void Sprite::Setup(HDC hBackBufferDC)
{
	if( mySpriteDC == 0 )
	{
		mySpriteDC = CreateCompatibleDC(hBackBufferDC);
	}
}

void Sprite::Update(float dt)
{
	// Update the sprites position.

	myPosition += myVelocity * dt;
	// Update bounding rectangle/circle
}

void Sprite::Draw(HDC hBackBufferDC) const
{
	if( myImageMask != 0 )
		drawMask(hBackBufferDC);
	else
		drawTransparent(hBackBufferDC);
}

void Sprite::drawMask(HDC hBackBufferDC) const
{
	// The position BitBlt wants is not the sprite's center
	// position; rather, it wants the upper-left position,
	// so compute that.
	int x = (int)myPosition.x - (myFrameWidth / 2);
	int y = (int)myPosition.y - (myFrameHeight / 2);

	// Note: For this masking technique to work, it is assumed
	// the backbuffer bitmap has been cleared to some
	// non-zero value.
	// Select the mask bitmap.
	HGDIOBJ oldObj = SelectObject(mySpriteDC, myImageMask);

	// Draw the mask to the backbuffer with SRCAND. This
	// only draws the black pixels in the mask to the backbuffer,
	// thereby marking the pixels we want to draw the sprite
	// image onto.
	BitBlt(hBackBufferDC, x, y, myFrameWidth, myFrameHeight, mySpriteDC, myFrameCropX, myFrameCropY, SRCAND);

	// Now select the image bitmap.
	SelectObject(mySpriteDC, myImage);

	// Draw the image to the backbuffer with SRCPAINT. This
	// will only draw the image onto the pixels that where previously
	// marked black by the mask.
	BitBlt(hBackBufferDC, x, y, myFrameWidth, myFrameHeight, mySpriteDC, myFrameCropX, myFrameCropY, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(mySpriteDC, oldObj);
}

void Sprite::drawTransparent(HDC hBackBufferDC) const
{
	int w = GetWidth();
	int h = GetHeight();

	// Upper-left corner.
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
	HBITMAP bitmapTrans = CreateBitmap(myFrameWidth, myFrameHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	SelectObject(dcTrans, bitmapTrans);

	// Build mask based on transparent color
	SetBkColor(dcImage, myTransparentColor);
	BitBlt(dcTrans, 0, 0, bitmap.bmWidth, bitmap.bmHeight, dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	BitBlt(hBackBufferDC, x, y, myFrameWidth, myFrameHeight, dcImage, myFrameCropX, myFrameCropY, SRCINVERT);
	BitBlt(hBackBufferDC, x, y, myFrameWidth, myFrameHeight, dcTrans, myFrameCropX, myFrameCropY, SRCAND);
	BitBlt(hBackBufferDC, x, y, myFrameWidth, myFrameHeight, dcImage, myFrameCropX, myFrameCropY, SRCINVERT);

	// free memory	
	DeleteDC(dcImage);
	DeleteDC(dcTrans);
	DeleteObject(bitmapTrans);

	// Restore settings
	SetBkColor(hBackBufferDC, crOldBack);
	SetTextColor(hBackBufferDC, crOldText);
}
