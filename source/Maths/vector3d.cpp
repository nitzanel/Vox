// ******************************************************************************
//
// Filename:	vector2d.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   3d vector class implementation.
//
// Revision History:
//   Initial Revision - 11/03/06
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include <cmath>

#include "3dmaths.h"


#define VECTOR3D_ZERO		= Vector3d(0.0f, 0.0f, 0.0f)
#define VECTOR3D_ONE		= Vector3d(1.0f, 1.0f, 1.0f)
#define VECTOR3D_XAXIS		= Vector3d(1.0f, 0.0f, 0.0f)
#define VECTOR3D_YAXIS		= Vector3d(0.0f, 1.0f, 0.0f)
#define VECTOR3D_ZAXIS		= Vector3d(0.0f, 0.0f, 1.0f)


// Constructors
Vector3d::Vector3d() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3d::Vector3d(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3d::Vector3d(float xyz[]) {
	x = xyz[0];
	y = xyz[1];
	z = xyz[2];
}


// Properties
const float Vector3d::GetLength() const {
	return sqrt((x * x) + (y * y) + (z * z));
}

const float Vector3d::GetLengthSquared() const {
	return (x * x) + (y * y) + (z * z);
}

const Vector3d Vector3d::GetUnit() const {
	Vector3d unitVector;

	float length = GetLength();

	unitVector.x = x / length;
	unitVector.y = y / length;
	unitVector.z = z / length;

	return unitVector;
}

const Vector3d Vector3d::GetNegative() const {
	return Vector3d(-x, -y, -z);
}


// Operations
void Vector3d::Negate() {
	x = -x;
	y = -y;
	z = -z;
}

void Vector3d::Normalize() {
	float length = GetLength();

	x /= length;
	y /= length;
	z /= length;
}


// Dot / Cross Product
float Vector3d::DotProduct(const Vector3d &v) const {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

float Vector3d::DotProduct(const Vector3d &v1, const Vector3d &v2) {
	return v1.DotProduct(v2);
}

Vector3d Vector3d::CrossProduct(const Vector3d &v) const {
	return Vector3d::CrossProduct(*this, v);
}

Vector3d Vector3d::CrossProduct(const Vector3d &v1, const Vector3d &v2) {
	Vector3d result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	
	return result;
}


// Arithmatic
Vector3d &Vector3d::Add(const Vector3d &v1, const Vector3d &v2, Vector3d &result) {
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	
	return result;
}

Vector3d &Vector3d::Subtract(const Vector3d &v1, const Vector3d &v2, Vector3d &result) {
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	
	return result;
}

Vector3d &Vector3d::Scale(const Vector3d &v1, const float &scale, Vector3d &result) {
	result.x = v1.x * scale;
	result.y = v1.y * scale;
	result.z = v1.z * scale;
	
	return result;
}

bool Vector3d::equal(const Vector3d &v1, const Vector3d &v2) {
	if(fabs(v1.x - v2.x) > 1.0e-7f)
		return false;

	if(fabs(v1.y - v2.y) > 1.0e-7f)
		return false;

	if(fabs(v1.z - v2.z) > 1.0e-7f)
		return false;

	return true;
}


/*
// Test the Vector3d class
void Vector3dTester() {
	// Test all constructors
	Vector3d vector1;
	Vector3d vector2(2.0f, 2.0f, 2.0f);
	float v[3] = {3.0f, 3.0f, 3.0f};
	Vector3d vector3(v);
	Vector3d vector4(0.0f, 4.0f, 0.0f);
	Vector3d vector5(1.0f, 0.0f, 0.0f);

	cout << vector1 << endl;
	cout << vector2 << endl;
	cout << vector3 << endl;

	// Test all return properties
	cout << vector3.GetLength() << endl;
	cout << vector3.GetLengthSquared() << endl;
	cout << vector4.GetUnit() << endl;
	cout << vector2.GetNegative() << endl;

	// Test all operations
	Vector3d tVector3 = vector3;
	Vector3d tVector4 = vector4;
	tVector3.Negate();
	tVector4.Normalize();
	cout << tVector3 << endl;
	cout << tVector4 << endl;

	// Test dot and cross products
	cout << vector2.DotProduct(vector3) << endl;
	cout << Vector3d::DotProduct(vector2, vector3) << endl;
	cout << vector2 * vector3 << endl;
	cout << vector4.CrossProduct(vector5) << endl;
	cout << Vector3d::CrossProduct(vector4, vector5) << endl;
	cout << (vector2 ^ vector4) << endl;
	

	// Test operators
	cout << vector2 + vector3 << endl;
	cout << vector3 - vector2 << endl;
	cout << vector4 * 10.0f << endl;
	cout << vector5 / 5 << endl;
	cout << -vector5 << endl;

	if(vector3 == vector2) 
		cout << "equal" << endl;
	else if(vector3 != vector2) 
		cout << "not equal" << endl;

	if(vector2 == Vector3d(2.0f, 2.0f, 2.0f))
		cout << "equal" << endl;

	// Test streaming
	Vector3d vectorIn;
	cin >> vectorIn;
	cout << vectorIn << endl;
}
*/