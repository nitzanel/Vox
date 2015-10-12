// ******************************************************************************
//
// Filename:	Plane3D.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   3D Plane implementation.
//
// Revision History:
//   Initial Revision - 28/11/08
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include "3dGeometry.h"


// Constructors
Plane3D::Plane3D()
{
	d = 0.0f;
}

Plane3D::Plane3D(Vector3d lNormal, Vector3d lPoint)
{
	mNormal = lNormal;
	mPoint = lPoint;

	mNormal.Normalize();
	d = -(Vector3d::DotProduct(mNormal, mPoint));
}

Plane3D::Plane3D(Vector3d lv1, Vector3d lv2, Vector3d lv3)
{
	Vector3d aux1;
	Vector3d aux2;

	aux1 = lv1 - lv2;
	aux2 = lv3 - lv2;

	mNormal = Vector3d::CrossProduct(aux2, aux1);

	mNormal.Normalize();
	mPoint = lv2;

	d = -(Vector3d::DotProduct(mNormal, mPoint));
}

Plane3D::Plane3D(float a, float b, float c, float d)
{
	// Set the normal vector
	mNormal = Vector3d(a, b, c);

	// Compute the length of the vector
	float length = mNormal.GetLength();
	
	// Normalize the vector
	mNormal = Vector3d(a / length, b / length, c / length);

	// And divide d by the length as well
	this->d = d / length;
}


// Operations
float Plane3D::GetPointDistance(Vector3d lPoint)
{
	return (d + Vector3d::DotProduct(mNormal, lPoint));
}