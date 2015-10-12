#pragma once

#include "BoundingBox.h"
#include "../Renderer/Renderer.h"


// OBJ Model structs
typedef struct {
	Vector3d position;
	int massID;
	Vector3d massOffset;
} OBJ_Vertex;

typedef struct {
	int *pIndices;    // List of vertex indices.
	int numPoints;    // Number of vertex points.
} OBJ_Face;

typedef struct {
	float u;
	float v;
} OBJ_TextureCoordinate;


// OBJ Model class
class OBJModel
{
public:
	OBJModel(Renderer *lpRenderer);
	~OBJModel();

	bool Load(const char *modelFileName, const char *textureFileName);

	void CalculateBoundingBox();
	BoundingBox* GetBoundingBox();

	void Update(float dt);

	void Render();
	void RenderMesh();
	void RenderWireFrame();
	void RenderBoundingBox();

private:
	Renderer *mpRenderer;

	string m_ModelFilename;
	string m_TextureFilename;

	// Vertices
	int m_numVertices;
	OBJ_Vertex *m_pVertices;

	// Texture Coordinates
	int m_numTexCoordinates;
	OBJ_TextureCoordinate *m_pTextureCoordinates;

	// Normals
	int m_numNormals;
	Vector3d *m_pNormals;

	// Faces
	int m_numFaces;	
	OBJ_Face *m_pFaces;

	// Texture
	unsigned int m_texture;

	// Bounding box
	BoundingBox m_BoundingBox;
};
