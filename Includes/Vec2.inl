#ifndef VEC2_INL
#define VEC2_INL

#ifndef VEC2_H
#include "Vec2.h"
#endif

#include "Main.h"

#ifndef _DEBUG
	#define VEC2_INLINE inline
#else
	#define VEC2_INLINE
#endif

VEC2_INLINE Vec2::Vec2()
	: x(0.0f)
	, y(0.0f)
{
}

VEC2_INLINE Vec2::Vec2(float x, float y)
	: x(x)
	, y(y)
{
}

VEC2_INLINE Vec2& Vec2::operator+=(const Vec2 &vector)
{
	return *this = *this + vector;
}

VEC2_INLINE Vec2& Vec2::operator-=(const Vec2 &vector)
{
	return *this = *this - vector;
}

VEC2_INLINE Vec2& Vec2::operator*=(const Vec2 &vector)
{
	return *this = *this * vector;
}

VEC2_INLINE Vec2& Vec2::operator*=(float factor)
{
	return *this = *this * factor;
}

VEC2_INLINE Vec2& Vec2::operator/=(float divisor)
{
	return *this = *this / divisor;
}

VEC2_INLINE Vec2 operator-(const Vec2& v)
{
	return Vec2(-v.x, -v.y);
}

VEC2_INLINE bool operator==(const Vec2& v1, const Vec2& v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}

VEC2_INLINE bool operator!=(const Vec2& v1, const Vec2& v2)
{
	return v1.x != v2.x && v1.y != v2.y;
}

VEC2_INLINE Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x + v2.x, v1.y + v2.y);
}

VEC2_INLINE Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x - v2.x, v1.y - v2.y);
}

VEC2_INLINE Vec2 operator*(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x * v2.x, v1.y * v2.y);
}

VEC2_INLINE Vec2 operator*(float factor, const Vec2& v)
{
	return Vec2(factor * v.x, factor * v.y);
}

VEC2_INLINE Vec2 operator*(const Vec2& v, float factor)
{
	return Vec2(factor * v.x, factor * v.y);
}

VEC2_INLINE Vec2 operator/(const Vec2& v, float divisor)
{
	return Vec2(v.x / divisor, v.y / divisor);
}

VEC2_INLINE float Dot(const Vec2& v1, const Vec2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

VEC2_INLINE float Distance(const Vec2& v1, const Vec2& v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return sqrtf(dx*dx + dy*dy);
}

VEC2_INLINE float SquaredDistance(const Vec2& v1, const Vec2& v2)
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return dx*dx + dy*dy;
}

VEC2_INLINE Vec2 Polar(float radius, float angle)
{
	Vec2 result;
	if (radius < 0)
	{
		radius = -radius;
	}

	float principalAngle = fmodf(angle, 2 * PI);
	if (principalAngle < 0)
	{
		principalAngle = principalAngle + 2 * PI;
	}

	result.x = radius * cos(principalAngle);
	result.y = radius * sin(principalAngle);

	return result;
}

VEC2_INLINE Vec2& Vec2::Rotate(float angle)
{
	float xx = cosf(angle) * x - sinf(angle) * y;
	float yy = sinf(angle) * x + cosf(angle) * y;
	x = xx;
	y = yy;
	return *this;
}

VEC2_INLINE float Vec2::Length() const
{
	return sqrtf(x * x + y * y);
}

VEC2_INLINE float Vec2::SquaredLength() const
{
	return x * x + y * y;
}

VEC2_INLINE float Vec2::Normalize()
{
	float norm = Vec2::Length();
	*this = *this / norm;
	return norm;
}

VEC2_INLINE float Vec2::Argument() const
{
	if (x < 0.0)
	{
		return PI + atanf(y / x);
	}
	else if (fabsf(x) < EPS)
	{
		return (y > 0 ? PI : -PI);
	}
	else
	{
		return atanf(y / x);
	}
}

#endif // VEC2_INL