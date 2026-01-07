#include "Vector2.h"
#include <cmath>

using namespace std;

float Vector2::Length() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::SqetLength() const
{
	return x * x + y * y;
}

Vector2 Vector2::Normalize(const Vector2 &v)
{
	float len = v.Length();

	if (len <= 0.0f)return Vector2();

	return Vector2(v.x / len, v.y / len);
}

float Vector2::Dot(const Vector2 &v1, const Vector2 &v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float Vector2::Cross(const Vector2 &v1, const Vector2 &v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

Vector2 Vector2::Lerp(const Vector2 &s, const Vector2 &e, float t)
{
	if (t <= 0.0f)return s;
	if (t >= 1.0f)return e;
	return s + (e - s) * t;
}

void Vector2::operator+=(const Vector2 &v)
{
	x += v.x;
	y += v.y;
}

void Vector2::operator*=(float n)
{
	x *= n;
	y *= n;
}

Vector2 operator+(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 operator-(const Vector2 &v1, const Vector2 &v2)
{
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 operator-(const Vector2 &v)
{
	return Vector2(-v.x, -v.y);
}

Vector2 operator*(const Vector2 &v, float n)
{
	return Vector2(v.x * n, v.y * n);
}

Vector2 operator*(float n, const Vector2 &v)
{
	return Vector2(n * v.x, n * v.y);
}

Vector2 operator/(const Vector2 &v, float n)
{
	return Vector2(v.x / n, v.y / n);
}

Vector2 operator/(float n, const Vector2 &v)
{
	return Vector2(n / v.x, n / v.y);
}