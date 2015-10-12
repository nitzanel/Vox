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

#include "../glew/include/GL/glew.h"

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
	m_cullMode = CM_NOCULL;
	m_primativeMode = PM_TRIANGLES;
	m_activeViewport = -1;
}

Renderer::~Renderer()
{
	unsigned int i;

	// Delete the vertex arrays
	for (i = 0; i < m_vertexArrays.size(); i++)
	{
		delete m_vertexArrays[i];
		m_vertexArrays[i] = 0;
	}

	// Delete the viewports
	for (i = 0; i < m_viewports.size(); i++)
	{
		delete m_viewports[i];
		m_viewports[i] = 0;
	}

	// Delete the frustums
	for (i = 0; i < m_frustums.size(); i++)
	{
		delete m_frustums[i];
		m_frustums[i] = 0;
	}

	// Delete the materials
	for (i = 0; i < m_materials.size(); i++)
	{
		delete m_materials[i];
		m_materials[i] = 0;
	}

	// Delete the textures
	for (i = 0; i < m_textures.size(); i++)
	{
		delete m_textures[i];
		m_textures[i] = 0;
	}

	// Delete the lights
	for (i = 0; i < m_lights.size(); i++)
	{
		delete m_lights[i];
		m_lights[i] = 0;
	}

	// Delete the FreeType fonts
	for (i = 0; i < m_freetypeFonts.size(); i++)
	{
		delete m_freetypeFonts[i];
		m_freetypeFonts[i] = 0;
	}
}

void Renderer::ResizeWindow(int newWidth, int newHeight)
{
	m_windowWidth = newWidth;
	m_windowHeight = newHeight;
}
// Viewport
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


bool Renderer::ResizeViewport(unsigned int viewportid, int bottom, int left, int width, int height, float fov)
{
	Viewport* pViewport = m_viewports[viewportid];
	Frustum* pFrustum = m_frustums[viewportid];

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
	gluPerspective(pViewport->Fov, pViewport->Aspect, m_clipNear, m_clipFar);
	float mat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, mat);
	pViewport->Perspective = mat;
	glPopMatrix();

	// Resize the frustum
	pFrustum->SetFrustum(pViewport->Fov, pViewport->Aspect, m_clipNear, m_clipFar);

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

	return true;
}

int Renderer::GetActiveViewPort()
{
	return m_activeViewport;
}

// Render modes
void Renderer::SetRenderMode(RenderMode mode)
{
	switch (mode)
	{
	case RM_WIREFRAME:
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	case RM_SOLID:
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	case RM_SHADED:
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	case RM_TEXTURED:
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	case RM_TEXTURED_LIGHTING:
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	};
}

void Renderer::SetPrimativeMode(PrimativeMode mode)
{
	switch (mode)
	{
	case PM_POINTS:
		m_primativeMode = GL_POINTS;
		break;
	case PM_LINES:
		m_primativeMode = GL_LINES;
		break;
	case PM_LINELIST:
		m_primativeMode = GL_LINE_STRIP;
		break;
	case PM_TRIANGLES:
		m_primativeMode = GL_TRIANGLES;
		break;
	case PM_TRIANGLESTRIPS:
		m_primativeMode = GL_TRIANGLE_STRIP;
		break;
	case PM_TRIANGLEFANS:
		m_primativeMode = GL_TRIANGLE_FAN;
		break;
	case PM_QUADS:
		m_primativeMode = GL_QUADS;
		break;
	}
}

void Renderer::SetCullMode(CullMode mode)
{
	m_cullMode = mode;

	switch (mode)
	{
	case CM_NOCULL:
		glDisable(GL_CULL_FACE);
		break;
	case CM_FRONT:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case CM_BACK:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	}
}

CullMode Renderer::GetCullMode()
{
	return m_cullMode;
}

void Renderer::SetLineWidth(float width)
{
	glLineWidth(width);
}

void Renderer::SetPointSize(float width)
{
	glPointSize(width);
}

// Projection
bool Renderer::SetProjectionMode(ProjectionMode mode, int viewPort)
{
	Viewport* pVeiwport = m_viewports[viewPort];
	glViewport(pVeiwport->Left, pVeiwport->Bottom, pVeiwport->Width, pVeiwport->Height);

	m_activeViewport = viewPort;

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

// Scene
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

// Push / Pop matrix stack
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

// Matrix manipulations
void Renderer::SetWorldMatrix(const Matrix4x4& mat)
{
	float m[16];
	mat.GetMatrix(m);
	glLoadMatrixf(m);
}

void Renderer::GetModelViewMatrix(Matrix4x4 *pMat)
{
	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	memcpy(pMat->m, mat, 16 * sizeof(float));
}

void Renderer::GetModelMatrix(Matrix4x4 *pMat)
{
	memcpy(pMat->m, m_model.m, 16 * sizeof(float));
}

void Renderer::GetViewMatrix(Matrix4x4 *pMat)
{
	memcpy(pMat->m, m_view.m, 16 * sizeof(float));
}

void Renderer::GetProjectionMatrix(Matrix4x4 *pMat)
{
	float mat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, mat);
	memcpy(pMat->m, mat, 16 * sizeof(float));
	//memcpy(pMat->m, m_projection->m, 16 * sizeof(float));
}

void Renderer::IdentityWorldMatrix()
{
	glLoadIdentity();

	m_model.LoadIdentity();
}

void Renderer::MultiplyWorldMatrix(const Matrix4x4 &mat)
{
	float m[16];
	mat.GetMatrix(m);
	glMultMatrixf(m);

	Matrix4x4 world(mat);
	m_model = world * m_model;
}

void Renderer::TranslateWorldMatrix(float x, float y, float z)
{
	glTranslatef(x, y, z);

	Matrix4x4 translate;
	translate.SetTranslation(Vector3d(x, y, z));
	m_model = translate * m_model;
}

