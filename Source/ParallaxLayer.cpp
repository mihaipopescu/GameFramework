#include "ParallaxLayer.h"


ParallaxLayer::ParallaxLayer(const char *szImageFile)
    : TileableSprite(szImageFile)
{
}

ParallaxLayer::~ParallaxLayer()
{
}

void ParallaxLayer::Initialize(HDC hdc, ULONG ulAxis, int iLayerSpeed, ULONG ulViewWidth, ULONG ulViewHeight)
{
    int cx = 2 + (ulViewWidth / myFrameWidth);
    int cy = 2 + (ulViewHeight / myFrameHeight);

    if( cx > 2 && (ulViewWidth % myFrameWidth) != 0 )
    {
        cx++;
    }

    if( cy > 2 && (ulViewHeight % myFrameHeight) != 0 )
    {
        cy++;
    }

    TileableSprite::Initialize(hdc, ulAxis & AXIS_HORIZONTAL ? cx : 1, ulAxis & AXIS_VERTICAL ? cy : 1);

    Vec2 vInsertPos = TileableSprite::myPosition;

    if( ulAxis & AXIS_HORIZONTAL )
    {
        vInsertPos.x = -myFrameWidth - myFrameWidth/2;
    }

    if( ulAxis & AXIS_VERTICAL )
    {
        vInsertPos.y = -myFrameHeight - myFrameHeight/2;
    }

    TileableSprite::myPosition = vInsertPos;

    m_iLayerSpeed = iLayerSpeed;
    m_nViewWidth = ulViewWidth;
    m_nViewHeight = ulViewHeight;
}

void ParallaxLayer::Update(float dt)
{
    Sprite::Update(dt);

    if(myPosition.x-myFrameWidth/2<=-2*myFrameWidth)
        myPosition.x+=myFrameWidth;

    if(myPosition.x-myFrameWidth/2>=0)
        myPosition.x-=myFrameWidth;

    if(myPosition.y<=-2*myFrameHeight)
        myPosition.y+=myFrameHeight;

    if(myPosition.y>=0)
        myPosition.y-=myFrameHeight;

}

void ParallaxLayer::Move(ULONG ulDirection)
{
    myVelocity = Vec2();

    if( ulDirection & ParallaxLayer::DIR_RIGHT )
    {
        myVelocity.x = -m_iLayerSpeed;
    }

    if( ulDirection & ParallaxLayer::DIR_LEFT )
    {
        myVelocity.x = m_iLayerSpeed;
    }

    if( ulDirection & ParallaxLayer::DIR_FORWARD )
    {
        myVelocity.y = m_iLayerSpeed;
    }

    if( ulDirection & ParallaxLayer::DIR_BACKWARD )
    {
        myVelocity.y = -m_iLayerSpeed;
    }
}
