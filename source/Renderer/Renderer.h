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
#include "mesh.h"
#include "vertexarray.h"
#include "texture.h"
#include "material.h"
#include "light.h"


enum ProjectionMode
{
	PM_PERSPECTIVE = 0,
	PM_ORTHOGRAPHIC,
	PM_2D
};

enum RenderMode
{
	RM_WIREFRAME = 0,
	RM_SOLID,
	RM_SHADED,
	RM_TEXTURED,
	RM_TEXTURED_LIGHTING,
};

enum PrimativeMode
{
	PM_POINTS = 0,
	PM_LINES,
	PM_LINELIST,
	PM_TRIANGLES,
	PM_TRIANGLESTRIPS,
	PM_TRIANGLEFANS,
	PM_QUADS,
};

enum CullMode
{
	CM_NOCULL = 0,
	CM_BACK,
	CM_FRONT
};

enum BlendFunction
{
	BF_ONE = 0,
	BF_ZERO,
	BF_ONE_MINUS_SRC_ALPHA,
	BF_SRC_ALPHA
};

enum DepthTest
{
	DT_NEVER = 0,
	DT_ALWAYS,
	DT_LESS,
	DT_LEQUAL,
	DT_EQUAL,
	DT_GEQUAL,
	DT_GREATER,
	DT_NOTEQUAL,
};

enum ImmediateModePrimitive
{
	IM_POINTS = 0,
	IM_LINES,
	IM_LINE_LOOP,
	IM_LINE_STRIP,
	IM_TRIANGLES,
	IM_TRIANGLE_STRIP,
	IM_TRIANGLE_FAN,
	IM_QUADS,
	IM_QUAD_STRIP,
	IM_POLYGON
};

struct OGLPositionVertex
{
	float x, y, z; // Position.
};

struct OGLPositionDiffuseVertex
{
	float x, y, z;		// Position.
	float r, g, b;		// Colour.
};

struct OGLPositionDiffuseAlphaVertex
{
	float x, y, z;		// Position.
	float r, g, b;		// Colour.
	float a;			// Alpha
};

struct OGLPositionNormalVertex
{
	float x, y, z;		// Position.
	float nx, ny, nz;	// Normal.
};

struct OGLPositionNormalColourVertex
{
	float x, y, z;		// Position.
	float nx, ny, nz;	// Normal.
	float r, g, b;		// Colour
	float a;			// Alpha
};

struct OGLUVCoordinate
{
	float u, v;			// Texture coordinates
};

class Renderer
{
public:
	/* Public methods */
	Renderer(int width, int height, int depthBits, int stencilBits);
	~Renderer();

	void ResizeWindow(int newWidth, int newHeight);

	// Viewport
	bool CreateViewport(int bottom, int left, int width, int height, float fov, unsigned int *pID);
	bool ResizeViewport(unsigned int viewportid, int bottom, int left, int width, int height, float fov);
	int GetActiveViewPort();

	// Render modes
	void SetRenderMode(RenderMode mode);
	void SetPrimativeMode(PrimativeMode mode);
	void SetCullMode(CullMode mode);
	CullMode GetCullMode();
	void SetLineWidth(float width);
	void SetPointSize(float width);

	// Projection
	bool SetProjectionMode(ProjectionMode mode, int viewPort);
	void SetViewProjection();
	void MultViewProjection();

	// Scene
	bool ClearScene(bool pixel = true, bool depth = true, bool stencil = true);
	bool BeginScene(bool pixel = true, bool depth = true, bool stencil = true);
	void EndScene();

	// Push / Pop matrix stack
	void PushMatrix();
	void PopMatrix();

	// Matrix manipulations
	void SetWorldMatrix(const Matrix4x4 &mat);
	void GetModelViewMatrix(Matrix4x4 *pMat);
	void GetModelMatrix(Matrix4x4 *pMat);
	void GetViewMatrix(Matrix4x4 *pMat);
	void GetProjectionMatrix(Matrix4x4 *pMat);
	void IdentityWorldMatrix();
	void MultiplyWorldMatrix(const Matrix4x4 &mat);
	void TranslateWorldMatrix(float x, float y, float z);
	void RotateWorldMatrix(float x, float y, float z);
	void ScaleWorldMatrix(float x, float y, float z);