void Renderer::RotateWorldMatrix(float x, float y, float z)
{
	// Posible gimbal lock?
	glRotatef(z, 0.0f, 0.0f, 1.0f);
	glRotatef(y, 0.0f, 1.0f, 0.0f);
	glRotatef(x, 1.0f, 0.0f, 0.0f);

	Matrix4x4 rotX;
	Matrix4x4 rotY;
	Matrix4x4 rotZ;
	rotX.SetXRotation(DegToRad(x));
	rotY.SetYRotation(DegToRad(y));
	rotZ.SetZRotation(DegToRad(z));

	m_model = rotZ * rotY * rotX * m_model;
}

void Renderer::ScaleWorldMatrix(float x, float y, float z)
{
	glScalef(x, y, z);

	Matrix4x4 scale;
	scale.SetScale(Vector3d(x, y, z));
	m_model = scale * m_model;
}

// Texture matrix manipulations
void Renderer::SetTextureMatrix()
{
	static double modelView[16];
	static double projection[16];

	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5 
	// y = y* 0.5 + 0.5 
	// z = z* 0.5 + 0.5 
	// Moving from unit cube [-1,1] to [0,1]  
	const GLdouble bias[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0 };

	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);


	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);

	glLoadIdentity();
	glLoadMatrixd(bias);

	// concatating all matrice into one.
	glMultMatrixd(projection);
	glMultMatrixd(modelView);

	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::PushTextureMatrix()
{
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
}

void Renderer::PopTextureMatrix()
{
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

// Camera functionality
void Renderer::SetLookAtCamera(Vector3d pos, Vector3d target, Vector3d up)
{
	gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
}

// Transparency
void Renderer::EnableTransparency(BlendFunction source, BlendFunction destination)
{
	glDisable(GL_DEPTH_WRITEMASK);
	glEnable(GL_BLEND);
	glBlendFunc(GetBlendEnum(source), GetBlendEnum(destination));
}

void Renderer::DisableTransparency()
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_WRITEMASK);
}

GLenum Renderer::GetBlendEnum(BlendFunction flag)
{
	GLenum glFlag;
	switch (flag)
	{
	case BF_ONE:
		glFlag = GL_ONE;
		break;
	case BF_ZERO:
		glFlag = GL_ZERO;
		break;
	case BF_SRC_ALPHA:
		glFlag = GL_SRC_ALPHA;
		break;
	case BF_ONE_MINUS_SRC_ALPHA:
		glFlag = GL_ONE_MINUS_SRC_ALPHA;
		break;
	}

	return glFlag;
}

// Depth testing
void Renderer::EnableDepthTest(DepthTest lTestFunction)
{
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GetDepthTest(lTestFunction));
}

void Renderer::DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

GLenum Renderer::GetDepthTest(DepthTest lTest)
{
	GLenum glFlag;
	switch (lTest)
	{
	case DT_NEVER:
		glFlag = GL_NEVER;
		break;
	case DT_ALWAYS:
		glFlag = GL_ALWAYS;
		break;
	case DT_LESS:
		glFlag = GL_LESS;
		break;
	case DT_LEQUAL:
		glFlag = GL_LEQUAL;
		break;
	case DT_EQUAL:
		glFlag = GL_EQUAL;
		break;
	case DT_GEQUAL:
		glFlag = GL_GEQUAL;
		break;
	case DT_GREATER:
		glFlag = GL_GREATER;
		break;
	case DT_NOTEQUAL:
		glFlag = GL_NOTEQUAL;
		break;
	}

	return glFlag;
}

void Renderer::EnableDepthWrite()
{
	glDepthMask(GL_TRUE);
}

void Renderer::DisableDepthWrite()
{
	glDepthMask(GL_FALSE);
}

// Immediate mode
void Renderer::EnableImmediateMode(ImmediateModePrimitive mode)
{
	GLenum glMode;
	switch (mode)
	{
	case IM_POINTS:
		glMode = GL_POINTS;
		break;
	case IM_LINES:
		glMode = GL_LINES;
		break;
	case IM_LINE_LOOP:
		glMode = GL_LINE_LOOP;
		break;
	case IM_LINE_STRIP:
		glMode = GL_LINE_STRIP;
		break;
	case IM_TRIANGLES:
		glMode = GL_TRIANGLES;
		break;
	case IM_TRIANGLE_STRIP:
		glMode = GL_TRIANGLE_STRIP;
		break;
	case IM_TRIANGLE_FAN:
		glMode = GL_TRIANGLE_FAN;
		break;
	case IM_QUADS:
		glMode = GL_QUADS;
		break;
	case IM_QUAD_STRIP:
		glMode = GL_QUAD_STRIP;
		break;
	case IM_POLYGON:
		glMode = GL_POLYGON;
		break;
	}

	glBegin(glMode);
}

void Renderer::ImmediateVertex(float x, float y, float z)
{
	glVertex3f(x, y, z);
}

void Renderer::ImmediateVertex(int x, int y, int z)
{
	glVertex3i(x, y, z);
}

void Renderer::ImmediateNormal(float x, float y, float z)
{
	glNormal3f(x, y, z);
}

void Renderer::ImmediateNormal(int x, int y, int z)
{
	glNormal3i(x, y, z);
}

void Renderer::ImmediateTextureCoordinate(float s, float t)
{
	glTexCoord2f(s, t);
}

void Renderer::ImmediateColourAlpha(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
}

void Renderer::DisableImmediateMode()
{
	glEnd();
}

// Text rendering
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

