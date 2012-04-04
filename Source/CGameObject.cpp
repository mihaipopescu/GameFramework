#include "CGameObject.h"


void CGameObject::Update(float dt)
{
	Vec2 a(0, GCONST);

	// compute resultant acceleration
	a += myAcceleration;

	// Update the sprites position and speed
	myPosition += myVelocity * dt + a * dt * dt / 2;
	myVelocity += a * dt;
}