	// Texture matrix manipulations
	void SetTextureMatrix();
	void PushTextureMatrix();
	void PopTextureMatrix();

	// Camera functionality
	void SetLookAtCamera(Vector3d pos, Vector3d target, Vector3d up);

	// Transparency
	void EnableTransparency(BlendFunction source, BlendFunction destination);
	void DisableTransparency();
	GLenum GetBlendEnum(BlendFunction flag);

	// Depth testing
	void EnableDepthTest(DepthTest lTestFunction);
	void DisableDepthTest();
	GLenum GetDepthTest(DepthTest lTest);
	void EnableDepthWrite();
	void DisableDepthWrite();

	// Immediate mode
	void EnableImmediateMode(ImmediateModePrimitive mode);
	void ImmediateVertex(float x, float y, float z);
	void ImmediateVertex(int x, int y, int z);
	void ImmediateNormal(float x, float y, float z);
	void ImmediateNormal(int x, int y, int z);
	void ImmediateTextureCoordinate(float s, float t);
	void ImmediateColourAlpha(float r, float g, float b, float a);
	void DisableImmediateMode();

	// Text rendering
	bool CreateFreeTypeFont(char *fontName, int fontSize, unsigned int *pID);
	bool RenderFreeTypeText(unsigned int fontID, float x, float y, float z, Colour colour, float scale, char *inText, ...);
	int GetFreeTypeTextWidth(unsigned int fontID, char *inText, ...);
	int GetFreeTypeTextHeight(unsigned int fontID, char *inText, ...);
	int GetFreeTypeTextAscent(unsigned int fontID);
	int GetFreeTypeTextDescent(unsigned int fontID);

	// Lighting
	bool CreateLight(const Colour &ambient, const Colour &diffuse, const Colour &specular, Vector3d &position, Vector3d &direction, float exponent, float cutoff, float cAtten, float lAtten, float qAtten, bool point, bool spot, unsigned int *pID);
	bool EditLight(unsigned int id, const Colour &ambient, const Colour &diffuse, const Colour &specular, Vector3d &position, Vector3d &direction, float exponent, float cutoff, float cAtten, float qAtten, float lAtten, bool point, bool spot);
	bool EditLightPosition(unsigned int id, Vector3d &position);
	void DeleteLight(unsigned int id);
	void EnableLight(unsigned int id, unsigned int lightNumber);
	void DisableLight(unsigned int lightNumber);
	void RenderLight(unsigned int id);
	Colour GetLightAmbient(unsigned int id);
	Colour GetLightDiffuse(unsigned int id);
	Colour GetLightSpecular(unsigned int id);
	Vector3d GetLightPosition(unsigned int id);
	float GetConstantAttenuation(unsigned int id);
	float GetLinearAttenuation(unsigned int id);
	float GetQuadraticAttenuation(unsigned int id);

	// Materials
	bool CreateMaterial(const Colour &ambient, const Colour &diffuse, const Colour &specular, const Colour &emmisive, float specularPower, unsigned int *pID);
	bool EditMaterial(unsigned int id, const Colour &ambient, const Colour &diffuse, const Colour &specular, const Colour &emmisive, float specularPower);
	void EnableMaterial(unsigned int id);
	void DeleteMaterial(unsigned int id);

	// Textures
	bool LoadTexture(string filename, int *width, int *height, int *width_power2, int *height_power2, unsigned int *pID);
	bool RefreshTexture(unsigned int id);
	bool RefreshTexture(string filename);
	void BindTexture(unsigned int id);
	void DisableTexture();
	Texture* GetTexture(unsigned int id);
	void BindRawTextureId(unsigned int textureId);
	void GenerateEmptyTexture(unsigned int *pID);
	void SetTextureData(unsigned int id, int width, int height, unsigned char *texdata);