// Lighting
bool Renderer::CreateLight(const Colour &ambient, const Colour &diffuse, const Colour &specular, Vector3d &position, Vector3d &direction, float exponent, float cutoff, float cAtten, float lAtten, float qAtten, bool point, bool spot, unsigned int *pID)
{
	Light *pLight = new Light();

	pLight->Ambient(ambient);
	pLight->Diffuse(diffuse);
	pLight->Specular(specular);
	pLight->Position(position);
	pLight->Direction(direction);
	pLight->Exponent(exponent);
	pLight->Cutoff(cutoff);
	pLight->ConstantAttenuation(cAtten);
	pLight->LinearAttenuation(lAtten);
	pLight->QuadraticAttenuation(qAtten);
	pLight->Point(point);
	pLight->Spotlight(spot);

	// Push the light onto the list
	m_lights.push_back(pLight);

	// Return the light id
	*pID = (int)m_lights.size() - 1;

	return true;
}

bool Renderer::EditLight(unsigned int id, const Colour &ambient, const Colour &diffuse, const Colour &specular, Vector3d &position, Vector3d &direction, float exponent, float cutoff, float cAtten, float lAtten, float qAtten, bool point, bool spot)
{
	Light *pLight = m_lights[id];

	pLight->Ambient(ambient);
	pLight->Diffuse(diffuse);
	pLight->Specular(specular);
	pLight->Position(position);
	pLight->Direction(direction);
	pLight->Exponent(exponent);
	pLight->Cutoff(cutoff);
	pLight->ConstantAttenuation(cAtten);
	pLight->LinearAttenuation(lAtten);
	pLight->QuadraticAttenuation(qAtten);
	pLight->Point(point);
	pLight->Spotlight(spot);

	return true;
}

bool Renderer::EditLightPosition(unsigned int id, Vector3d &position)
{
	Light *pLight = m_lights[id];

	pLight->Position(position);

	return true;
}

void Renderer::DeleteLight(unsigned int id)
{
	if (m_lights[id]) {
		delete m_lights[id];
		m_lights[id] = 0;
	}
}

void Renderer::EnableLight(unsigned int id, unsigned int lightNumber)
{
	if (m_lights[id])
	{
		m_lights[id]->Apply(lightNumber);
	}
}

void Renderer::DisableLight(unsigned int lightNumber)
{
	glDisable(GL_LIGHT0 + lightNumber);
}

void Renderer::RenderLight(unsigned int id)
{
	m_lights[id]->Render();
}

Colour Renderer::GetLightAmbient(unsigned int id)
{
	return m_lights[id]->Ambient();
}

Colour Renderer::GetLightDiffuse(unsigned int id)
{
	return m_lights[id]->Diffuse();
}

Colour Renderer::GetLightSpecular(unsigned int id)
{
	return m_lights[id]->Specular();
}

Vector3d Renderer::GetLightPosition(unsigned int id)
{
	return m_lights[id]->Position();
}

float Renderer::GetConstantAttenuation(unsigned int id)
{
	return m_lights[id]->ConstantAttenuation();
}

float Renderer::GetLinearAttenuation(unsigned int id)
{
	return m_lights[id]->LinearAttenuation();
}

float Renderer::GetQuadraticAttenuation(unsigned int id)
{
	return m_lights[id]->QuadraticAttenuation();
}

// Materials
bool Renderer::CreateMaterial(const Colour &ambient, const Colour &diffuse, const Colour &specular, const Colour &emmisive, float specularPower, unsigned int *pID)
{
	Material *pMaterial = new Material();

	pMaterial->Ambient(ambient);
	pMaterial->Diffuse(diffuse);
	pMaterial->Specular(specular);
	pMaterial->Emission(emmisive);
	pMaterial->Shininess(specularPower);

	// Push the material onto the list
	m_materials.push_back(pMaterial);

	// Return the material id
	*pID = (int)m_materials.size() - 1;

	return true;
}

bool Renderer::EditMaterial(unsigned int id, const Colour &ambient, const Colour &diffuse, const Colour &specular, const Colour &emmisive, float specularPower)
{
	Material *pMaterial = m_materials[id];

	pMaterial->Ambient(ambient);
	pMaterial->Diffuse(diffuse);
	pMaterial->Specular(specular);
	pMaterial->Emission(emmisive);
	pMaterial->Shininess(specularPower);

	return true;
}

void Renderer::EnableMaterial(unsigned int id)
{
	m_materials[id]->Apply();
}

void Renderer::DeleteMaterial(unsigned int id)
{
	if (m_materials[id])
	{
		delete m_materials[id];
		m_materials[id] = 0;
	}
}

// Textures
bool Renderer::LoadTexture(string fileName, int *width, int *height, int *width_power2, int *height_power2, unsigned int *pID)
{
	// Check that this texture hasn't already been loaded
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->GetFileName() == fileName)
		{
			*width = m_textures[i]->GetWidth();
			*height = m_textures[i]->GetHeight();
			*width_power2 = m_textures[i]->GetWidthPower2();
			*height_power2 = m_textures[i]->GetHeightPower2();
			*pID = i;

			return true;
		}
	}

	// Texture hasn't already been loaded, create and load it!
	Texture *pTexture = new Texture();
	pTexture->Load(fileName, width, height, width_power2, height_power2, false);

	// Push the vertex array onto the list
	m_textures.push_back(pTexture);

	// Return the vertex array id
	*pID = (int)m_textures.size() - 1;

	return true;
}

bool Renderer::RefreshTexture(unsigned int id)
{
	Texture *pTexture = m_textures[id];

	int width;
	int height;
	int width_power2;
	int height_power2;
	pTexture->Load(pTexture->GetFileName(), &width, &height, &width_power2, &height_power2, true);

	return true;
}

bool Renderer::RefreshTexture(string filename)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		if (m_textures[i]->GetFileName() == filename)
		{
			return RefreshTexture(i);
		}
	}

	return false;
}

void Renderer::BindTexture(unsigned int id)
{
	glEnable(GL_TEXTURE_2D);
	m_textures[id]->Bind();
}

