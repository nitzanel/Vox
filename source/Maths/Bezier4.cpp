// ******************************************************************************
//
// Filename:	Bezier4.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   Bezier Curve with 4 points - 2 control points.
//
// Revision History:
//   Initial Revision - 03/08/08
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include "3dGeometry.h"


// Constructors
Bezier4::Bezier4()
{
}

Bezier4::Bezier4(Vector3d lStart, Vector3d lEnd, Vector3d lControl1, Vector3d lControl2)
{
	mStartPoint = lStart;
	mEndPoint = lEnd;
	mControlPoint1 = lControl1;
	mControlPoint2 = lControl2;
}

Bezier4::Bezier4(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float xControl1, float yControl1, float zControl1, float xControl2, float yControl2, float zControl2)
{
	mStartPoint = Vector3d(xStart, yStart, zStart);
	mEndPoint = Vector3d(xEnd, yEnd, zEnd);
	mControlPoint1 = Vector3d(xControl1, yControl1, zControl1);
	mControlPoint2 = Vector3d(xControl2, yControl2, zControl2);
}

// Operations
const Vector3d Bezier4::GetInterpolatedPoint(float t) const
{
	float iT = 1.0f - t;
	float b0 = iT * iT * iT;
	float b1 = 3 * t * iT * iT;
	float b2 = 3 * t * t * iT;
	float b3 = t * t * t;

	float lx = mStartPoint.x * b0 + mControlPoint1.x * b1 + mControlPoint2.x * b2 + mEndPoint.x * b3;
	float ly = mStartPoint.y * b0 + mControlPoint1.y * b1 + mControlPoint2.y * b2 + mEndPoint.y * b3;
	float lz = mStartPoint.z * b0 + mControlPoint1.z * b1 + mControlPoint2.z * b2 + mEndPoint.z * b3;

	return Vector3d(lx, ly, lz);
}