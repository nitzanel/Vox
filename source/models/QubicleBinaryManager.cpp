// ******************************************************************************
//
// Filename:	QubicleBinaryManager.cpp
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 10/07/14
//
// Copyright (c) 2005-2011, Steven Ball
//
// ******************************************************************************

#include "QubicleBinaryManager.h"


QubicleBinaryManager::QubicleBinaryManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

QubicleBinaryManager::~QubicleBinaryManager()
{
	ClearQubicleBinaryList();
}

void QubicleBinaryManager::ClearQubicleBinaryList()
{
	for(unsigned int i = 0; i < m_vpQubicleBinaryList.size(); i++)
	{
		delete m_vpQubicleBinaryList[i];
		m_vpQubicleBinaryList[i] = 0;
	}
	m_vpQubicleBinaryList.clear();
}

QubicleBinary* QubicleBinaryManager::GetQubicleBinaryFile(const char* fileName, bool refreshModel)
{
	for(unsigned int i = 0; i < m_vpQubicleBinaryList.size(); i++)
	{
		if(strcmp(m_vpQubicleBinaryList[i]->GetFileName().c_str(), fileName) == 0)
		{
			if(refreshModel)
			{
				m_vpQubicleBinaryList[i]->Reset();
				m_vpQubicleBinaryList[i]->Import(fileName);
			}

			return m_vpQubicleBinaryList[i];
		}
	}

	return AddQubicleBinaryFile(fileName);
}

QubicleBinary* QubicleBinaryManager::AddQubicleBinaryFile(const char* fileName)
{
	QubicleBinary* pNewQubicleBinary = new QubicleBinary(m_pRenderer);
	pNewQubicleBinary->Import(fileName);

	m_vpQubicleBinaryList.push_back(pNewQubicleBinary);

	return pNewQubicleBinary;
}