void Renderer::DisableTexture()
{
	glDisable(GL_TEXTURE_2D);
}

Texture* Renderer::GetTexture(unsigned int id)
{
	return m_textures[id];
}

void Renderer::BindRawTextureId(unsigned int textureId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Renderer::GenerateEmptyTexture(unsigned int *pID)
{
	Texture *pTexture = new Texture();
	pTexture->GenerateEmptyTexture();

	// Push the vertex array onto the list
	m_textures.push_back(pTexture);

	// Return the vertex array id
	*pID = (int)m_textures.size() - 1;
}

void Renderer::SetTextureData(unsigned int id, int width, int height, unsigned char *texdata)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_textures[id]->GetId());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texdata);
	glDisable(GL_TEXTURE_2D);
}

// Vertex buffers
bool Renderer::CreateStaticBuffer(VertexType type, unsigned int materialID, unsigned int textureID, int nVerts, int nTextureCoordinates, int nIndices, const void *pVerts, const void *pTextureCoordinates, const unsigned int *pIndices, unsigned int *pID)
{
	VertexArray *pVertexArray = new VertexArray();

	pVertexArray->nIndices = nIndices;
	pVertexArray->nVerts = nVerts;
	pVertexArray->nTextureCoordinates = nTextureCoordinates;
	pVertexArray->materialID = materialID;
	pVertexArray->textureID = textureID;
	pVertexArray->type = type;

	// Get the correct vertex size and construct the vertex array to hold the vertices
	if (nVerts)
	{
		switch (type)
		{
		case VT_POSITION:
			pVertexArray->vertexSize = sizeof(OGLPositionVertex);
			pVertexArray->pVA = new float[nVerts * 3];
			break;
		case VT_POSITION_DIFFUSE:
			pVertexArray->vertexSize = sizeof(OGLPositionDiffuseVertex);
			pVertexArray->pVA = new float[nVerts * 6];
			break;
		case VT_POSITION_DIFFUSE_ALPHA:
			pVertexArray->vertexSize = sizeof(OGLPositionDiffuseAlphaVertex);
			pVertexArray->pVA = new float[nVerts * 7];
			break;
		case VT_POSITION_NORMAL:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalVertex);
			pVertexArray->pVA = new float[nVerts * 6];
			break;
		case VT_POSITION_NORMAL_COLOUR:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalColourVertex);
			pVertexArray->pVA = new float[nVerts * 10];
			break;
		case VT_POSITION_NORMAL_UV:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalVertex);
			pVertexArray->pVA = new float[nVerts * 6];
			pVertexArray->textureCoordinateSize = sizeof(OGLUVCoordinate);
			pVertexArray->pTextureCoordinates = new float[nTextureCoordinates * 2];
			break;
		case VT_POSITION_NORMAL_UV_COLOUR:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalColourVertex);
			pVertexArray->pVA = new float[nVerts * 10];
			pVertexArray->textureCoordinateSize = sizeof(OGLUVCoordinate);
			pVertexArray->pTextureCoordinates = new float[nTextureCoordinates * 2];
			break;
		}
	}


	// If we have indices, create the indices array to hold the information
	if (nIndices)
	{
		pVertexArray->pIndices = new unsigned int[nIndices];
	}

	// Copy the vertices into the vertex array
	memcpy(pVertexArray->pVA, pVerts, pVertexArray->vertexSize*nVerts);

	// Copt the texture coordinates into the texture array
	memcpy(pVertexArray->pTextureCoordinates, pTextureCoordinates, pVertexArray->textureCoordinateSize*nTextureCoordinates);

	// Copy the indices into the vertex array
	memcpy(pVertexArray->pIndices, pIndices, sizeof(unsigned int)*nIndices);

	// Push the vertex array onto the list
	m_vertexArrays.push_back(pVertexArray);

	// Return the vertex array id
	*pID = (int)m_vertexArrays.size() - 1;

	return true;
}

bool Renderer::RecreateStaticBuffer(unsigned int ID, VertexType type, unsigned int materialID, unsigned int textureID, int nVerts, int nTextureCoordinates, int nIndices, const void *pVerts, const void *pTextureCoordinates, const unsigned int *pIndices)
{
	// Create a new vertex array
	m_vertexArrays[ID] = new VertexArray();

	// Get this already existing array pointer from the list
	VertexArray *pVertexArray = m_vertexArrays[ID];

	pVertexArray->nIndices = nIndices;
	pVertexArray->nVerts = nVerts;
	pVertexArray->nTextureCoordinates = nTextureCoordinates;
	pVertexArray->materialID = materialID;
	pVertexArray->textureID = textureID;
	pVertexArray->type = type;

	// Get the correct vertex size and construct the vertex array to hold the vertices
	if (nVerts)
	{
		switch (type)
		{
		case VT_POSITION:
			pVertexArray->vertexSize = sizeof(OGLPositionVertex);
			pVertexArray->pVA = new float[nVerts * 3];
			break;
		case VT_POSITION_DIFFUSE:
			pVertexArray->vertexSize = sizeof(OGLPositionDiffuseVertex);
			pVertexArray->pVA = new float[nVerts * 6];
			break;
		case VT_POSITION_DIFFUSE_ALPHA:
			pVertexArray->vertexSize = sizeof(OGLPositionDiffuseAlphaVertex);
			pVertexArray->pVA = new float[nVerts * 7];
			break;
		case VT_POSITION_NORMAL:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalVertex);
			pVertexArray->pVA = new float[nVerts * 6];
			break;
		case VT_POSITION_NORMAL_COLOUR:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalColourVertex);
			pVertexArray->pVA = new float[nVerts * 10];
			break;
		case VT_POSITION_NORMAL_UV:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalVertex);
			pVertexArray->pVA = new float[nVerts * 6];
			pVertexArray->textureCoordinateSize = sizeof(OGLUVCoordinate);
			pVertexArray->pTextureCoordinates = new float[nTextureCoordinates * 2];
			break;
		case VT_POSITION_NORMAL_UV_COLOUR:
			pVertexArray->vertexSize = sizeof(OGLPositionNormalColourVertex);
			pVertexArray->pVA = new float[nVerts * 10];
			pVertexArray->textureCoordinateSize = sizeof(OGLUVCoordinate);
			pVertexArray->pTextureCoordinates = new float[nTextureCoordinates * 2];
			break;
		}
	}


	// If we have indices, create the indices array to hold the information
	if (nIndices)
	{
		pVertexArray->pIndices = new unsigned int[nIndices];
	}

	// Copy the vertices into the vertex array
	memcpy(pVertexArray->pVA, pVerts, pVertexArray->vertexSize*nVerts);

	// Copt the texture coordinates into the texture array
	memcpy(pVertexArray->pTextureCoordinates, pTextureCoordinates, pVertexArray->textureCoordinateSize*nTextureCoordinates);

	// Copy the indices into the vertex array
	memcpy(pVertexArray->pIndices, pIndices, sizeof(unsigned int)*nIndices);

	return true;
}

