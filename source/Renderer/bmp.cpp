#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
//#pragma comment(lib, "glaux.lib")

#include <stdio.h>

#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library
#include "bmp_class.h"


int LoadFileBMP(const char *filename, unsigned char **pixels, int *width, int *height)
{
	AUX_RGBImageRec *pBitmap = NULL;
	FILE *pFile = NULL;

	if((pFile = fopen(filename, "rb")) == NULL) 
	{
		//MessageBox(hWnd, "Can't load BMP image!", "ERROR", MB_OK | MB_ICONINFORMATION);
		return NULL;
	}

	pBitmap = auxDIBImageLoad(filename);				

	(*width) = pBitmap->sizeX;
	(*height) = pBitmap->sizeY;
	(*pixels) = pBitmap->data;

	free(pBitmap);

	return 1;
}