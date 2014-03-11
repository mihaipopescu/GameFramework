#ifndef _CTUBE_H_
#define _CTUBE_H_

#include "Sprite.h"
#include "CGameObject.h"

class CTube: public CGameObject
{
public:
	enum ETubeType
	{
		ETube_Up,
		ETube_Down,
	};

	CTube(ETubeType type, const Vec2& position);

	void		Draw() const;

	virtual int	GetWidth() const { return m_pSprite->GetWidth(); }
	virtual int GetHeight() const { return m_pSprite->GetHeight(); }

private:
	Sprite*		m_pSprite;
};


#endif // _CTUBE_H_