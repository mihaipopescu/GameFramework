#include "CGameObject.h"
#include "CGameApp.h"

#define PIXEL_PER_METERS 20

CGameObject::CGameObject() 
    : myCollisionSide(CS_None)
	, myCollisionMask(CF_None)
	, myBodyIsStatic(true)
{
#ifdef _DEBUG
    mySpeedPen = CreatePen(PS_SOLID, 1, RGB(0xff, 0x00, 0x00));
    myAccelerationPen = CreatePen(PS_SOLID, 1, RGB(0x00, 0xff, 0x00));
#endif
}

CGameObject::~CGameObject()
{
#ifdef _DEBUG
    DeleteObject(mySpeedPen);
    DeleteObject(myAccelerationPen);
#endif
}

void CGameObject::ResolveCollision()
{
	// on collision with objects we have to apply the third fundamental law of motion
	// F_Reaction = F_Action

	if (!myBodyIsStatic)
	{
		if ((myCollisionSide & CS_Left) || (myCollisionSide & CS_Right))
		{
			myVelocity.x = 0;
		}

		if ((myCollisionSide & CS_Top) || (myCollisionSide & CS_Bottom))
		{
			myVelocity.y = 0;
		}
	}

	myCollisionSide = CS_None;
}

void CGameObject::Update(float dt)
{
	Vec2 a;

	ResolveCollision();

	if (!myBodyIsStatic)
	{
		// apply gravity
		a.y = GRAVITATIONAL_CONSTANT * PIXEL_PER_METERS;

		// apply Stokes law. For more info see: http://en.wikipedia.org/wiki/Stokes'_law
		a += myVelocity * (-MEDIUM_VISCOSITY);
	}
	
	// compute resultant acceleration
	a += myAcceleration;

    myResultant = a;

	// Update the sprites position and speed
	myPosition += myVelocity * dt + a * dt * dt / 2;
	myVelocity += a * dt;
}

void CGameObject::Draw() const
{ 
#ifdef _DEBUG
    HDC hdc = CGameApp::Get()->GetBackBufferDC();

    HGDIOBJ old_obj = SelectObject(hdc, mySpeedPen);
    MoveToEx(hdc, (int)myPosition.x, (int)myPosition.y, NULL);
    LineTo(hdc, (int)(myPosition.x + myVelocity.x), (int)(myPosition.y + myVelocity.y));

	Vec2 TL(myPosition.x - GetWidth() / 2.0f, myPosition.y - GetHeight() / 2.0f);
	Vec2 TR(myPosition.x + GetWidth() / 2.0f, myPosition.y - GetHeight() / 2.0f);
	Vec2 BR(myPosition.x + GetWidth() / 2.0f, myPosition.y + GetHeight() / 2.0f);
	Vec2 BL(myPosition.x - GetWidth() / 2.0f, myPosition.y + GetHeight() / 2.0f);

	if (myCollisionSide & CS_Top)
	{
		MoveToEx(hdc, (int)TL.x, (int)TL.y , NULL);
		LineTo(hdc, (int)TR.x, (int)TR.y);
	}

	if (myCollisionSide & CS_Right)
	{
		MoveToEx(hdc, (int)TR.x, (int)TR.y, NULL);
		LineTo(hdc, (int)BR.x, (int)BR.y);
	}

	if (myCollisionSide & CS_Bottom)
	{
		MoveToEx(hdc, (int)BR.x, (int)BR.y, NULL);
		LineTo(hdc, (int)BL.x, (int)BL.y);
	}

	if (myCollisionSide & CS_Left)
	{
		MoveToEx(hdc, (int)BL.x, (int)BL.y, NULL);
		LineTo(hdc, (int)TL.x, (int)TL.y);
	}
 
    SelectObject(hdc, myAccelerationPen);
    MoveToEx(hdc, (int)myPosition.x, (int)myPosition.y, NULL);
    LineTo(hdc, (int)(myPosition.x + myResultant.x), (int)(myPosition.y + myResultant.y));

    SelectObject(hdc, old_obj);
#endif
}