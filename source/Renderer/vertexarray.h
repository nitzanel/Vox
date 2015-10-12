// Juice Engine by Steven Ball
// VertexArray - A vertex array, used for static renderering
// Created : 02/04/06
// Edited  : 19/05/06

#pragma once

#include "Renderer.h"

enum VertexType {
	VT_POSITION = 0,
	VT_POSITION_DIFFUSE,
	VT_POSITION_DIFFUSE_ALPHA,
	VT_POSITION_NORMAL,
	VT_POSITION_NORMAL_COLOUR,
	VT_POSITION_NORMAL_UV,
	VT_POSITION_NORMAL_UV_COLOUR,
};

class VertexArray {
public:
	~VertexArray() {
		if(nVerts)
			delete pVA;

		if(nIndices)
			delete pIndices;

		if(nTextureCoordinates)
			delete pTextureCoordinates;

		nVerts = 0;
		nIndices = 0;
		nTextureCoordinates = 0;
	}

	VertexType type;
	unsigned int materialID;
	unsigned int textureID;
	int nVerts;
	int nTextureCoordinates;
	int nIndices;
	float *pVA;
	float *pTextureCoordinates;
	unsigned int *pIndices;
	int vertexSize;
	int textureCoordinateSize;
};