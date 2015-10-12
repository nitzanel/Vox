// ******************************************************************************
//
// Filename:	Renderer.cpp
// Project:		Vox
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

#include "Renderer.h"


Renderer::Renderer(int width, int height, int depthBits, int stencilBits)
{
	m_windowWidth = width;
	m_windowHeight = height;

	m_stencil = false;
	m_depth = false;

	// Default clipping planes
	m_clipNear = 0.1f;
	m_clipFar = 1000.0f;

	// Is depth buffer needed?
	if (depthBits > 0)
	{
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);

		m_depth = true;
	}

	// Is stencil buffer needed?
	if (stencilBits > 0)
	{
		glClearStencil(0);

		m_stencil = true;
	}

	// Initialize defaults
}

Renderer::~Renderer()
{
}


bool Renderer::CreateViewport(int bottom, int left, int width, int height, float fov, unsigned int *pID)
{
	Viewport* pViewport = new Viewport();
	Frustum* pFrustum = new Frustum();

	pViewport->Bottom = bottom;
	pViewport->Left = left;
	pViewport->Width = width;
	pViewport->Height = height;
	pViewport->Fov = fov;
	pViewport->Aspect = (float)width / (float)height;

	// Create the perspective projection for the viewport
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(fov, pViewport->Aspect, m_clipNear, m_clipFar);
	float mat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, mat);
	pViewport->Perspective = mat;
	glPopMatrix();

	// Setup the frustum for this viewport
	pFrustum->SetFrustum(fov, pViewport->Aspect, m_clipNear, m_clipFar);

	// Push this frustum onto the list
	m_frustums.push_back(pFrustum);

	// Create the orthographic projection matrix for the viewport
	float coordright = 1.0f;
	float coordleft = -1.0f;
	float coordtop = 1.0f;
	float coordbottom = -1.0f;

	memset(&(pViewport->Orthographic), 0, sizeof(Matrix4x4));
	pViewport->Orthographic.m[0] = 2.0f / (coordright - coordleft);
	pViewport->Orthographic.m[5] = 2.0f / (coordtop - coordbottom);
	pViewport->Orthographic.m[10] = -2.0f / (m_clipFar - m_clipNear);
	pViewport->Orthographic.m[12] = -(coordright + coordleft) / (coordright - coordleft);
	pViewport->Orthographic.m[13] = -(coordtop + coordbottom) / (coordtop - coordbottom);
	pViewport->Orthographic.m[14] = -(m_clipFar + m_clipNear) / (m_clipFar - m_clipNear);
	pViewport->Orthographic.m[15] = 1.0f;

	// Create the 2d projection matrix for the viewport
	coordright = (float)m_windowWidth;
	coordleft = 0.0f;
	coordtop = (float)m_windowHeight;
	coordbottom = 0.0f;

	memset(&(pViewport->Projection2d), 0, sizeof(Matrix4x4));
	pViewport->Projection2d.m[0] = 2.0f / (coordright - coordleft);
	pViewport->Projection2d.m[5] = 2.0f / (coordtop - coordbottom);
	pViewport->Projection2d.m[10] = -2.0f / (m_clipFar - m_clipNear);
	pViewport->Projection2d.m[12] = -(coordright + coordleft) / (coordright - coordleft);
	pViewport->Projection2d.m[13] = -(coordtop + coordbottom) / (coordtop - coordbottom);
	pViewport->Projection2d.m[14] = -(m_clipFar + m_clipNear) / (m_clipFar - m_clipNear);
	pViewport->Projection2d.m[15] = 1.0f;

	// Push this viewport onto the list
	m_viewports.push_back(pViewport);

	// Return the viewport id
	*pID = (int)m_viewports.size() - 1;

	return true;
}

bool Renderer::SetProjectionMode(ProjectionMode mode, int viewPort)
{
	Viewport* pVeiwport = m_viewports[viewPort];
	glViewport(pVeiwport->Left, pVeiwport->Bottom, pVeiwport->Width, pVeiwport->Height);

	if (mode == PM_PERSPECTIVE) {
		m_projection = &(pVeiwport->Perspective);
	}
	else if (mode == PM_ORTHOGRAPHIC) {
		m_projection = &(pVeiwport->Orthographic);
	}
	else if (mode == PM_2D) {
		m_projection = &(pVeiwport->Projection2d);
	}
	else {
		return false;
	}

	SetViewProjection();

	return true;
}

