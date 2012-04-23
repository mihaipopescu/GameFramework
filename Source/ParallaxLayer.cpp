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
    int cx = 2 + ulViewWidth / TileableSprite::GetWidth();
    int cy = 2 + ulViewHeight / TileableSprite::GetHeight();

    TileableSprite::Initialize(hdc, ulAxis & AXIS_HORIZONTAL ? cx : 1, ulAxis & AXIS_VERTICAL ? cy : 1);

    m_iLayerSpeed = iLayerSpeed;
    m_nViewWidth = ulViewWidth;
    m_nViewHeight = ulViewHeight;
}

void ParallaxLayer::Update(float dt)
{
	Sprite::Update(dt);

	if(myPosition.x<=-myFrameWidth/2)
		myPosition.x=0;

	if(myPosition.x>=myFrameWidth/2)
		myPosition.x=0;
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
