#include "stdafx.h"
#include "Texture.h"

void Texture::Load()
{
	glGenTextures(1, &textureId);
	if (textureResPtr->type == GL_TEXTURE_2D)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureResPtr->min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureResPtr->mag_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureResPtr->wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureResPtr->wrap_t);
		int width, height, bpp;
		char* pointer_textura = LoadTGA(textureResPtr->path, &width, &height, &bpp);
		bpp == 24 ? //then
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura) :
			//else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura);
		delete[] pointer_textura;
	}
	else if (textureResPtr->type == GL_TEXTURE_CUBE_MAP)
	{
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, textureResPtr->min_filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, textureResPtr->mag_filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, textureResPtr->wrap_s);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, textureResPtr->wrap_t);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R_OES, textureResPtr->wrap_r);
		int width, height, bpp;
		char* pointer_textura = LoadTGA(textureResPtr->path, &width, &height, &bpp);
		int faceWidth = width / 4, faceHeight = height / 3, texelSize;

		if (bpp == 24) //then
		{
			texelSize = 3;
			//Create memory for textures
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);			
			//Set textures
			int i;
			for (i = 0; i < faceHeight; i++)
			{
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, i, faceWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura + ((i + faceHeight)*width + 2 * faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, i, faceWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura + (i + faceHeight)*width * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, i, faceWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura + (i*width + faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, i, faceWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura + ((i + 2 * faceHeight)*width + faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, i, faceWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura + ((i + faceHeight)*width + faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, i, faceWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura + ((i + faceHeight)*width + 3 * faceWidth) * texelSize);
			}
		}			
		else
		{
			texelSize = 4;
			//Create memory for textures
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			//Set textures
			int i;
			for (i = 0; i < faceHeight; i++)
			{
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, i, faceWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura + ((i + faceHeight)*width + 2 * faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, i, faceWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura + (i + faceHeight)*width * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, i, faceWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura + (i*width + faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, i, faceWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura + ((i + 2 * faceHeight)*width + faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, i, faceWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura + ((i + faceHeight)*width + faceWidth) * texelSize);
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, i, faceWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura + ((i + faceHeight)*width + 3 * faceWidth) * texelSize);
			}
		}
		delete[] pointer_textura;	
	}
	
	//glBindTexture(textureResPtr->type, 0);
}