#include "stdafx.h"
#include "Texture.h"

void Texture::Load()
{
	glGenTextures(1, &textureId);
	glBindTexture(textureResPtr->type, textureId);
	glTexParameteri(textureResPtr->type, GL_TEXTURE_MIN_FILTER, textureResPtr->min_filter);
	glTexParameteri(textureResPtr->type, GL_TEXTURE_MAG_FILTER, textureResPtr->mag_filter);
	glTexParameteri(textureResPtr->type, GL_TEXTURE_WRAP_S, textureResPtr->wrap_s);
	glTexParameteri(textureResPtr->type, GL_TEXTURE_WRAP_T, textureResPtr->wrap_t);
	int width, height, bpp;
	char* pointer_textura = LoadTGA(textureResPtr->path, &width, &height, &bpp);
	bpp == 24 ? //then
		glTexImage2D(textureResPtr->type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pointer_textura):
		        //else
		glTexImage2D(textureResPtr->type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura);
	delete[] pointer_textura;
	//glBindTexture(textureResPtr->type, 0);
}