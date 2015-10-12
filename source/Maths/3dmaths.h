// ******************************************************************************
//
// Filename:	3dmaths.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   3d maths library.
//
// Revision History:
//   Initial Revision - 11/03/06
//
// Copyright (c) 2005-2006, Steven Ball
//
// ******************************************************************************

#pragma once

#include <iostream>
using namespace std;

const float PI = 3.14159265358979323846f;


// Conversions
template <class T>
inline T DegToRad(const T &degrees) {
	return (degrees * PI) / 180;
}

template <class T>
inline T RadToDeg(const T &radians) {
	return (radians * 180) / PI;
}

template <class T>
inline void Swap(T &v1, T &v2) {
	T temp = v1;
	v1 = v2;
	v2 = temp;
}


class Vector2d {
public:
	// Constructors
	Vector2d();
	Vector2d(float x, float y);
	Vector2d(float xy[]);

	// Properties
	const float GetLength() const;
	const float GetLengthSquared() const;
	const Vector2d GetUnit() const;
	const Vector2d GetNegative() const;
	const Vector2d GetPerpendicular() const;

	// Operations
	void Negate();
	void Normalize();

	// Dot Product
	float DotProduct(const Vector2d &v) const;
	static float DotProduct(const Vector2d &v1, const Vector2d &v2);

	// Arithmetic
	static Vector2d &Add(const Vector2d &v1, const Vector2d &v2, Vector2d &result);
	static Vector2d &Subtract(const Vector2d &v1, const Vector2d &v2, Vector2d &result);
	static Vector2d &Scale(const Vector2d &v1, const float &scale, Vector2d &result);
	static bool equal(const Vector2d &v1, const Vector2d &v2);

	// Operators
	Vector2d operator+(const Vector2d &v) const { Vector2d result; return Add(*this, v, result); }
	Vector2d& operator+=(const Vector2d &v) { Add(*this, v, *this); return *this; }
	Vector2d operator-(const Vector2d &v) const { Vector2d result; return Subtract(*this, v, result); }
	Vector2d& operator-=(const Vector2d &v) { Subtract(*this, v, *this); return *this; }
	Vector2d operator*(const float &sca) const { Vector2d result; return Scale(*this, sca, result); }
	Vector2d& operator*=(const float &sca) { Scale(*this, sca, *this); return *this; }
	Vector2d operator/(const float &sca) const { Vector2d result; return Scale(*this, (1/sca), result); }
	Vector2d& operator/=(const float &sca) { Scale(*this, (1/sca), *this); return *this; }
	float operator*(const Vector2d &v) const { return DotProduct(*this, v); }
	Vector2d operator-() const { return GetNegative(); }
	bool operator==(const Vector2d &v) const { return equal(*this, v); };
	bool operator!=(const Vector2d &v) const { return !equal(*this, v); };

public:
	float x, y;
};


class Vector3d {
public:
	// Constructors
	Vector3d();
	Vector3d(float x, float y, float z);
	Vector3d(float xyz[]);

	// Properties
	const float GetLength() const;
	const float GetLengthSquared() const;
	const Vector3d GetUnit() const;
	const Vector3d GetNegative() const;

	// Operations
	void Negate();
	void Normalize();

	// Dot / Cross Product
	float DotProduct(const Vector3d &v) const;
	static float DotProduct(const Vector3d &v1, const Vector3d &v2);
	Vector3d CrossProduct(const Vector3d &v) const;
	static Vector3d CrossProduct(const Vector3d &v1, const Vector3d &v2);

	// Arithmetic
	static Vector3d &Add(const Vector3d &v1, const Vector3d &v2, Vector3d &result);
	static Vector3d &Subtract(const Vector3d &v1, const Vector3d &v2, Vector3d &result);
	static Vector3d &Scale(const Vector3d &v1, const float &scale, Vector3d &result);
	static bool equal(const Vector3d &v1, const Vector3d &v2);

	// Operators
	Vector3d operator+(const Vector3d &v) const { Vector3d result; return Add(*this, v, result); }
	Vector3d& operator+=(const Vector3d &v) { Add(*this, v, *this); return *this; }
	Vector3d operator-(const Vector3d &v) const { Vector3d result; return Subtract(*this, v, result); }
	Vector3d& operator-=(const Vector3d &v) { Subtract(*this, v, *this); return *this; }
	Vector3d operator*(const float &sca) const { Vector3d result; return Scale(*this, sca, result); }
	Vector3d& operator*=(const float &sca) { Scale(*this, sca, *this); return *this; }
	Vector3d operator/(const float &sca) const { Vector3d result; return Scale(*this, (1/sca), result); }
	Vector3d& operator/=(const float &sca) { Scale(*this, (1/sca), *this); return *this; }
	Vector3d operator^(const Vector3d &v) const { Vector3d result; return CrossProduct(*this, v); }
	float operator*(const Vector3d &v) const { return DotProduct(*this, v); }
	Vector3d operator-() const { return GetNegative(); }
	bool operator==(const Vector3d &v) const { return equal(*this, v); };
	bool operator!=(const Vector3d &v) const { return !equal(*this, v); };

public:
	float x, y, z;
};


