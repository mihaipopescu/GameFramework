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

class Sprite : public IDrawable
{
public:
    Sprite(int imageID, int maskID);
    Sprite(const char *szImageFile);
    Sprite(const char *szImageFile, const char *szMaskFile);
    Sprite(const char *szImageFile, COLORREF crTransparentColor);
    Sprite(const char *szImageFile, unsigned char uAlpha);
    virtual ~Sprite();

    void Init();
    
    virtual void Draw() const;
    void DrawWithOffset(int dx, int dy) const;

    virtual int GetWidth() const { return myBitmap.bmWidth; }
    virtual int GetHeight() const { return myBitmap.bmHeight; }

    virtual int GetFrameCropX() const { return 0; }
    virtual int GetFrameCropY() const { return 0; }

    void SetAlpha(unsigned char uAlpha) { myAlpha = uAlpha; }

private:
    // Make copy constructor and assignment operator private
    // so client cannot copy Sprites. We do this because
    // this class is not designed to be copied because it
    // is not efficient--copying bitmaps is slow (lots of memory).
    Sprite(const Sprite& rhs);
    Sprite& operator=(const Sprite& rhs);

private:
    void drawTransparent(int dx, int dy) const;
    void drawMask(int dx, int dy) const;
    void drawBitmap(int dx, int dy) const;
    void drawAlphaBlend(int dx, int dy) const;

private:
    HDC myBackBufferDC;	
    HBITMAP myImage;
    HBITMAP myImageMask;
    BITMAP myBitmap;
    BITMAP myBitmapMask;
    HDC mySpriteDC;
    HDC mySpriteMaskDC;
    unsigned char myAlpha;
    COLORREF myTransparentColor;
    
    void (Sprite::*drawInternal)(int dx, int dy) const;
};

#endif // SPRITE_H