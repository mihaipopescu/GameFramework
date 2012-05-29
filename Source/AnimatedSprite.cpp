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

inline void AnimatedSprite::SetFrameIndex(int iFrameIndex)
{
    myFrameIndex = iFrameIndex; 
    myFrameCrop.x = myFrameStartCrop.x + (myFrameIndex % (Sprite::GetWidth() / myFrameWidth)) * myFrameWidth;
    myFrameCrop.y = myFrameStartCrop.y + (myFrameIndex / (Sprite::GetHeight() / myFrameHeight)) * myFrameHeight;
}

void AnimatedSprite::Update( float dt )
{
	if( !myInitializedState || !myPlayState )
		return;

	if( myFrameIndex != myFrameCount )
	{
		myFrameTimer += dt;
		if( myFrameTimer >= myFrameDuration )
		{
			myFrameTimer = 0.f;
            SetFrameIndex(myFrameIndex + 1);

            // loop when finished
            if( myFrameIndex == myFrameCount )
            {
                SetFrameIndex(0);
                if( !myLoopingState  )
                    myPlayState = false;
            }
		}
	}
}
