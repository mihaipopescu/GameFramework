/*! 
 *  \brief     AnimatedSprite class.
 *  \details   This class is used to handle animated sprites.
 *  \author    Mihai Popescu
 *  \date      26/03/2012
 *  \copyright GNU Public License.
 */
#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H
#include "Sprite.h"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(const char *szImageFile, const char *szMaskFile) : Sprite(szImageFile, szMaskFile) { myInitializedState = false; }
	AnimatedSprite(const char *szImageFile, COLORREF crTransparentColor) : Sprite(szImageFile, crTransparentColor) { myInitializedState = false; }

public:
	//! Initialization function
	void Initialize(const RECT& rcFirstFrame, int iFrameCount, float fFrameDuration);
	//! Returns the current frame count
	int GetFrameCount() { return myFrameCount; }
	//! Starts playing the animated sprite
	void Play() { myFrameIndex = 0; }
	//! Returns the finished state of the animation
	bool IsFinished() const { return myFrameIndex == myFrameCount; }

public:
	//! Handle switching to the next frame.
	virtual void Update(float dt);

protected:
	bool myInitializedState;	//!< tells if the class is initialized
	float myFrameDuration;		//!< how much a frame 
	float myFrameTimer;				//!< time accumulator
	POINT myFrameStartCrop;		//!< first point of the frame (upper-left corner)
	int myFrameCount;					//!< number of frames
	int myFrameIndex;					//!< current frame number
};

#endif // ANIMATED_SPRITE_H