	// Vertex buffers
	bool CreateStaticBuffer(VertexType type, unsigned int materialID, unsigned int textureID, int nVerts, int nTextureCoordinates, int nIndices, const void *pVerts, const void *pTextureCoordinates, const unsigned int *pIndices, unsigned int *pID);
	bool RecreateStaticBuffer(unsigned int ID, VertexType type, unsigned int materialID, unsigned int textureID, int nVerts, int nTextureCoordinates, int nIndices, const void *pVerts, const void *pTextureCoordinates, const unsigned int *pIndices);
	void DeleteStaticBuffer(unsigned int id);
	bool RenderStaticBuffer(unsigned int id);
	bool RenderStaticBuffer_NoColour(unsigned int id);
	bool RenderFromArray(VertexType type, unsigned int materialID, unsigned int textureID, int nVerts, int nTextureCoordinates, int nIndices, const void *pVerts, const void *pTextureCoordinates, const unsigned int *pIndices);
	unsigned int GetStride(VertexType type);

	// Mesh
	OpenGLTriangleMesh* CreateMesh(OGLMeshType meshType);
	void ClearMesh(OpenGLTriangleMesh* pMesh);
	unsigned int AddVertexToMesh(Vector3d p, Vector3d n, float r, float g, float b, float a, OpenGLTriangleMesh* pMesh);
	unsigned int AddTextureCoordinatesToMesh(float s, float t, OpenGLTriangleMesh* pMesh);
	unsigned int AddTriangleToMesh(unsigned int vertexId1, unsigned int vertexId2, unsigned int vertexId3, OpenGLTriangleMesh* pMesh);
	void ModifyMeshAlpha(float alpha, OpenGLTriangleMesh* pMesh);
	void ModifyMeshColour(float r, float g, float b, OpenGLTriangleMesh* pMesh);
	void FinishMesh(unsigned int textureID, unsigned int materialID, OpenGLTriangleMesh* pMesh);
	void RenderMesh(OpenGLTriangleMesh* pMesh);
	void RenderMesh_NoColour(OpenGLTriangleMesh* pMesh);
	void GetMeshInformation(int *numVerts, int *numTris, OpenGLTriangleMesh* pMesh);
	void StartMeshRender();
	void EndMeshRender();
	bool MeshStaticBufferRender(OpenGLTriangleMesh* pMesh);

	// Name rendering and name picking
	void InitNameStack();
	void LoadNameOntoStack(int lName);
	void EndNameStack();
	void StartNamePicking(unsigned int lViewportid, int lX, int lY);
	int  GetPickedObject();

	// Frustum
	Frustum* GetFrustum(unsigned int frustumid);
	int PointInFrustum(unsigned int frustumid, const Vector3d &point);
	int SphereInFrustum(unsigned int frustumid, const Vector3d &point, float radius);
	int CubeInFrustum(unsigned int frustumid, const Vector3d &center, float x, float y, float z);

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

	// Primitive mode that we are currently operating in
	GLenum m_primativeMode;

	// Cull mode
	CullMode m_cullMode;

	// Viewports
	vector<Viewport *> m_viewports;
	unsigned int m_activeViewport;

	// Frustums
	vector<Frustum *> m_frustums; // Note : We store a frustum for each viewport, therefore viewport and frustum are closely linked (See viewport functions)

	// Materials
	vector<Material *> m_materials;

	// Textures
	vector<Texture *> m_textures;

	// Lights
	vector<Light *> m_lights;

	// Fonts
	vector<FreeTypeFont *> m_freetypeFonts;

	// Vertex arrays, for storing static vertex data
	vector<VertexArray *> m_vertexArrays;

	// Matrices
	Matrix4x4 *m_projection;
	Matrix4x4  m_view;
	Matrix4x4  m_model;

	// Model stack
	vector<Matrix4x4> m_modelStack;

	// Name picking
	static const int NAME_PICKING_BUFFER = 64;
	unsigned int m_SelectBuffer[NAME_PICKING_BUFFER];
};