class Matrix3x3 {
public:
	// Constructors
	Matrix3x3();
	Matrix3x3(float m[9]);

	// Setup matrices
	void SetXRotation(const float x);
	void SetYRotation(const float y);
	void SetZRotation(const float z);
	void SetRotation(const float x, const float y, const float z);

	// Properties
	void GetMatrix(float* m) const;
	const float GetDeterminant() const;
	const Matrix3x3 GetNegative() const;
	const Matrix3x3 GetTranspose() const;
	const Matrix3x3 GetInverse() const;

	// Operations
	void LoadIdentity();
	void Negate();
	void Transpose();
	void Inverse();

	// Arithmetic
	static Matrix3x3 &Add(const Matrix3x3 &m1, const Matrix3x3 &m2, Matrix3x3 &result);
	static Matrix3x3 &Subtract(const Matrix3x3 &m1, const Matrix3x3 &m2, Matrix3x3 &result);
	static Matrix3x3 &Scale(const Matrix3x3 &m1, const float &scale, Matrix3x3 &result);
	static Matrix3x3 &Multiply(const Matrix3x3 &m1, const Matrix3x3 &m2, Matrix3x3 &result);
	static Vector3d &Multiply(const Matrix3x3 &m1, const Vector3d &v, Vector3d &result);
	static bool equal(const Matrix3x3 &m1, const Matrix3x3 &m2);

	// Operators
	Matrix3x3 operator+(const Matrix3x3 &m) const { Matrix3x3 result; return Add(*this, m, result); }
	Matrix3x3& operator+=(const Matrix3x3 &m) { Add(*this, m, *this); return *this; }
	Matrix3x3 operator-(const Matrix3x3 &m) const { Matrix3x3 result; return Subtract(*this, m, result); }
	Matrix3x3& operator-=(const Matrix3x3 &m) { Subtract(*this, m, *this); return *this; }
	Matrix3x3 operator*(const float &sca) const { Matrix3x3 result; return Scale(*this, sca, result); }
	Matrix3x3& operator*=(const float &sca) { Scale(*this, sca, *this); return *this; }
	Matrix3x3 operator/(const float &sca) const { Matrix3x3 result; return Scale(*this, (1/sca), result); }
	Matrix3x3& operator/=(const float &sca) { Scale(*this, (1/sca), *this); return *this; }
	Matrix3x3 operator*(const Matrix3x3 &m) { Matrix3x3 result; return Multiply(*this, m, result); }
	Vector3d operator*(const Vector3d &v) { Vector3d result; return Multiply(*this, v, result); }
	bool operator==(const Matrix3x3 &m) const { return equal(*this, m); };
	bool operator!=(const Matrix3x3 &m) const { return !equal(*this, m); };

public:
	float m[9];
};


class Matrix4x4 {
public:
	// Constructors
	Matrix4x4();
	Matrix4x4(float m[16]);

	// Setup matrices
	void SetXRotation(const float x);
	void SetYRotation(const float y);
	void SetZRotation(const float z);
	void SetRotation(const float x, const float y, const float z);
	void SetTranslation(float trans[3]);
	void SetTranslation(Vector3d trans);
	void SetScale(Vector3d scale);

	void AddTranslation(float *translation);
	void AddRotationRadians(float *angles);

	// Properties
	void GetMatrix(float* m) const;
	const float GetDeterminant() const;
	const Matrix4x4 GetNegative() const;
	const Matrix4x4 GetTranspose() const;
	const Matrix4x4 GetInverse() const;
	const Matrix4x4 GetOrthoNormal() const;

	const Vector3d GetRightVector() const;
	const Vector3d GetUpVector() const;
	const Vector3d GetForwardVector() const;
	const Vector3d GetTranslationVector() const;
	const void GetEuler(float *x, float *y, float *z) const;

	// Operations
	void LoadIdentity();
	void Negate();
	void Transpose();
	void Inverse();
	void OrthoNormalize();

	void SetValues(float m[16]);						// Set the values of the matrix
	void PostMultiply(Matrix4x4& matrix);				// Post multiple with another matrix
	void InverseTranslateVector(float *pVect);			// Translate a vector by the inverse of the translation part of this matrix.
	void InverseRotateVector(float *pVect);				// Rotate a vector by the inverse of the rotation part of this matrix.
	void SetValues_RotALL(float x, float y, float z);	// Generate rotation matrix
	void SetRotationRadians(float *angles);				// Set the Rotation matrix

