// ******************************************************************************
//
// Filename:	Renderer.h
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/10/15
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#pragma once

#include "../Maths/3dmaths.h"
#include "../freetype/freetypefont.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment (lib, "opengl32")
#pragma comment (lib, "glu32")

#include <vector>
using namespace std;

#include "viewport.h"
#include "frustum.h"
#include "colour.h"


enum ProjectionMode {
	PM_PERSPECTIVE = 0,
	PM_ORTHOGRAPHIC,
	PM_2D
};

class Renderer
{
public:
	/* Public methods */
	Renderer(int width, int height, int depthBits, int stencilBits);
	~Renderer();

	bool CreateViewport(int bottom, int left, int width, int height, float fov, unsigned int *pID);

	bool SetProjectionMode(ProjectionMode mode, int viewPort);
	void SetViewProjection();
	void MultViewProjection();

	bool ClearScene(bool pixel = true, bool depth = true, bool stencil = true);
	bool BeginScene(bool pixel = true, bool depth = true, bool stencil = true);
	void EndScene();

	void PushMatrix();
	void PopMatrix();

	void SetWorldMatrix(const Matrix4x4 &mat);
	void IdentityWorldMatrix();

	void SetLookAtCamera(Vector3d pos, Vector3d target, Vector3d up);

	bool CreateFreeTypeFont(char *fontName, int fontSize, unsigned int *pID);
	bool RenderFreeTypeText(unsigned int fontID, float x, float y, float z, Colour colour, float scale, char *inText, ...);
	int GetFreeTypeTextWidth(unsigned int fontID, char *inText, ...);
	int GetFreeTypeTextHeight(unsigned int fontID, char *inText, ...);
	int GetFreeTypeTextAscent(unsigned int fontID);
	int GetFreeTypeTextDescent(unsigned int fontID);

protected:
	/* Protected methods */

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	int m_windowWidth;
	int m_windowHeight;

	// Stencil and depth bits
	bool m_stencil;
	bool m_depth;

	// Clipping planes
	float m_clipNear;
	float m_clipFar;

	// Viewports
	vector<Viewport *> m_viewports;

	// Frustums
	vector<Frustum *> m_frustums; // Note : We store a frustum for each viewport, therefore viewport and frustum are closely linked (See viewport functions)

	// Fonts
	vector<FreeTypeFont *> m_freetypeFonts;

	// Matrices
	Matrix4x4 *m_projection;
	Matrix4x4  m_view;
	Matrix4x4  m_model;

	// Model stack
	vector<Matrix4x4> m_modelStack;
};
