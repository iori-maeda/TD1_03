#pragma once
class Vector2
{
public:
	Vector2() = default;
	Vector2(float x, float y)
		:x(x), y(y)
	{}
	float x = 0.0f, y = 0.0f;

	float Length() const;
	float LengthSquared() const;

	static Vector2 Normalize(const Vector2& v);
	static float Dot(const Vector2& v1, const Vector2& v2);
	static float Cross(const Vector2& v1,const Vector2& v2);
	static Vector2 Lerp(const Vector2& s, const Vector2& e, float t);

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float n);

	friend Vector2 operator+(const Vector2& v1,const Vector2& v2);
	friend Vector2 operator-(const Vector2& v1,const Vector2& v2);
	friend Vector2 operator-(const Vector2& v);
	friend Vector2 operator*(const Vector2& v,float n);
	friend Vector2 operator*(float n, const Vector2& v);
	friend Vector2 operator/(const Vector2& v,float n);
	friend Vector2 operator/(float n, const Vector2& v);
};