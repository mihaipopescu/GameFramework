#include "CGameObject.h"
#include "CGameApp.h"

CGameObject::CGameObject() 
    : myCollisionSide(CS_None)
{
    mySpeedPen = CreatePen(PS_SOLID, 1, RGB(0xff, 0x00, 0x00));
    myAccelerationPen = CreatePen(PS_SOLID, 1, RGB(0x00, 0xff, 0x00));
}

CGameObject::~CGameObject()
{
    DeleteObject(mySpeedPen);
    DeleteObject(myAccelerationPen);
}

void CGameObject::Update(float dt)
{
	Vec2 a;

	if( AffectedByGravity() && ((myCollisionSide & CS_Bottom) == 0) )
		a.y = GRAVITATIONAL_CONSTANT;

	// compute resultant acceleration
	a += myAcceleration;

	// apply Stokes law. For more info see: http://en.wikipedia.org/wiki/Stokes'_law
	a += myVelocity * (-MEDIUM_VISCOSITY);

    // on collision with objects we have to apply the third fundamental law of motion
    // F_Reaction = F_Action

    if( (myCollisionSide & CS_Left) || (myCollisionSide & CS_Right) )
    {
        myVelocity.x = 0;  
    }

    if( (myCollisionSide & CS_Top) || (myCollisionSide & CS_Bottom) )
    {
        myVelocity.y = 0;
    }
    
    myCollisionSide = CS_None;

    myResultant = a;

	// Update the sprites position and speed
	myPosition += myVelocity * dt + a * dt * dt / 2;
	myVelocity += a * dt;
}

void CGameObject::Draw() const
{ 
    HDC hdc = CGameApp::Get()->GetBackBufferDC();
    HGDIOBJ old_obj = SelectObject(hdc, mySpeedPen);

    MoveToEx(hdc, (int)myPosition.x, (int)myPosition.y, NULL);
    LineTo(hdc, (int)(myPosition.x + myVelocity.x), (int)(myPosition.y + myVelocity.y));
 
    SelectObject(hdc, myAccelerationPen);
    MoveToEx(hdc, (int)myPosition.x, (int)myPosition.y, NULL);
    LineTo(hdc, (int)(myPosition.x + myResultant.x), (int)(myPosition.y + myResultant.y));
    
    SelectObject(hdc, old_obj);
}