void Renderer::DeleteStaticBuffer(unsigned int id)
{
	if (m_vertexArrays[id])
	{
		delete m_vertexArrays[id];
		m_vertexArrays[id] = 0;
	}
}

bool Renderer::RenderStaticBuffer(unsigned int id)
{
	if (id >= m_vertexArrays.size())
	{
		return false;  // We have supplied an invalid id
	}

	// Find the vertex array from the list
	VertexArray *pVertexArray = m_vertexArrays[id];

	if (pVertexArray != NULL)
	{
		if ((pVertexArray->type != VT_POSITION_DIFFUSE_ALPHA) && (pVertexArray->type != VT_POSITION_DIFFUSE))
		{
			if (pVertexArray->materialID != -1)
			{
				m_materials[pVertexArray->materialID]->Apply();
			}
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR)
		{
			if (pVertexArray->textureID != -1)
			{
				BindTexture(pVertexArray->textureID);
			}
		}

		// Calculate the stride
		GLsizei totalStride = GetStride(pVertexArray->type);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, totalStride, pVertexArray->pVA);

		if (pVertexArray->type == VT_POSITION_NORMAL || pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR || pVertexArray->type == VT_POSITION_NORMAL_COLOUR)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, pVertexArray->pTextureCoordinates);
		}

		if (pVertexArray->type == VT_POSITION_DIFFUSE_ALPHA)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_DIFFUSE)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR || pVertexArray->type == VT_POSITION_NORMAL_COLOUR)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, totalStride, &pVertexArray->pVA[6]);
		}

		if (pVertexArray->nIndices != 0)
		{
			glDrawElements(m_primativeMode, pVertexArray->nIndices, GL_UNSIGNED_INT, pVertexArray->pIndices);
		}
		else
		{
			glDrawArrays(m_primativeMode, 0, pVertexArray->nVerts);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		return true;
	}

	return false;
}

bool Renderer::RenderStaticBuffer_NoColour(unsigned int id)
{
	if (id >= m_vertexArrays.size())
	{
		return false;  // We have supplied an invalid id
	}

	// Find the vertex array from the list
	VertexArray *pVertexArray = m_vertexArrays[id];

	if (pVertexArray != NULL)
	{
		if ((pVertexArray->type != VT_POSITION_DIFFUSE_ALPHA) && (pVertexArray->type != VT_POSITION_DIFFUSE))
		{
			if (pVertexArray->materialID != -1)
			{
				m_materials[pVertexArray->materialID]->Apply();
			}
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR)
		{
			if (pVertexArray->textureID != -1)
			{
				BindTexture(pVertexArray->textureID);
			}
		}

		// Calculate the stride
		GLsizei totalStride = GetStride(pVertexArray->type);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, totalStride, pVertexArray->pVA);

		if (pVertexArray->type == VT_POSITION_NORMAL || pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR || pVertexArray->type == VT_POSITION_NORMAL_COLOUR)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, pVertexArray->pTextureCoordinates);
		}

		if (pVertexArray->type == VT_POSITION_DIFFUSE_ALPHA)
		{
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(4, GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_DIFFUSE)
		{
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(3, GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR || pVertexArray->type == VT_POSITION_NORMAL_COLOUR)
		{
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(4, GL_FLOAT, totalStride, &pVertexArray->pVA[6]);
		}

		if (pVertexArray->nIndices != 0)
		{
			glDrawElements(m_primativeMode, pVertexArray->nIndices, GL_UNSIGNED_INT, pVertexArray->pIndices);
		}
		else
		{
			glDrawArrays(m_primativeMode, 0, pVertexArray->nVerts);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);

		return true;
	}

	return false;
}

