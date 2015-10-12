// ******************************************************************************
//
// Filename:	Line3D.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   3D Line implementation.
//
// Revision History:
//   Initial Revision - 03/08/08
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include "3dGeometry.h"


// Constructors
Line3D::Line3D()
{
	/* Nothing */
}

Line3D::Line3D(Vector3d lStart, Vector3d lEnd)
{
	mStartPoint = lStart;
	mEndPoint = lEnd;
}

Line3D::Line3D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	mStartPoint = Vector3d(x1, y1, z1);
	mEndPoint = Vector3d(x2, y2, z2);
}

// Properties
const Vector3d Line3D::GetMidPoint() const
{
	return GetInterpolatedPoint(0.5f);
}

const Vector3d Line3D::GetVector() const
{
	return (mEndPoint - mStartPoint);
}

const float Line3D::GetLength() const
{
	return (mEndPoint - mStartPoint).GetLength();
}

const float Line3D::GetLengthSquared() const
{
	return (mEndPoint - mStartPoint).GetLengthSquared();
}

// Operations
const Vector3d Line3D::GetInterpolatedPoint(float t) const
{
	Vector3d lScale = (mEndPoint - mStartPoint) * t;

	return (mStartPoint + lScale);
}