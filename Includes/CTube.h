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

	CTube(ETubeType type, const Vec2& position, int gap);
	~CTube();

	virtual void			Draw() const;
	virtual void			Update(float dt);

	virtual int				GetWidth() const { return m_pSprite->GetWidth(); }
	virtual int				GetHeight() const { return m_pSprite->GetHeight(); }
	virtual bool			Expired() const;
	virtual GameObjectType  GetObjectType() const { return GOT_Tube; }
	virtual CollisionFlag	GetCollisionResponseFlag() const { return CF_Tube; }

private:
	Sprite*		m_pSprite;
};


#endif // _CTUBE_H_