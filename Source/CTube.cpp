#include "CTube.h"
#include "CGameApp.h"

CTube::CTube(ETubeType type, const Vec2& position, int gap)
{
	float fSizeFactor = 0.0f;
	switch (type)
	{
	case ETube_Up:
		m_pSprite = new Sprite("data/tube_up.bmp", RGB(0xff, 0x00, 0xff));
		fSizeFactor = 1.0f;
		break;
	case ETube_Down:
		m_pSprite = new Sprite("data/tube_down.bmp", RGB(0xff, 0x00, 0xff));
		fSizeFactor = -1.0f;
		break;
	default:
		m_pSprite = NULL;
		break;
	}

	myPosition = position + fSizeFactor * Vec2(0.f, GetHeight() / 2.f + gap * 0.5f);
	m_pSprite->myPosition = myPosition;
}

CTube::~CTube()
{
	delete m_pSprite;
}

void CTube::Update(float dt)
{
	myVelocity = CGameApp::Get()->GetParalaxLayer()->myVelocity;

	CGameObject::Update(dt);

	m_pSprite->myPosition = myPosition;
}

void CTube::Draw() const
{
	// Draw if visibility condition is satisfied
	if (myPosition.x > -GetWidth() / 2 && myPosition.x < CGameApp::Get()->GetViewWidth() + GetWidth())
	{
		m_pSprite->Draw();
	}
}

bool CTube::Expired() const
{
	return myPosition.x < -GetWidth() / 2;
}

