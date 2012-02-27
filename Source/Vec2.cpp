//-----------------------------------------------------------------------------
// File: Vec2.h
//
// Desc: Defines vector based co-ords used throughout game.
//		 Overrides operators to allow two entry vector based calculations
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Vec2 Specific Includes
//-----------------------------------------------------------------------------
#include "Vec2.h"
#include "main.h"

Vec2& Vec2::operator-()
{
	x = -x;
	y = -y;
	return *this;
}

bool Vec2::operator==(Vec2 v)
{
	return (x == v.x && y == v.y);
}

bool Vec2::operator!=(Vec2 v)
{
	return (x != v.x || y != v.y);
}

Vec2 Vec2::operator+(Vec2 v)
{
	return Vec2(x + v.x, y + v.y);
}

Vec2 Vec2::operator-(Vec2 v)
{
	return Vec2(x - v.x, y - v.y);
}

Vec2& Vec2::operator+=(Vec2 v)
{
	*this = *this + v;
	return *this;
}

Vec2& Vec2::operator-=(Vec2 v)
{
	*this = *this - v;
	return *this;
}

double Vec2::Magnitude() const 
{
	return sqrt(x*x + y*y);
}

double Vec2::Argument() const 
{
	if (x < 0.0)
	{
		return PI + atan(y / x);
	}
	else if (fabs(x) < EPS)
	{
		return (y > 0 ? PI : -PI);
	}
	else
	{
		return atan(y / x);
	}
}

double Vec2::Distance(Vec2 v) const // Euclidean distance
{
	double dx = x - v.x;
	double dy = y - v.y;
	return sqrt(dx*dx + dy*dy);
}

Vec2 Polar(double r, double radians)
{
	Vec2 result;
	if (r < 0)
	{
		r = -r;
	}
	radians = PrincipleAngle(radians);
	result.x = r * cos(radians);
	result.y = r * sin(radians);
	return result;
}

double Vec2::operator*(Vec2 v)  // dot product
{  
	return x*v.x + y*v.y;
}

void Vec2::Rotate(double radians)
{
	double xx = cos(radians)*x - sin(radians)*y;
	double yy = sin(radians)*x + cos(radians)*y;
	x = xx;
	y = yy;
}

Vec2 Vec2::operator*(double s) // scale
{
	return Vec2(s*x, s*y);
}

Vec2 Vec2::operator/(double s) // scale
{
	return Vec2(x/s, y/s);
}

double PrincipleAngle(double radians)
{
	double result = fmod(radians, 2 * PI);
	if (radians > 0)
	{
		return result;
	}
	else
	{
		return 2 * PI + result;
	}
}
