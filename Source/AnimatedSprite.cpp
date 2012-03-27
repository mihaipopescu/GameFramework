#include "AnimatedSprite.h"


void AnimatedSprite::Initialize(const RECT& rcFirstFrame, int iFrameCount, float fFrameDuration)
{
	myFrameStartCrop.x = rcFirstFrame.left;
	myFrameStartCrop.y = rcFirstFrame.top;
	Sprite::myFrameWidth = rcFirstFrame.right - rcFirstFrame.left;
	Sprite::myFrameHeight = rcFirstFrame.bottom - rcFirstFrame.top;

	myFrameCount = iFrameCount;
	myFrameDuration = fFrameDuration;
	myFrameTimer = 0.f;
	myFrameIndex = 0;

	myInitializedState = true;
}

void AnimatedSprite::Update( float dt )
{
	if( !myInitializedState )
		return;

	if( myFrameIndex != myFrameCount )
	{
		myFrameTimer += dt;
		if( myFrameIndex == 0 || myFrameTimer >= myFrameDuration )
		{
			Sprite::myFrameCropX = myFrameStartCrop.x + myFrameIndex % (GetWidth() / Sprite::myFrameWidth) * Sprite::myFrameWidth;
			Sprite::myFrameCropY = myFrameStartCrop.y + myFrameIndex / (GetHeight() / Sprite::myFrameHeight) * Sprite::myFrameHeight;
			myFrameTimer = 0.f;
			myFrameIndex++;
		}
	}
}
