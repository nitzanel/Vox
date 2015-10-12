#include "texture.h"
#include "tga.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif //GL_CLAMP_TO_EDGE

inline int next_p2(int a)
{
	int rval = 2;

	while(rval < a)
		rval <<= 1;

	return rval;
}

Texture::Texture() {
}

Texture::~Texture() {
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}

int Texture::GetWidthPower2()
{
	return m_width_power2;
}

int Texture::GetHeightPower2()
{
	return m_height_power2;
}

GLuint Texture::GetId() const
{
	return m_id;
}

TextureFileType Texture::GetFileType() const
{
	return m_filetype;
}

bool Texture::Load(string fileName, int *width, int *height, int *width_power2, int *height_power2, bool refresh)
{
	m_fileName = fileName;

	bool lbNeedScaling = false;
	int lNumchannels = 0;
	unsigned char *texdata = 0;
	unsigned char *texdata_power2 = 0;

	bool loaded = false;

	if(strstr(fileName.c_str(), ".jpg"))
	{
		// TODO : Add back in JPG support

		// JPG
		//loaded = LoadFileJPG(fileName.c_str(), &texdata, width, height) == 1;

		//m_filetype = TextureFileType_JPG;
		//lbNeedScaling = false;
		//lNumchannels = 3;
	}
	else if(strstr(fileName.c_str(), ".tga"))
	{
		// TGA
		loaded = LoadFileTGA(fileName.c_str(), &texdata, width, height, true) == 1;

		m_filetype = TextureFileType_TGA;
		lbNeedScaling = false;  // Was initially true but not really sure if this is needed or not...
		lNumchannels = 4;
	}
	else if(strstr(fileName.c_str(), ".bmp"))
	{
		// BMP
		loaded = LoadFileBMP(fileName.c_str(), &texdata, width, height) == 1;
		
		m_filetype = TextureFileType_BMP;
		lbNeedScaling = false;
		lNumchannels = 3;
	}

	if(loaded == false)
	{
		return false;
	}

	// Store the real width and height of this texture
	m_width = (*width);
	m_height = (*height);

	if(lbNeedScaling)
	{
		// Now get the next power of 2 for width and height, since we want our final texture to be a power of 2
		m_width_power2 = next_p2(m_width);
		m_height_power2 = next_p2(m_height);

		(*width_power2) = m_width_power2;
		(*height_power2) = m_height_power2;

		// Create space for the new power of 2 data
		texdata_power2	= new unsigned char[ m_width_power2 * m_height_power2 * 4 ];

		// Pointer to the original data
		unsigned char	*ptr= (&texdata)[0];

		// Pointer into the new power of 2 data
		unsigned char	*ptr_power2= (&texdata_power2)[0];

		// Go through the old data and insert it into the new expanded structure, or add padding if we need to...
		for(int y = 0; y < m_height_power2; y++)
		{
			for(int x = 0; x < m_width_power2; x++)
			{
				if(y < m_height && x < m_width)
				{
					// We dont need padding
					ptr_power2[0] = ptr[0];
					ptr_power2[1] = ptr[1];
					ptr_power2[2] = ptr[2];
					ptr_power2[3] = ptr[3];

					ptr += 4;
				}
				else
				{
					// We need padding 
					ptr_power2[0] = 0;
					ptr_power2[1] = 0;
					ptr_power2[2] = 0;
					ptr_power2[3] = 0;
				}

				ptr_power2 += 4;
			}
		}
	}
	else
	{
		m_width_power2 = m_width;
		m_height_power2 = m_height;
		(*width_power2) = m_width_power2;
		(*height_power2) = m_height_power2;
	}

    if(refresh == false)
    {
        // Create a new texture id, since we are loading a fully new texture
	    glGenTextures(1, &m_id);
    }

	glBindTexture(GL_TEXTURE_2D, m_id);

	// TODO : Are these good parameters for MAG and MIN filters??

	//if(mbMipMap)
	//{
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // GL_CLAMP
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // GL_CLAMP
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, (*width), (*height), GL_RGBA, GL_UNSIGNED_BYTE, texdata);
	//}
	//else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // GL_LINEAR

		int textureType = GL_RGB;

		if(lNumchannels == 4)
			textureType = GL_RGBA;

		if(lbNeedScaling)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width_power2, m_height_power2, 0, textureType, GL_UNSIGNED_BYTE, texdata_power2);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, textureType, GL_UNSIGNED_BYTE, texdata);
		}
	}



	if(lbNeedScaling)
	{
		// Delete the data now
		if(texdata_power2)
			delete[] texdata_power2;
	}

	if(texdata)
		delete[] texdata;

	return true;
}

void Texture::GenerateEmptyTexture()
{
	// Create a new texture id, since we are loading a fully new texture
	glGenTextures(1, &m_id);

	m_width = -1;
	m_height = -1;
	m_width_power2 = -1;
	m_height_power2 = -1;
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, m_id);
}