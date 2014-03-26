/*! 
 *  \brief     CGameObject class.
 *  \details   This class is used as a base class for all game objects.
 *  \author    Mihai Popescu
 *  \date      27/03/2012
 *  \copyright GNU Public License.
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Vec2.h"
#include "main.h"
#include <memory>

enum GameObjectType
{
	GOT_Invalid = -1,
	GOT_Player,
	GOT_Tube,
};

enum CollisionSide
{
    CS_None = 0,
    CS_Left = 1,
    CS_Right = 2,
    CS_Top = 4,
    CS_Bottom = 8
};

enum CollisionFlag
{
	CF_None = 0,
	CF_Screen = 1,
	CF_Player = 2,
	CF_Tube = 4,
};

class IDrawable
{
public:
    virtual void Draw() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

public:
    Vec2 myPosition;
};

class CGameObject : public IDrawable
{
public:
    CGameObject();
    virtual ~CGameObject();

	virtual GameObjectType GetObjectType() const { return GOT_Invalid; }
	virtual CollisionFlag GetCollisionResponseFlag() const { return CF_None; }

    virtual void Draw() const;
	virtual void Update(float dt);
	virtual bool Expired() const { return false; }
	virtual void ResolveCollision();

public:
	// Keep these public because they need to be
	// modified externally frequently.
	Vec2 myVelocity;
	Vec2 myAcceleration;
    int myCollisionSide;
	int myCollisionMask;
	bool myBodyIsStatic;

private:

#ifdef _DEBUG
    HPEN mySpeedPen;
    HPEN myAccelerationPen;
#endif

    Vec2 myResultant;
};


class ExpiredPredicate
{
public:
	bool operator()(const std::shared_ptr<CGameObject>& object) const
	{
		return object->Expired();
	}
};

class UpdateFunctor
{
public:
	UpdateFunctor(float dt) : myDt(dt) { }
	void operator()(std::shared_ptr<CGameObject>& object)
	{
		object->Update(myDt);
	}
protected:
	float myDt;
};

class DrawFunctor
{
public:
	DrawFunctor() { }
	void operator()(const std::shared_ptr<CGameObject>& object)
	{
		object->Draw();
	}
};

#endif // GAME_OBJECT_H