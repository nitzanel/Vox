// Juice Engine by Steven Ball
// Mesh - A generic mesh object, used to render complex geometrix shapes
// Created : 16/04/12

#include "mesh.h"

#include <cmath>


OpenGLTriangleMesh::OpenGLTriangleMesh()
{
    m_staticMeshId = -1;

	m_materialId = -1;
	m_textureId = -1;
}

OpenGLTriangleMesh::~OpenGLTriangleMesh()
{
	// Delete the vertices
	for(unsigned int i = 0; i < m_vertices.size(); i++) {
		delete m_vertices[i];
		m_vertices[i] = 0;
	}

	// Delete the texture coordinates
	for(unsigned int i = 0; i < m_textureCoordinates.size(); i++) {
		delete m_textureCoordinates[i];
		m_textureCoordinates[i] = 0;
	}
	
    // Delete the triangles
    for(unsigned int i = 0; i < m_triangles.size(); i++) {
        delete m_triangles[i];
        m_triangles[i] = 0;
    }
}