#include "AnimatedSprite.h"


void AnimatedSprite::Initialize(HDC hdc, const RECT& rcFirstFrame, int iFrameCount, float fFrameDuration)
{
    Sprite::Initialize(hdc);

	myFrameStartCrop.x = rcFirstFrame.left;
	myFrameStartCrop.y = rcFirstFrame.top;

	myFrameWidth = rcFirstFrame.right - rcFirstFrame.left;
	myFrameHeight = rcFirstFrame.bottom - rcFirstFrame.top;

	myFrameCount = iFrameCount;
	myFrameDuration = fFrameDuration;
	myFrameTimer = 0.f;
	myFrameIndex = myFrameCount;

	myInitializedState = true;
    myLoopingState = false;
}

void AnimatedSprite::Update( float dt )
{
	CGameObject::Update(dt);

	if( !myInitializedState )
		return;

	if( myFrameIndex != myFrameCount )
	{
		myFrameTimer += dt;
		if( myFrameTimer >= myFrameDuration )
		{
			myFrameCropX = myFrameStartCrop.x + (myFrameIndex % (Sprite::GetWidth() / myFrameWidth)) * myFrameWidth;
			myFrameCropY = myFrameStartCrop.y + (myFrameIndex / (Sprite::GetHeight() / myFrameHeight)) * myFrameHeight;

			myFrameTimer = 0.f;
			myFrameIndex++;

            // loop when finished
            if( myLoopingState && myFrameIndex == myFrameCount )
                myFrameIndex = 0;
		}
	}
}
