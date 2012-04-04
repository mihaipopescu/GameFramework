//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class CPlayer : public CGameObject
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION 
	{ 
		DIR_FORWARD		= 1, 
		DIR_BACKWARD	= 2, 
		DIR_LEFT			= 4, 
		DIR_RIGHT			= 8, 
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
			 CPlayer();
	virtual ~CPlayer();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void						Init(HDC hdc, const Vec2& position);
	void						Update( float dt );
	GameObjectType	GetObjectType() const { return GOT_Player; }
	void						Draw(HDC hdc) const;
	void						Move(ULONG ulDirection);
	virtual int			GetWidth() const { return m_pSprite->GetWidth(); }
	virtual int			GetHeight() const { return m_pSprite->GetHeight(); }
	void						Explode();

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*					m_pSprite;
	ESpeedStates		m_eSpeedState;
	float						m_fTimer;
	
	bool						m_bExplosion;
	AnimatedSprite*	m_pExplosionSprite;
};

#endif // _CPLAYER_H_