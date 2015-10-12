// ******************************************************************************
//
// Filename:	VoxelObject.cpp
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 11/08/14
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#include "VoxelObject.h"


VoxelObject::VoxelObject()
{
	m_usingQubicleManager = false;

	Reset();
}

void VoxelObject::SetOpenGLRenderer(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

void VoxelObject::SetQubicleBinaryManager(QubicleBinaryManager* pQubicleBinaryManager)
{
	m_pQubicleBinaryManager = pQubicleBinaryManager;
}

VoxelObject::~VoxelObject()
{
	UnloadObject();
	Reset();
}

void VoxelObject::Reset()
{
	m_pVoxelModel = NULL;

	m_loaded = false;
}

QubicleBinary* VoxelObject::GetQubicleModel()
{
	return m_pVoxelModel;
}

Matrix4x4 VoxelObject::GetModelMatrix(int qubicleMatrixIndex)
{
	if(m_pVoxelModel == NULL || m_loaded == false)
	{
		return Matrix4x4();
	}

	return m_pVoxelModel->GetModelMatrix(qubicleMatrixIndex);
}

Vector3d VoxelObject::GetCenter()
{
	if(m_pVoxelModel == NULL || m_loaded == false)
	{
		return Vector3d(0.0f, 0.0f, 0.0f);
	}

	Vector3d centerPos;
	for(int i = 0; i < m_pVoxelModel->GetNumMatrices(); i++)
	{
		QubicleMatrix* pMatrix = m_pVoxelModel->GetQubicleMatrix(i);

		centerPos.x += (pMatrix->m_matrixSizeX*0.5f)*pMatrix->m_scale;
		centerPos.y += (pMatrix->m_matrixSizeY*0.5f)*pMatrix->m_scale;
		centerPos.z += (pMatrix->m_matrixSizeZ*0.5f)*pMatrix->m_scale;
	}

	centerPos /= (float)m_pVoxelModel->GetNumMatrices();

	return centerPos;
}

void VoxelObject::LoadObject(const char *qbFilename, bool useManager)
{
	m_usingQubicleManager = useManager;

	if(m_pVoxelModel == NULL)
	{
		if(useManager)
		{
			m_pVoxelModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qbFilename, false);
		}
		else
		{
			m_pVoxelModel = new QubicleBinary(m_pRenderer);
			m_pVoxelModel->Import(qbFilename);
		}
	}

	m_loaded = true;
}

void VoxelObject::UnloadObject()
{
	if(m_loaded)
	{
		if(m_usingQubicleManager == false)
		{
			delete m_pVoxelModel;
		}

		m_pVoxelModel = NULL;
	}

	m_loaded = false;
}

// Rendering modes
void VoxelObject::SetWireFrameRender(bool wireframe)
{
	if(m_pVoxelModel != NULL)
	{
		m_pVoxelModel->SetWireFrameRender(wireframe);
	}
}

void VoxelObject::SetMeshAlpha(float alpha)
{
	if(m_pVoxelModel != NULL)
	{
		m_pVoxelModel->SetMeshAlpha(alpha);
	}
}

void VoxelObject::SetMeshSingleColour(float r, float g, float b)
{
	if(m_pVoxelModel != NULL)
	{
		m_pVoxelModel->SetMeshSingleColour(r, g, b);
	}
}

void VoxelObject::SetForceTransparency(bool force)
{
	if(m_pVoxelModel != NULL)
	{
		m_pVoxelModel->SetForceTransparency(force);
	}
}

void VoxelObject::Update(float dt)
{
	if(m_loaded == false)
	{
		return;
	}
}

void VoxelObject::Render(bool renderOutline, bool reflection, bool silhouette, Colour OutlineColour)
{
	if(m_pVoxelModel != NULL)
	{
		m_pVoxelModel->Render(renderOutline, reflection, silhouette, OutlineColour);
	}
}