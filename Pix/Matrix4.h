#pragma once
#include <math.h>
#include "Vector3.h"

struct Matrix4
{
    float _11, _12, _13, _14,
        _21, _22, _23, _24,
        _31, _32, _33, _34,
        _41, _42, _43, _44;

    Matrix4(float a11, float a12, float a13, float a14,
        float a21, float a22, float a23, float a24,
        float a31, float a32, float a33, float a34,
        float a41, float a42, float a43, float a44)
        : _11(a11), _12(a12), _13(a13), _14(a14),
        _21(a21), _22(a22), _23(a23), _24(a24),
        _31(a31), _32(a32), _33(a33), _34(a34),
        _41(a41), _42(a42), _43(a43), _44(a44)
    {

    }

    Matrix4()
        :Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f)
    {

    }

    Matrix4 operator+(const Matrix4& m) const
    {
        return Matrix4(
            _11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
            _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
            _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
            _41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44);
    }

    Matrix4 operator* (float s) const
    {
        return Matrix4(
            _11 * s, _12 * s, _13 * s, _14 * s,
            _21 * s, _22 * s, _23 * s, _24 * s,
            _31 * s, _32 * s, _33 * s, _34 * s,
            _41 * s, _42 * s, _43 * s, _44 * s);

    }
    static Matrix4 Identity()
    {
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Matrix4 RotationX(float rad)
    {
        const float s = sinf(rad);
        const float c = cosf(rad);
        return Matrix4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

	static Matrix4 RotationY(float rad)
	{
		const float s = sinf(rad);
		const float c = cosf(rad);
		return Matrix4(
			c, 0.0f, -s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			s, 0.0f, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	static Matrix4 RotationZ(float rad)
	{
		const float s = sinf(rad);
		const float c = cosf(rad);
		return Matrix4(
			c, s, 0.0f, 0.0f,
			-s, c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	} 

	static Matrix4 Scale(float s)
	{
		return Matrix4(
			s, 0.0f, 0.0f, 0.0f,
			0.0f, s, 0.0f, 0.0f,
			0.0f, 0.0f, s, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

    static Matrix4 Scale(const Vector3& s) {
		return Matrix4(
			s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
    }
    static Matrix4 Translation(float dx, float dy, float dz) {
		return Matrix4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			dx, dy, dz, 1.0f);
    }

    static Matrix4 Translation(const Vector3& d) {
		return Matrix4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			d.x, d.y, d.z, 1.0f);
    }
};

