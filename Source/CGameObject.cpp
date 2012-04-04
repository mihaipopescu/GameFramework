#include "CGameObject.h"


void CGameObject::Update(float dt)
{
	Vec2 a;

	if( AffectedByGravity() )
		a.y = GRAVITATIONAL_CONSTANT;

	// compute resultant acceleration
	a += myAcceleration;

	// apply Stokes law. For more info see: http://en.wikipedia.org/wiki/Stokes'_law
	a += myVelocity * (-MEDIUM_VISCOSITY);

	// Update the sprites position and speed
	myPosition += myVelocity * dt + a * dt * dt / 2;
	myVelocity += a * dt;
}