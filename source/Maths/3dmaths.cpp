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
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include "3dmaths.h"


// Retun the normal for an arbitary polygon
Vector3d PolygonNormal(Vector3d polyData[]) {
	// Construct a normal for this polygon
	Vector3d v1 = polyData[0] - polyData[1];
	Vector3d v2 = polyData[1] - polyData[2];

	Vector3d normal;
	normal = Vector3d::CrossProduct(v1, v2);

	// Make sure this normal is a unit vector
	normal.Normalize();

	return normal;
}

// Return the closest point on a line that a 3d point is (NOTE - can return either end of the line if they are closest)
Vector3d ClostestPointOnLine(Vector3d p1, Vector3d p2, Vector3d point) {
	// Create some support vectors
	Vector3d v1 = point - p1;
	Vector3d v2 = p2 - p1;

	v2 = v2.GetUnit();

	// Get the distance of this line
	float distance = (p2 - p1).GetLength();

	// Project v1 onto v2.
	float t = Vector3d::DotProduct(v2, v1);

	// If the projection is less than zero, the closest point is end p1
	if(t <= 0.0f)
		return p1;

	// If the projection is greater than the line distance, the clostest point is end p2
	if(t >= distance)
		return p2;

	// Create a vector in direction v2 of magnitude t
	Vector3d projection = v2 * t;

	// Get to the closest edge point by going from p1 along the projection
	Vector3d closestPoint = p1 + projection;

	return closestPoint;
}

// Returns the point of intersection of a line and a plane
Vector3d LinePlaneIntersectionPoint(Vector3d normal, Vector3d lineStart, Vector3d lineEnd, float distance) {
	Vector3d intersectionPoint;
	Vector3d lineDirection;
	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	lineDirection = lineEnd - lineStart;  // Get the Vector of the line
	lineDirection = lineDirection.GetUnit();  // Normalize the line vector

	// Use the plane equation with the normal and the line
	Numerator = -(normal.x * lineStart.x +
				  normal.y * lineStart.y +
				  normal.z * lineStart.z + distance);

	Denominator = Vector3d::DotProduct(normal, lineDirection);

	if( Denominator == 0.0)	 // Dont divide by zero
		return lineStart;

	dist = Numerator / Denominator;

	// Travel along the lineDirection by dist and add this to the line start
	intersectionPoint.x = (float)(lineStart.x + (lineDirection.x * dist));
	intersectionPoint.y = (float)(lineStart.y + (lineDirection.y * dist));
	intersectionPoint.z = (float)(lineStart.z + (lineDirection.z * dist));

	return intersectionPoint;
}