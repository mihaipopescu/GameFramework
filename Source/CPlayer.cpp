//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "CPlayer.h"

#define FLAP_SPEED_IMPULSE -100

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer::CPlayer()
{
	m_pSprite = new AnimatedSprite("data/bird_green.bmp", RGB(0xff, 0x00, 0xff));
}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
}

void CPlayer::Init(const Vec2& position)
{
    myPosition = position;

    // Animation frame crop rectangle
    RECT r;
    r.left = 0;
    r.top = 0;
    r.right = 34;
    r.bottom = 24;
	m_pSprite->Initialize(r, 3, 1/8.f);
	m_pSprite->Play(true);
}

void CPlayer::Update(float dt)
{
	CGameObject::Update(dt);

	// Update sprites
	m_pSprite->myPosition = myPosition;
	
	m_pSprite->Update(dt);

	// Get velocity
	double v = myVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommended to be used in complex projects.


	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see MSDN reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void CPlayer::Draw() const
{
	m_pSprite->Draw();
}

void CPlayer::Flap()
{
	myVelocity.y = FLAP_SPEED_IMPULSE;
}
