// ******************************************************************************
//
// Filename:	QubicleBinaryManager.h
// Project:		Game
// Author:		Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 10/07/14
//
// Copyright (c) 2005-2015, Steven Ball
//
// ******************************************************************************

#pragma once


#include "QubicleBinary.h"

typedef std::vector<QubicleBinary*> QubicleBinaryList;


class QubicleBinaryManager
{
public:
	/* Public methods */
	QubicleBinaryManager(Renderer* pRenderer);
	~QubicleBinaryManager();

	void ClearQubicleBinaryList();

	QubicleBinary* GetQubicleBinaryFile(const char* fileName, bool refreshModel);
	QubicleBinary* AddQubicleBinaryFile(const char* fileName);

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
	Renderer* m_pRenderer;

	QubicleBinaryList m_vpQubicleBinaryList;
};
