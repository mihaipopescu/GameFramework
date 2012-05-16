#include "TileableSprite.h"


void TileableSprite::Initialize(int cxTiles, int cyTiles)
{
    myXTileCount = cxTiles;
    myYTileCount = cyTiles;
}

void TileableSprite::Draw() const
{
    for(int y=0;y<myYTileCount;++y)
	{
        for(int x=0;x<myXTileCount;++x)
	    {
		    Sprite::DrawWithOffset(x*GetWidth(), y*GetHeight());
	    }
    }
}