void Renderer::SetViewProjection()
{
	glMatrixMode(GL_PROJECTION);
	MultViewProjection();
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::MultViewProjection()
{
	SetWorldMatrix(m_view * (*m_projection));
}

bool Renderer::ClearScene(bool pixel, bool depth, bool stencil)
{
	GLbitfield clear(0);

	if (pixel)
		clear |= GL_COLOR_BUFFER_BIT;
	if (depth && m_depth)
		clear |= GL_DEPTH_BUFFER_BIT;
	if (stencil && m_stencil)
		clear |= GL_STENCIL_BUFFER_BIT;

	glClear(clear);

	return true;
}

bool Renderer::BeginScene(bool pixel, bool depth, bool stencil)
{
	ClearScene(pixel, depth, stencil);

	// Reset the projection and modelview matrices to be identity
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	IdentityWorldMatrix();

	// Start off with lighting and texturing disabled. If these are required, they need to be set explicitly
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	return true;
}

void Renderer::EndScene()
{
	// Swap buffers
}

void Renderer::PushMatrix()
{
	glPushMatrix();

	m_modelStack.push_back(m_model);
}

void Renderer::PopMatrix()
{
	glPopMatrix();

	m_model = m_modelStack.back();
	m_modelStack.pop_back();
}


void Renderer::SetWorldMatrix(const Matrix4x4& mat)
{
	float m[16];
	mat.GetMatrix(m);
	glLoadMatrixf(m);
}

void Renderer::IdentityWorldMatrix()
{
	glLoadIdentity();

	m_model.LoadIdentity();
}

void Renderer::SetLookAtCamera(Vector3d pos, Vector3d target, Vector3d up)
{
	gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
}

bool Renderer::CreateFreeTypeFont(char *fontName, int fontSize, unsigned int *pID)
{
	FreeTypeFont* font = new FreeTypeFont();

	// Build the new freetype font
	font->BuildFont(fontName, fontSize);

	// Push this font onto the list of fonts and return the id
	m_freetypeFonts.push_back(font);
	*pID = (unsigned int)m_freetypeFonts.size() - 1;

	return true;
}

bool Renderer::RenderFreeTypeText(unsigned int fontID, float x, float y, float z, Colour colour, float scale, char *inText, ...)
{
	char		outText[8192];
	va_list		ap;  // Pointer to list of arguments

	if (inText == NULL)
		return false;  // Return fail if there is no text

	// Loop through variable argument list and add them to the string
	va_start(ap, inText);
		vsprintf_s(outText, inText, ap);
	va_end(ap);

	glColor4fv(colour.GetRGBA());

	// Add on the descent value, so we don't draw letters with underhang out of bounds. (e.g - g, y, q and p)
	y -= GetFreeTypeTextDescent(fontID);

	// HACK : The descent has rounding errors and is usually off by about 1 pixel
	y -= 1;

	glPushMatrix();
		glTranslatef(x, y, 0);
		m_freetypeFonts[fontID]->DrawString(outText, scale);
	glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}

int Renderer::GetFreeTypeTextWidth(unsigned int fontID, char *inText, ...)
{
	char outText[8192];
	va_list ap;

	if (inText == NULL)
		return 0;

	// Loop through variable argument list and add them to the string
	va_start(ap, inText);
	vsprintf_s(outText, inText, ap);
	va_end(ap);

	return m_freetypeFonts[fontID]->GetTextWidth(outText);
}

int Renderer::GetFreeTypeTextHeight(unsigned int fontID, char *inText, ...)
{
	return m_freetypeFonts[fontID]->GetCharHeight('a');
}

int Renderer::GetFreeTypeTextAscent(unsigned int fontID)
{
	return m_freetypeFonts[fontID]->GetAscent();
}

int Renderer::GetFreeTypeTextDescent(unsigned int fontID)
{
	return m_freetypeFonts[fontID]->GetDescent();
}