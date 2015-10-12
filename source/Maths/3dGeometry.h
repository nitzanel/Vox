// ******************************************************************************
//
// Filename:	3dGeometry.cpp
// Project:		3DMaths
// Author:		Steven Ball
//
// Purpose:
//   3d geometry functionality.
//
// Revision History:
//   Initial Revision - 03/08/08
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#pragma once

#include "3dmaths.h"


class Plane3D
{
public:
	// Constructors
	Plane3D();
	Plane3D(Vector3d lNormal, Vector3d lPoint);
	Plane3D(Vector3d lv1, Vector3d lv2, Vector3d lv3);
	Plane3D(float a, float b, float c, float d);

	// Operations
	float GetPointDistance(Vector3d lPoint);

public:
	Vector3d mPoint;
	Vector3d mNormal;
	float d;
};


class Line3D
{
public:
	// Constructors
	Line3D();
	Line3D(Vector3d lStart, Vector3d lEnd);
	Line3D(float x1, float y1, float z1, float x2, float y2, float z2);

	// Properties
	const Vector3d GetMidPoint() const;
	const Vector3d GetVector() const;
	const float GetLength() const;
	const float GetLengthSquared() const;

	// Operations
	const Vector3d GetInterpolatedPoint(float t) const;

public:
	Vector3d mStartPoint;
	Vector3d mEndPoint;
};


class Bezier3
{
public:
	// Constructors
	Bezier3();
	Bezier3(Vector3d lStart, Vector3d lEnd, Vector3d lControl);
	Bezier3(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float xControl, float yControl, float zControl);

	// Operations
	const Vector3d GetInterpolatedPoint(float t) const;

public:
	Vector3d mStartPoint;
	Vector3d mEndPoint;
	Vector3d mControlPoint;
};


class Bezier4
{
public:
	// Constructors
	Bezier4();
	Bezier4(Vector3d lStart, Vector3d lEnd, Vector3d lControl1, Vector3d lControl2);
	Bezier4(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float xControl1, float yControl1, float zControl1, float xControl2, float yControl2, float zControl2);

	// Operations
	const Vector3d GetInterpolatedPoint(float t) const;

public:
	Vector3d mStartPoint;
	Vector3d mEndPoint;
	Vector3d mControlPoint1;
	Vector3d mControlPoint2;
};