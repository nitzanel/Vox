// Juice Engine by Steven Ball
// Mesh - A generic mesh object, used to render complex geometrix shapes
// Created : 16/04/12

#pragma once

#include <vector>
using namespace std;

#include "../Maths/3dGeometry.h"

// Vertex data
typedef struct OpenGLMesh_Vertex
{
    float vertexPosition[3];
    float vertexNormals[3];
	float vertexColour[4];
} OpenGLMesh_Vertex;


// Texture coordinate
typedef struct OpenGLMesh_TextureCoordinate
{
	float s, t;
} OpenGLMesh_TextureCoordinate;


// Triangle
typedef struct OpenGLMesh_Triangle
{
    unsigned int vertexIndices[3];
} OpenGLMesh_Triangle;


enum OGLMeshType
{
	OGLMeshType_Colour = 0,
	OGLMeshType_Textured,
};

class OpenGLTriangleMesh
{
public:
	OpenGLTriangleMesh();
	~OpenGLTriangleMesh();

public:
    vector<OpenGLMesh_Triangle*> m_triangles;
	vector<OpenGLMesh_Vertex*> m_vertices;
	vector<OpenGLMesh_TextureCoordinate*> m_textureCoordinates;

    unsigned int m_staticMeshId;

	unsigned int m_materialId;
	unsigned int m_textureId;

	OGLMeshType m_meshType;
};