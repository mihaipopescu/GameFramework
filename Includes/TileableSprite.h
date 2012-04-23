/*! 
 *  \brief     TileableSprite class.
 *  \details   This class is used to handle tilable sprites.
 *  \author    Mihai Popescu
 *  \date      23/04/2012
 *  \copyright GNU Public License.
 */
#ifndef TILEABLE_SPRITE_H
#define TILEABLE_SPRITE_H

#include "Sprite.h"


class TileableSprite : public Sprite
{
public:
    TileableSprite(const char *szImageFile) : Sprite(szImageFile) { }
    TileableSprite(const char *szImageFile, const char *szMaskFile) : Sprite(szImageFile, szMaskFile) { }
    TileableSprite(const char *szImageFile, COLORREF crTransparentColor) : Sprite(szImageFile, crTransparentColor) { }

    void Initialize(HDC hdc, int cxTiles, int cyTiles);

	virtual void Draw(HDC hBackBufferDC) const;

private:
    int myXTileCount;
    int myYTileCount;
};

#endif // TILEABLE_SPRITE_H