bool Renderer::RenderFromArray(VertexType type, unsigned int materialID, unsigned int textureID, int nVerts, int nTextureCoordinates, int nIndices, const void *pVerts, const void *pTextureCoordinates, const unsigned int *pIndices)
{
	if ((type != VT_POSITION_DIFFUSE_ALPHA) && (type != VT_POSITION_DIFFUSE))
	{
		if (materialID != -1)
		{
			m_materials[materialID]->Apply();
		}
	}

	if (type == VT_POSITION_NORMAL_UV || type == VT_POSITION_NORMAL_UV_COLOUR)
	{
		if (textureID != -1)
		{
			BindTexture(textureID);
		}
	}

	// Calculate the stride
	GLsizei totalStride = GetStride(type);

	float* pVertsf = (float*)pVerts;
	float* pTexturesf = (float*)pTextureCoordinates;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, totalStride, pVerts);

	if (type == VT_POSITION_NORMAL || type == VT_POSITION_NORMAL_UV || type == VT_POSITION_NORMAL_UV_COLOUR || type == VT_POSITION_NORMAL_COLOUR)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, totalStride, &pVertsf[3]);
	}

	if (type == VT_POSITION_NORMAL_UV || type == VT_POSITION_NORMAL_UV_COLOUR)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &pTexturesf);
	}

	if (type == VT_POSITION_DIFFUSE_ALPHA)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, totalStride, &pVertsf[3]);
	}

	if (type == VT_POSITION_DIFFUSE)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, totalStride, &pVertsf[3]);
	}

	if (type == VT_POSITION_NORMAL_UV_COLOUR || type == VT_POSITION_NORMAL_COLOUR)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, totalStride, &pVertsf[6]);
	}

	if (nIndices != 0)
	{
		glDrawElements(m_primativeMode, nIndices, GL_UNSIGNED_INT, pIndices);
	}
	else
	{
		glDrawArrays(m_primativeMode, 0, nVerts);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	return true;
}

unsigned int Renderer::GetStride(VertexType type)
{
	// Add xyz stride
	unsigned int totalStride = sizeof(float) * 3;

	// Add normals stride
	if (type == VT_POSITION_NORMAL || type == VT_POSITION_NORMAL_UV || type == VT_POSITION_NORMAL_UV_COLOUR || type == VT_POSITION_NORMAL_COLOUR)
		totalStride += sizeof(float) * 3;

	// Add colour  stride
	if (type == VT_POSITION_DIFFUSE)
		totalStride += sizeof(float) * 3;

	// Add colour and alpha stride
	if (type == VT_POSITION_DIFFUSE_ALPHA || type == VT_POSITION_NORMAL_UV_COLOUR || type == VT_POSITION_NORMAL_COLOUR)
		totalStride += sizeof(float) * 4;

	return totalStride;
}

// Mesh
OpenGLTriangleMesh* Renderer::CreateMesh(OGLMeshType meshType)
{
	OpenGLTriangleMesh* pNewMesh = new OpenGLTriangleMesh();

	pNewMesh->m_meshType = meshType;

	// Return the mesh pointer
	return pNewMesh;
}

void Renderer::ClearMesh(OpenGLTriangleMesh* pMesh)
{
	pMesh->m_textureId = -1;
	pMesh->m_materialId = -1;
	//pMesh->m_staticMeshId = -1; // DON'T reset this! Else we end up create more and more and more static buffers and data

	// Delete the vertices
	for (unsigned int i = 0; i < pMesh->m_vertices.size(); i++)
	{
		delete pMesh->m_vertices[i];
		pMesh->m_vertices[i] = 0;
	}

	// Delete the texture coordinates
	for (unsigned int i = 0; i < pMesh->m_textureCoordinates.size(); i++)
	{
		delete pMesh->m_textureCoordinates[i];
		pMesh->m_textureCoordinates[i] = 0;
	}

	// Delete the triangles
	for (unsigned int i = 0; i < pMesh->m_triangles.size(); i++)
	{
		delete pMesh->m_triangles[i];
		pMesh->m_triangles[i] = 0;
	}

	pMesh->m_vertices.clear();
	pMesh->m_textureCoordinates.clear();
	pMesh->m_triangles.clear();

	if (pMesh->m_staticMeshId != -1)
	{
		DeleteStaticBuffer(pMesh->m_staticMeshId);
	}
	pMesh->m_staticMeshId = -1;

	delete pMesh;
	pMesh = NULL;
}

unsigned int Renderer::AddVertexToMesh(Vector3d p, Vector3d n, float r, float g, float b, float a, OpenGLTriangleMesh* pMesh)
{
	OpenGLMesh_Vertex* pNewVertex = new OpenGLMesh_Vertex();
	pNewVertex->vertexPosition[0] = p.x;
	pNewVertex->vertexPosition[1] = p.y;
	pNewVertex->vertexPosition[2] = p.z;

	pNewVertex->vertexNormals[0] = n.x;
	pNewVertex->vertexNormals[1] = n.y;
	pNewVertex->vertexNormals[2] = n.z;

	pNewVertex->vertexColour[0] = r;
	pNewVertex->vertexColour[1] = g;
	pNewVertex->vertexColour[2] = b;
	pNewVertex->vertexColour[3] = a;

	if (pMesh != NULL)
	{
		pMesh->m_vertices.push_back(pNewVertex);

		unsigned int vertex_id = (int)pMesh->m_vertices.size() - 1;

		return vertex_id;
	}
	else
	{
		return -1;
	}
}

unsigned int Renderer::AddTextureCoordinatesToMesh(float s, float t, OpenGLTriangleMesh* pMesh)
{
	OpenGLMesh_TextureCoordinate* pNewTextureCoordinate = new OpenGLMesh_TextureCoordinate();
	pNewTextureCoordinate->s = s;
	pNewTextureCoordinate->t = t;

	if (pMesh != NULL)
	{
		pMesh->m_textureCoordinates.push_back(pNewTextureCoordinate);

		unsigned int textureCoordinate_id = (int)pMesh->m_textureCoordinates.size() - 1;

		return textureCoordinate_id;
	}
	else
	{
		return -1;
	}
}

unsigned int Renderer::AddTriangleToMesh(unsigned int vertexId1, unsigned int vertexId2, unsigned int vertexId3, OpenGLTriangleMesh* pMesh)
{
	// Create the triangle
	OpenGLMesh_Triangle* pTri = new OpenGLMesh_Triangle();
	pTri->vertexIndices[0] = vertexId1;
	pTri->vertexIndices[1] = vertexId2;
	pTri->vertexIndices[2] = vertexId3;

	if (pMesh != NULL)
	{
		pMesh->m_triangles.push_back(pTri);

		unsigned int tri_id = (int)pMesh->m_triangles.size() - 1;

		return tri_id;
	}
	else
	{
		return -1;
	}
}

