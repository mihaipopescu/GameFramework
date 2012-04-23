#include "TileableSprite.h"


void TileableSprite::Initialize(HDC hdc, int cxTiles, int cyTiles)
{
    Sprite::Initialize(hdc);

    myXTileCount = cxTiles;
    myYTileCount = cyTiles;
}

void TileableSprite::Draw(HDC hBackBufferDC) const
{
    for(int y=0;y<myYTileCount;++y)
	{
        for(int x=0;x<myXTileCount;++x)
	    {
		    Sprite::DrawWithOffset(hBackBufferDC, x*myFrameWidth, y*myFrameHeight);
	    }
    }
}