// ******************************************************************************
//
// Filename:	vector2d.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   2d vector class implementation.
//
// Revision History:
//   Initial Revision - 11/03/06
//
// Copyright (c) 2005-2006, Steven Ball
//
// ******************************************************************************

#include <cmath>

#include "3dmaths.h"

#define VECTOR2D_ZERO		= Vector2d(0.0f, 0.0f)
#define VECTOR2D_ONE		= Vector2d(1.0f, 1.0f)
#define VECTOR2D_XAXIS		= Vector2d(1.0f, 0.0f)
#define VECTOR2D_YAXIS		= Vector2d(0.0f, 1.0f)

// Constructors
Vector2d::Vector2d() {
	x = 0.0f;
	y = 0.0f;
}

Vector2d::Vector2d(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2d::Vector2d(float xy[]) {
	x = xy[0];
	y = xy[1];
}


// Properties
const float Vector2d::GetLength() const {
	return sqrt((x * x) + (y * y));
}

const float Vector2d::GetLengthSquared() const {
	return (x * x) + (y * y);
}

const Vector2d Vector2d::GetUnit() const {
	Vector2d unitVector;

	float length = GetLength();

	unitVector.x = x / length;
	unitVector.y = y / length;

	return unitVector;
}

const Vector2d Vector2d::GetNegative() const {
	return Vector2d(-x, -y);
}

const Vector2d Vector2d::GetPerpendicular() const {
	return Vector2d(-y, x);
}

// Operations
void Vector2d::Negate() {
	x = -x;
	y = -y;
}

void Vector2d::Normalize() {
	float length = GetLength();

	x /= length;
	y /= length;
}

// Dot Product
float Vector2d::DotProduct(const Vector2d &v) const {
	return (x * v.x) + (y * v.y);
}

float Vector2d::DotProduct(const Vector2d &v1, const Vector2d &v2) {
	return v1.DotProduct(v2);
}

// Arithmatic
Vector2d &Vector2d::Add(const Vector2d &v1, const Vector2d &v2, Vector2d &result) {
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	
	return result;
}

Vector2d &Vector2d::Subtract(const Vector2d &v1, const Vector2d &v2, Vector2d &result) {
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	
	return result;
}

Vector2d &Vector2d::Scale(const Vector2d &v1, const float &scale, Vector2d &result) {
	result.x = v1.x * scale;
	result.y = v1.y * scale;
	
	return result;
}

bool Vector2d::equal(const Vector2d &v1, const Vector2d &v2) {
	if(fabs(v1.x - v2.x) > 1.0e-7f)
		return false;

	if(fabs(v1.y - v2.y) > 1.0e-7f)
		return false;

	return true;
}


/*
// Test the Vector2d class
void Vector2dTester() {
	// Test all constructors
	Vector2d vector1;
	Vector2d vector2(2.0f, 2.0f);
	float v[2] = {3.0f, 3.0f};
	Vector2d vector3(v);
	Vector2d vector4(0.0f, 4.0f);
	Vector2d vector5(1.0f, 0.0f);

	cout << vector1 << endl;
	cout << vector2 << endl;
	cout << vector3 << endl;

	
	// Test all return properties
	cout << vector3.GetLength() << endl;
	cout << vector3.GetLengthSquared() << endl;
	cout << vector4.GetUnit() << endl;
	cout << vector2.GetNegative() << endl;
	cout << vector4.GetPerpendicular() << endl;

	
	// Test all operations
	Vector2d tVector3 = vector3;
	Vector2d tVector4 = vector2;
	tVector3.Negate();
	tVector4.Normalize();
	cout << tVector3 << endl;
	cout << tVector4 << endl;

	
	// Test dot products
	cout << vector2.DotProduct(vector3) << endl;
	cout << Vector2d::DotProduct(vector2, vector3) << endl;
	cout << vector2 * vector3 << endl;
	

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

	if(vector4.GetPerpendicular() == Vector2d(-4.0f, 0.0f))
		cout << "equal" << endl;

	// Test streaming
	Vector2d vectorIn;
	cin >> vectorIn;
	cout << vectorIn << endl;
}
*/