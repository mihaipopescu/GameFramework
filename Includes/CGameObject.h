/*! 
 *  \brief     CGameObject class.
 *  \details   This class is used as a base class for all game objects.
 *  \author    Mihai Popescu
 *  \date      27/03/2012
 *  \copyright GNU Public License.
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <Windows.h>
#include <memory>

enum GameObjectType
{
	GOT_Invalid = -1,
	GOT_Player,
	GOT_Bullet,
	GOT_Crate,
};

class CGameObject
{
public:
	virtual ~CGameObject() { }

	virtual GameObjectType GetObjectType() const { return GOT_Invalid; }
	virtual void Update(float dt) = 0;
	virtual void Draw(HDC hdc) const = 0;
	virtual bool Expired() const { return false; }
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
	DrawFunctor(HDC hdc) : myHDC(hdc) { }
	void operator()(const std::shared_ptr<CGameObject>& object)
	{
		object->Draw(myHDC);
	}
protected:
	HDC myHDC;
};

#endif // GAME_OBJECT_H