#pragma once

struct Vector3 // Vector for the x, y and z axis'
{
	float x, y, z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {} 

	Vector3(float s) : x(s), y(s), z(s) {} 
		  
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {} 
		  
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}


	Vector3 operator - () const { return Vector3(-x, -y, -z); }
		  
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
		  
	Vector3 operator * (float s) const { return Vector3(x * s, y * s, z * s); }
	Vector3 operator / (float s) const { return Vector3(x / s, y / s, z / s); }
		  
	Vector3& operator += (const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vector3& operator -= (const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	Vector3& operator *= (float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator /= (float s) { x /= s; y /= s; z /= s; return *this; }
};