void Renderer::ModifyMeshAlpha(float alpha, OpenGLTriangleMesh* pMesh)
{
	VertexArray* pArray = m_vertexArrays[pMesh->m_staticMeshId];

	GLsizei totalStride = GetStride(pArray->type) / 4;
	int alphaIndex = totalStride - 1;

	for (int i = 0; i < pArray->nVerts; i++)
	{
		pArray->pVA[alphaIndex] = alpha;

		alphaIndex += totalStride;
	}
}

void Renderer::ModifyMeshColour(float r, float g, float b, OpenGLTriangleMesh* pMesh)
{
	VertexArray* pArray = m_vertexArrays[pMesh->m_staticMeshId];

	GLsizei totalStride = GetStride(pArray->type) / 4;
	int rIndex = totalStride - 4;
	int gIndex = totalStride - 3;
	int bIndex = totalStride - 2;

	for (int i = 0; i < pArray->nVerts; i++)
	{
		pArray->pVA[rIndex] = r;
		pArray->pVA[gIndex] = g;
		pArray->pVA[bIndex] = b;

		rIndex += totalStride;
		gIndex += totalStride;
		bIndex += totalStride;
	}
}

void Renderer::FinishMesh(unsigned int textureID, unsigned int materialID, OpenGLTriangleMesh* pMesh)
{
	unsigned int numTriangles = (int)pMesh->m_triangles.size();
	unsigned int numVertices = (int)pMesh->m_vertices.size();
	unsigned int numTextureCoordinates = (int)pMesh->m_textureCoordinates.size();
	unsigned int numIndices = (int)pMesh->m_triangles.size() * 3;

	pMesh->m_materialId = materialID;
	pMesh->m_textureId = textureID;

	// Vertices
	OGLPositionNormalColourVertex* meshBuffer;
	meshBuffer = new OGLPositionNormalColourVertex[numVertices];
	for (unsigned int i = 0; i < numVertices; i++)
	{
		meshBuffer[i].x = pMesh->m_vertices[i]->vertexPosition[0];
		meshBuffer[i].y = pMesh->m_vertices[i]->vertexPosition[1];
		meshBuffer[i].z = pMesh->m_vertices[i]->vertexPosition[2];

		meshBuffer[i].nx = pMesh->m_vertices[i]->vertexNormals[0];
		meshBuffer[i].ny = pMesh->m_vertices[i]->vertexNormals[1];
		meshBuffer[i].nz = pMesh->m_vertices[i]->vertexNormals[2];

		meshBuffer[i].r = pMesh->m_vertices[i]->vertexColour[0];
		meshBuffer[i].g = pMesh->m_vertices[i]->vertexColour[1];
		meshBuffer[i].b = pMesh->m_vertices[i]->vertexColour[2];
		meshBuffer[i].a = pMesh->m_vertices[i]->vertexColour[3];
	}

	// Texture coordinates
	OGLUVCoordinate* textureCoordinatesBuffer;
	textureCoordinatesBuffer = new OGLUVCoordinate[numTextureCoordinates];
	for (unsigned int i = 0; i < numTextureCoordinates; i++)
	{
		textureCoordinatesBuffer[i].u = pMesh->m_textureCoordinates[i]->s;
		textureCoordinatesBuffer[i].v = pMesh->m_textureCoordinates[i]->t;
	}

	// Indices
	unsigned int* indicesBuffer;
	indicesBuffer = new unsigned int[numIndices];
	int lIndexCounter = 0;
	for (unsigned int i = 0; i < numTriangles; i++)
	{
		indicesBuffer[lIndexCounter] = pMesh->m_triangles[i]->vertexIndices[0];
		indicesBuffer[lIndexCounter + 1] = pMesh->m_triangles[i]->vertexIndices[1];
		indicesBuffer[lIndexCounter + 2] = pMesh->m_triangles[i]->vertexIndices[2];

		lIndexCounter += 3;
	}

	if (pMesh->m_meshType == OGLMeshType_Colour)
	{
		if (pMesh->m_staticMeshId == -1)
		{
			CreateStaticBuffer(VT_POSITION_NORMAL_COLOUR, pMesh->m_materialId, -1, numVertices, 0, numIndices, meshBuffer, NULL, indicesBuffer, &pMesh->m_staticMeshId);
		}
		else
		{
			RecreateStaticBuffer(pMesh->m_staticMeshId, VT_POSITION_NORMAL_COLOUR, pMesh->m_materialId, -1, numVertices, 0, numIndices, meshBuffer, NULL, indicesBuffer);
		}
	}
	else if (pMesh->m_meshType == OGLMeshType_Textured)
	{
		if (pMesh->m_staticMeshId == -1)
		{
			CreateStaticBuffer(VT_POSITION_NORMAL_UV_COLOUR, pMesh->m_materialId, pMesh->m_textureId, numVertices, numTextureCoordinates, numIndices, meshBuffer, textureCoordinatesBuffer, indicesBuffer, &pMesh->m_staticMeshId);
		}
		else
		{
			RecreateStaticBuffer(pMesh->m_staticMeshId, VT_POSITION_NORMAL_UV_COLOUR, pMesh->m_materialId, pMesh->m_textureId, numVertices, numTextureCoordinates, numIndices, meshBuffer, textureCoordinatesBuffer, indicesBuffer);
		}
	}

	// Delete temp data
	delete[] meshBuffer;
	delete[] textureCoordinatesBuffer;
	delete[] indicesBuffer;
}

