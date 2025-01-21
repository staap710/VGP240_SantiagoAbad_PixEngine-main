#pragma once

struct Vector2 // Vector for the x and y axis'
{
	float x, y;

	Vector2() : x(0.0f), y(0.0f) {} // -> Default coordinates = (0,0)

	Vector2(float s) : x(s), y(s) {} // -> When only one coordinate is put in both x & y default to it
	                                 // i.e; Vector2 a(10.0f); = (10, 10)

	Vector2(float x, float y) : x(x), y(y) {} // -> Allows both both x & y to be different coordinates

	Vector2(const Vector2& v) : x(v.x), y(v.y) {} // -> Allows a Vector2 to be another Vector2
												  // i.e; Vector2 a(Vector2(10.0f, 4.0f)); = (10, 4)


	Vector2 operator - () const { return Vector2(-x, -y); }

	Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }

	Vector2 operator * (float s) const { return Vector2(x * s, y * s); }
	Vector2 operator / (float s) const { return Vector2(x / s, y / s); }
	
	Vector2& operator += (const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vector2& operator -= (const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	Vector2& operator *= (float s) { x *= s; y *= s; return *this; }
	Vector2& operator /= (float s) { x /= s; y /= s; return *this; }
};
