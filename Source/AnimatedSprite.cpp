#include "AnimatedSprite.h"


void AnimatedSprite::Initialize(const RECT& rcFirstFrame, int iFrameCount, float fFrameDuration)
{
	myFrameStartCrop.x = rcFirstFrame.left;
	myFrameStartCrop.y = rcFirstFrame.top;

    myFrameCrop = myFrameStartCrop;

	myFrameWidth = rcFirstFrame.right - rcFirstFrame.left;
	myFrameHeight = rcFirstFrame.bottom - rcFirstFrame.top;

	myFrameCount = iFrameCount;
	myFrameDuration = fFrameDuration;
	myFrameTimer = 0.f;
	myFrameIndex = 0;

	myInitializedState = true;
    myLoopingState = false;
    myPlayState = false;
}

void AnimatedSprite::Update( float dt )
{
	CGameObject::Update(dt);

	if( !myInitializedState || !myPlayState )
		return;

	if( myFrameIndex != myFrameCount )
	{
		myFrameTimer += dt;
		if( myFrameTimer >= myFrameDuration )
		{
			myFrameCrop.x = myFrameStartCrop.x + (myFrameIndex % (Sprite::GetWidth() / myFrameWidth)) * myFrameWidth;
			myFrameCrop.y = myFrameStartCrop.y + (myFrameIndex / (Sprite::GetHeight() / myFrameHeight)) * myFrameHeight;

			myFrameTimer = 0.f;
			myFrameIndex++;

            // loop when finished
            if( myFrameIndex == myFrameCount )
            {
                myFrameIndex = 0;
                if( !myLoopingState  )
                    myPlayState = false;
            }
		}
	}
}