	// Arithmetic
	static Matrix4x4 &Add(const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &result);
	static Matrix4x4 &Subtract(const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &result);
	static Matrix4x4 &Scale(const Matrix4x4 &m1, const float &scale, Matrix4x4 &result);
	static Matrix4x4 &Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2, Matrix4x4 &result);
	static Vector3d &Multiply(const Matrix4x4 &m1, const Vector3d &v, Vector3d &result);
	static bool equal(const Matrix4x4 &m1, const Matrix4x4 &m2);

	// Operators
	Matrix4x4 operator+(const Matrix4x4 &m) const { Matrix4x4 result; return Add(*this, m, result); }
	Matrix4x4& operator+=(const Matrix4x4 &m) { Add(*this, m, *this); return *this; }
	Matrix4x4 operator-(const Matrix4x4 &m) const { Matrix4x4 result; return Subtract(*this, m, result); }
	Matrix4x4& operator-=(const Matrix4x4 &m) { Subtract(*this, m, *this); return *this; }
	Matrix4x4 operator*(const float &sca) const { Matrix4x4 result; return Scale(*this, sca, result); }
	Matrix4x4& operator*=(const float &sca) { Scale(*this, sca, *this); return *this; }
	Matrix4x4 operator/(const float &sca) const { Matrix4x4 result; return Scale(*this, (1/sca), result); }
	Matrix4x4& operator/=(const float &sca) { Scale(*this, (1/sca), *this); return *this; }
	Matrix4x4 operator*(const Matrix4x4 &m) { Matrix4x4 result; return Multiply(*this, m, result); }
	Vector3d operator*(const Vector3d &v) { Vector3d result; return Multiply(*this, v, result); }
	bool operator==(const Matrix4x4 &m) const { return equal(*this, m); };
	bool operator!=(const Matrix4x4 &m) const { return !equal(*this, m); };

public:
	float m[16];
};


class Quaternion {
public:
	// Constructors
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(float xyzw[]);

	// Setup quaternion
	void SetEuler(float x, float y, float z);
	void SetAxis(Vector3d v, float angle);

	// Properties
	const float GetLength() const;
	const Quaternion GetUnit() const;
	const Quaternion GetConjugate() const;
	void GetEuler(float &x, float &y, float &z) const;
	void GetAxis(Vector3d &v, float &angle) const;
	const Matrix4x4 GetMatrix() const;

	// Operations
	void LoadIdentity();
	void Normalize();
	void Conjugate();

	// Spherical Linear Interpolation
	static Quaternion Slerp(Quaternion &q1, Quaternion &q2, float time);
	Quaternion Slerp(Quaternion &q, float time);

	// Arithmetic
	static Quaternion &Add(const Quaternion &q1, const Quaternion &q2, Quaternion &result);
	static Quaternion &Subtract(const Quaternion &q1, const Quaternion &q2, Quaternion &result);
	static Quaternion &Scale(const Quaternion &q1, const float &scale, Quaternion &result);
	static Quaternion &Multiply(const Quaternion&q1, const Quaternion &q2, Quaternion &result);
	static Vector3d &Multiply(const Quaternion &q1, const Vector3d &v, Vector3d &result);
	static bool equal(const Quaternion &q1, const Quaternion &q2);

	// Operators
	Quaternion operator+(const Quaternion &q) const { Quaternion result; return Add(*this, q, result); }
	Quaternion& operator+=(const Quaternion &q) { Add(*this, q, *this); return *this; }
	Quaternion operator-(const Quaternion &q) const { Quaternion result; return Subtract(*this, q, result); }
	Quaternion& operator-=(const Quaternion &q) { Subtract(*this, q, *this); return *this; }
	Quaternion operator*(const float &sca) const { Quaternion result; return Scale(*this, sca, result); }
	Quaternion& operator*=(const float &sca) { Scale(*this, sca, *this); return *this; }
	Quaternion operator/(const float &sca) const { Quaternion result; return Scale(*this, (1/sca), result); }
	Quaternion& operator/=(const float &sca) { Scale(*this, (1/sca), *this); return *this; }
	Quaternion operator*(const Quaternion &q) { Quaternion result; return Multiply(*this, q, result); }
	Vector3d operator*(const Vector3d &v) { Vector3d result; return Multiply(*this, v, result); }
	bool operator==(const Quaternion &q) const { return equal(*this, q); };
	bool operator!=(const Quaternion &q) const { return !equal(*this, q); };

public:
	float x, y, z, w;
};


// 3D Maths Helper functions
//-----------------------------

// Retun the normal for an arbitary polygon
Vector3d PolygonNormal(Vector3d polyData[]);

// Return the closest point on a line that a 3d point is (NOTE - can return either end of the line if they are closest)
Vector3d ClostestPointOnLine(Vector3d p1, Vector3d p2, Vector3d point);

// Returns the point of intersection of a line and a plane
Vector3d LinePlaneIntersectionPoint(Vector3d normal, Vector3d lineStart, Vector3d lineEnd, float distance);