// ******************************************************************************
//
// Filename:	vector2d.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   Quaternion class implementation.
//
// Revision History:
//   Initial Revision - 11/03/06
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include <cmath>

#include "3dmaths.h"


// Constructors
Quaternion::Quaternion() {
	LoadIdentity();
}

Quaternion::Quaternion(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(float xyzw[]) {
	x = xyzw[0];
	y = xyzw[1];
	z = xyzw[2];
	w = xyzw[3];
}


// Setup quaternion
void Quaternion::SetEuler(float x, float y, float z) {
	double	ex, ey, ez;
	double	cr, cp, cy, sr, sp, sy, cpcy, spsy;

	ex = DegToRad(x) / 2.0;
	ey = DegToRad(y) / 2.0;
	ez = DegToRad(z) / 2.0;

	cr = cos(ex);
	cp = cos(ey);
	cy = cos(ez);

	sr = sin(ex);
	sp = sin(ey);
	sy = sin(ez);

	cpcy = cp * cy;
	spsy = sp * sy;

	this->x = float(sr * cpcy - cr * spsy);
	this->y = float(cr * sp * cy + sr * cp * sy);
	this->z = float(cr * cp * sy - sr * sp * cy);

	this->w = float(cr * cpcy + sr * spsy);
}

void Quaternion::SetAxis(Vector3d v, float angle) {
	float x, y, z;
	double rad, scale;

	v.Normalize();


	rad	= DegToRad(angle) / 2;
	scale = sin(rad);

	x = v.x;
	y = v.y;
	z = v.z;

	this->x = float(x * scale);
	this->y = float(y * scale);
	this->z = float(z * scale);

	this->w = (float)cos(rad);
}


// Properties
const float Quaternion::GetLength() const {
	return (float)sqrt( (x * x) + (y * y) + (z * z) + (w * w) );
}

const Quaternion Quaternion::GetUnit() const {
	Quaternion unit;

	float length = GetLength();

	unit.x = x / length;
	unit.y = y / length;
	unit.z = z / length;
	unit.w = w / length;

	return unit;
}

const Quaternion Quaternion::GetConjugate() const {
	return Quaternion(-x, -y, -z, w);
}

void Quaternion::GetEuler(float &x, float &y, float &z) const {
	// TODO : Write... Quaternion::GetEuler
}

void Quaternion::GetAxis(Vector3d &v, float &angle) const {
	float temp_angle;
	float scale;

	temp_angle = acos(w);

	scale = (float)sqrt((x * x) + (y * y) + (z * z));

	if(fabs(scale) < 1.0e-7f) {
		angle = 0.0f;

		v.x = 0.0f;
		v.y = 0.0f;
		v.z = 1.0f;
	}
	else {
		angle = temp_angle * 2.0f;

		v.x = x / scale;
		v.y = y / scale;
		v.z = z / scale;

		v.Normalize();
	}
}

const Matrix4x4 Quaternion::GetMatrix() const {
	float x2, y2, z2, w2, xy, xz, yz, wx, wy, wz;

	float matrix[16];

	memset(matrix, 0, 16 * sizeof(float));
	matrix[15] = 1;

	x2	= (x * x);
	y2	= (y * y);
	z2	= (z * z);
	w2	= (w * w);

	xy	= x * y;
	xz	= x * z;
	yz	= y * z;
	wx	= w * x;
	wy	= w * y;
	wz	= w * z;


	matrix[0]	= (1 - 2 * (y2 + z2));
	matrix[1]	= (2 * (xy + wz));
	matrix[2]	= (2 * (xz - wy));

	matrix[4]	= (2 * (xy - wz));
	matrix[5]	= (1 - 2 * (x2 + z2));
	matrix[6]	= (2 * (yz + wx));

	matrix[8]	= (2 * (xz + wy));
	matrix[9]	= (2 * (yz - wx));
	matrix[10]	= (1 - 2 * (x2 + y2));

	Matrix4x4 result(matrix);

	return result;
}


// Operations
void Quaternion::LoadIdentity() {
	x	= 0.0f;
	y	= 0.0f;
	z	= 0.0f;

	w	= 1.0f;
}

void Quaternion::Normalize() {
	Quaternion unit;

	float length = GetLength();

	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

void Quaternion::Conjugate() {
	x = -x;
	y = -y;
	z = -z;
}


// Spherical Linear Interpolation
Quaternion Quaternion::Slerp(Quaternion &q1, Quaternion &q2, float time) {
	if(q1 == q2) 
		return q1;

	Quaternion qInterpolated;

	float result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

	if(result < 0.0f) {
		q2 = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
		result = -result;
	}

	float scale0 = 1 - time, scale1 = time;

	if(1 - result > 0.1f) {
		float theta = (float)acos(result);
		float sinTheta = (float)sin(theta);

		scale0 = (float)sin( ( 1 - time ) * theta) / sinTheta;
		scale1 = (float)sin( ( time * theta) ) / sinTheta;
	}	

	qInterpolated.x = (scale0 * q1.x) + (scale1 * q2.x);
	qInterpolated.y = (scale0 * q1.y) + (scale1 * q2.y);
	qInterpolated.z = (scale0 * q1.z) + (scale1 * q2.z);
	qInterpolated.w = (scale0 * q1.w) + (scale1 * q2.w);

	return qInterpolated;
}

Quaternion Quaternion::Slerp(Quaternion &q, float time) {
	if((*this) == q) 
		return q;

	Quaternion qInterpolated;

	float result = (this->x * q.x) + (this->y * q.y) + (this->z * q.z) + (this->w * q.w);

	if(result < 0.0f) {
		q = Quaternion(-q.x, -q.y, -q.z, -q.w);
		result = -result;
	}

	float scale0 = 1 - time, scale1 = time;

	if(1 - result > 0.1f) {
		float theta = (float)acos(result);
		float sinTheta = (float)sin(theta);

		scale0 = (float)sin( ( 1 - time ) * theta) / sinTheta;
		scale1 = (float)sin( ( time * theta) ) / sinTheta;
	}	

	qInterpolated.x = (scale0 * this->x) + (scale1 * q.x);
	qInterpolated.y = (scale0 * this->y) + (scale1 * q.y);
	qInterpolated.z = (scale0 * this->z) + (scale1 * q.z);
	qInterpolated.w = (scale0 * this->w) + (scale1 * q.w);

	return qInterpolated;
}


// Arithmatic
Quaternion &Quaternion::Add(const Quaternion &q1, const Quaternion &q2, Quaternion &result) {
	result.x = q1.x + q2.x;
	result.y = q1.y + q2.y;
	result.z = q1.z + q2.z;
	result.w = q1.w + q2.w;

	return result;
}

Quaternion &Quaternion::Subtract(const Quaternion &q1, const Quaternion &q2, Quaternion &result) {
	result.x = q1.x - q2.x;
	result.y = q1.y - q2.y;
	result.z = q1.z - q2.z;
	result.w = q1.w - q2.w;

	return result;
}

Quaternion &Quaternion::Scale(const Quaternion &q1, const float &scale, Quaternion &result) {
	result.x = q1.x * scale;
	result.y = q1.y * scale;
	result.z = q1.z * scale;
	result.w = q1.w * scale;

	return result;
}

Quaternion &Quaternion::Multiply(const Quaternion&q1, const Quaternion &q2, Quaternion &result) {
	result.x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
	result.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
	result.z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
	result.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;

	return result;
}

Vector3d &Quaternion::Multiply(const Quaternion &q1, const Vector3d &v, Vector3d &result) {

	Quaternion vecQuat, resQuat;
	vecQuat.x = v.x;
	vecQuat.y = v.y;
	vecQuat.z = v.z;
	vecQuat.w = 0.0f;

	resQuat = vecQuat * q1.GetConjugate();
	resQuat = Quaternion::Multiply(q1, resQuat, resQuat);

	result = Vector3d(resQuat.x, resQuat.y, resQuat.z);

	return(result);
}

bool Quaternion::equal(const Quaternion &q1, const Quaternion &q2) {
	if(fabs(q1.x - q2.x) > 1.0e-7f)
		return false;

	if(fabs(q1.y - q2.y) > 1.0e-7f)
		return false;

	if(fabs(q1.z - q2.z) > 1.0e-7f)
		return false;

	if(fabs(q1.w - q2.w) > 1.0e-7f)
		return false;

	return true;
}

/*
// Test the Quaternion class
void QuaternionTester() {
	// TODO : Write... QuaternionTester()

	// Test all constructors
}
*/