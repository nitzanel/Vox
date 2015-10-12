// Juice Engine by Steven Ball
// Frustum - A geometric implementation of the viewing frustum for a viewport
// Created : 28/11/08

#pragma once

#include "../Maths/3dGeometry.h"


class Frustum
{
public:
	Frustum();
	~Frustum();

	void SetFrustum(float angle, float ratio, float nearD, float farD);
	void SetCamera(const Vector3d &pos, const Vector3d &target, const Vector3d &up);

	int PointInFrustum(const Vector3d &point);
	int SphereInFrustum(const Vector3d &point, float radius);
	int CubeInFrustum(const Vector3d &center, float x, float y, float z);

public:
	enum {
		FRUSTUM_TOP = 0,
		FRUSTUM_BOTTOM,
		FRUSTUM_LEFT,
		FRUSTUM_RIGHT,
		FRUSTUM_NEAR,
		FRUSTUM_FAR,
	};

	enum {
		FRUSTUM_OUTSIDE = 0,
		FRUSTUM_INTERSECT,
		FRUSTUM_INSIDE,
	};

	Plane3D planes[6];

	Vector3d nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	Vector3d farTopLeft, farTopRight, farBottomLeft, farBottomRight;

	float nearDistance, farDistance;
	float nearWidth, nearHeight;
	float farWidth, farHeight;
	float ratio, angle, tang;
};