// Juice Engine by Steven Ball
// Viewport - A viewport, used by the opengl renderer to hold viewport information
// Created : 02/04/06
// Edited  : 19/05/06

#pragma once

class Viewport {
public:
	int Bottom, Left, Width, Height;
	float Fov, Aspect;
	Matrix4x4 Perspective, Orthographic, Projection2d;
};