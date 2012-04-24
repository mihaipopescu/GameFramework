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
    int w = GetWidth();
    int h = GetHeight();

    int cx = 2 + (ulViewWidth / w);
    int cy = 2 + (ulViewHeight / h);

    if( cx > 2 && (ulViewWidth % w) != 0 )
    {
        cx++;
    }

    if( cy > 2 && (ulViewHeight % h) != 0 )
    {
        cy++;
    }

    TileableSprite::Initialize(hdc, ulAxis & AXIS_HORIZONTAL ? cx : 1, ulAxis & AXIS_VERTICAL ? cy : 1);

    Vec2 vInsertPos = TileableSprite::myPosition;

    if( ulAxis & AXIS_HORIZONTAL )
    {
        vInsertPos.x = -3*w/2;
    }

    if( ulAxis & AXIS_VERTICAL )
    {
        vInsertPos.y = -3*h/2;
    }

    TileableSprite::myPosition = vInsertPos;

    m_iLayerSpeed = iLayerSpeed;
    m_nViewWidth = ulViewWidth;
    m_nViewHeight = ulViewHeight;
}

void ParallaxLayer::Update(float dt)
{
    Sprite::Update(dt);

    int w = GetWidth();
    int h = GetHeight();

    if(myPosition.x - w/2 <= -2*w)
        myPosition.x += w;

    if(myPosition.x - w/2 >= 0)
        myPosition.x -= w;

    if(myPosition.y - h/2 <= -2*h)
        myPosition.y += h;

    if(myPosition.y - h/2 >= 0)
        myPosition.y -= h;
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
