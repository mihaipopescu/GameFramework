#include "CTube.h"

CTube::CTube(ETubeType type, const Vec2& position)
{
	switch (type)
	{
	case ETube_Up:
		m_pSprite = new Sprite("data/tube_up.bmp");
		break;
	case ETube_Down:
		m_pSprite = new Sprite("data/tube_down.bmp");
		break;
	default:
		m_pSprite = NULL;
		break;
	}

	m_pSprite->myPosition = position;
}


void CTube::Draw() const
{
	m_pSprite->Draw();
}