// ******************************************************************************
//
// Filename:	Bezier3.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   Bezier Curve with 3 points - 1 control point.
//
// Revision History:
//   Initial Revision - 03/08/08
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include "3dGeometry.h"


// Constructors
Bezier3::Bezier3()
{
}

Bezier3::Bezier3(Vector3d lStart, Vector3d lEnd, Vector3d lControl)
{
	mStartPoint = lStart;
	mEndPoint = lEnd;
	mControlPoint = lControl;
}

Bezier3::Bezier3(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float xControl, float yControl, float zControl)
{
	mStartPoint = Vector3d(xStart, yStart, zStart);
	mEndPoint = Vector3d(xEnd, yEnd, zEnd);
	mControlPoint = Vector3d(xControl, yControl, zControl);
}

// Operations
const Vector3d Bezier3::GetInterpolatedPoint(float t) const
{
	float iT = 1.0f - t;
	float b0 = iT * iT;
	float b1 = 2 * t * iT;
	float b2 = t * t;

	float lx = mStartPoint.x * b0 + mControlPoint.x * b1 + mEndPoint.x * b2;
	float ly = mStartPoint.y * b0 + mControlPoint.y * b1 + mEndPoint.y * b2;
	float lz = mStartPoint.z * b0 + mControlPoint.z * b1 + mEndPoint.z * b2;

	return Vector3d(lx, ly, lz);
}