void Renderer::RenderMesh(OpenGLTriangleMesh* pMesh)
{
	PushMatrix();
		//SetCullMode(CM_NOCULL);
		SetPrimativeMode(PM_TRIANGLES);
		//SetRenderMode(RM_SOLID);
		if (pMesh->m_staticMeshId != -1)
		{
			RenderStaticBuffer(pMesh->m_staticMeshId);
		}
	PopMatrix();
}

void Renderer::RenderMesh_NoColour(OpenGLTriangleMesh* pMesh)
{
	PushMatrix();
		//SetCullMode(CM_NOCULL);
		SetPrimativeMode(PM_TRIANGLES);
		//SetRenderMode(RM_SOLID);
		if (pMesh->m_staticMeshId != -1)
		{
			RenderStaticBuffer_NoColour(pMesh->m_staticMeshId);
		}
	PopMatrix();
}

void Renderer::GetMeshInformation(int *numVerts, int *numTris, OpenGLTriangleMesh* pMesh)
{
	*numVerts = (int)pMesh->m_vertices.size();
	*numTris = (int)pMesh->m_triangles.size();
}

void Renderer::StartMeshRender()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void Renderer::EndMeshRender()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

bool Renderer::MeshStaticBufferRender(OpenGLTriangleMesh* pMesh)
{
	SetPrimativeMode(PM_TRIANGLES);
	//SetRenderMode(RM_SOLID);

	VertexArray *pVertexArray = m_vertexArrays[pMesh->m_staticMeshId];

	if (pVertexArray != NULL)
	{
		if (pVertexArray->nVerts == 0)
		{
			return false;
		}

		if ((pVertexArray->type != VT_POSITION_DIFFUSE_ALPHA) && (pVertexArray->type != VT_POSITION_DIFFUSE))
		{
			if (pVertexArray->materialID != -1)
			{
				m_materials[pVertexArray->materialID]->Apply();
			}
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR)
		{
			if (pVertexArray->textureID != -1)
			{
				BindTexture(pVertexArray->textureID);
			}
		}

		// Calculate the stride
		GLsizei totalStride = GetStride(pVertexArray->type);

		glVertexPointer(3, GL_FLOAT, totalStride, pVertexArray->pVA);

		if (pVertexArray->type == VT_POSITION_NORMAL || pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR || pVertexArray->type == VT_POSITION_NORMAL_COLOUR)
		{
			glNormalPointer(GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV || pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR)
		{
			glTexCoordPointer(2, GL_FLOAT, 0, pVertexArray->pTextureCoordinates);
		}

		if (pVertexArray->type == VT_POSITION_DIFFUSE_ALPHA)
		{
			glColorPointer(4, GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_DIFFUSE)
		{
			glColorPointer(3, GL_FLOAT, totalStride, &pVertexArray->pVA[3]);
		}

		if (pVertexArray->type == VT_POSITION_NORMAL_UV_COLOUR || pVertexArray->type == VT_POSITION_NORMAL_COLOUR)
		{
			glColorPointer(4, GL_FLOAT, totalStride, &pVertexArray->pVA[6]);
		}

		if (pVertexArray->nIndices != 0)
		{
			glDrawElements(m_primativeMode, pVertexArray->nIndices, GL_UNSIGNED_INT, pVertexArray->pIndices);
		}
		else
		{
			glDrawArrays(m_primativeMode, 0, pVertexArray->nVerts);
		}

		return true;
	}

	return false;
}

// Name rendering and name picking
void Renderer::InitNameStack()
{
	glInitNames();

	glPushName(-1);
}

void Renderer::LoadNameOntoStack(int lName)
{
	glPopName();
	glPushName(lName);
	//glLoadName(lName);
}

void Renderer::EndNameStack()
{
	glPopName();
}

void Renderer::StartNamePicking(unsigned int lViewportid, int lX, int lY)
{
	ClearScene(true, true, true);

	glSelectBuffer(NAME_PICKING_BUFFER, m_SelectBuffer);

	glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int	viewportCoords[4] = { 0, 0, 0, 0 };
	glGetIntegerv(GL_VIEWPORT, viewportCoords);
	gluPickMatrix(lX, lY, 3, 3, viewportCoords);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	MultViewProjection();
}

int Renderer::GetPickedObject()
{
	int objectsFound = 0;
	objectsFound = glRenderMode(GL_RENDER);

	if (objectsFound > 0)
	{
		unsigned int lowestDepth = m_SelectBuffer[1];

		int selectedObject = m_SelectBuffer[3];

		// Any time that we find more than one named object, we choose the one with the lowest depth. i.e Closest to the screen
		for (int i = 1; i < objectsFound; i++)
		{
			unsigned int lDepth = m_SelectBuffer[(i * 4) + 1];
			int lname = m_SelectBuffer[(i * 4) + 3];

			// Also make sure that a name of -1 doesnt ever take priority
			if (lDepth < lowestDepth && lname != -1)
			{
				lowestDepth = m_SelectBuffer[(i * 4) + 1];

				selectedObject = m_SelectBuffer[(i * 4) + 3];
			}
		}

		return selectedObject;
	}

	//No objects found
	return -1;
}

// Frustum
Frustum* Renderer::GetFrustum(unsigned int frustumid)
{
	Frustum* pFrustum = m_frustums[frustumid];

	return pFrustum;
}

int Renderer::PointInFrustum(unsigned int frustumid, const Vector3d &point)
{
	Frustum* pFrustum = m_frustums[frustumid];

	return pFrustum->PointInFrustum(point);
}

int Renderer::SphereInFrustum(unsigned int frustumid, const Vector3d &point, float radius)
{
	Frustum* pFrustum = m_frustums[frustumid];

	return pFrustum->SphereInFrustum(point, radius);
}

int Renderer::CubeInFrustum(unsigned int frustumid, const Vector3d &center, float x, float y, float z)
{
	Frustum* pFrustum = m_frustums[frustumid];

	return pFrustum->CubeInFrustum(center, x, y, z);
}