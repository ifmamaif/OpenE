#pragma once
#include <iostream>
#include "../Utilities/utilities.h"

using namespace std;

class TextureResource
{
private:
	friend class Texture;
	GLenum type;
	char* path;
	GLenum min_filter, mag_filter;
	GLenum wrap_s, wrap_t, wrap_r;
public:
	TextureResource(GLenum const &_type, char * const &_path, GLenum const &_min_filter,
		GLenum const &_mag_filter, GLenum const &_wrap_s, GLenum const &_wrap_t, GLenum const &_wrap_r = GL_CLAMP_TO_EDGE):
	type(_type), path(_path), min_filter(_min_filter), mag_filter(_mag_filter), wrap_s(_wrap_s), wrap_t(_wrap_t), wrap_r(_wrap_r){}
	TextureResource(){ cout << "Constructor implicit apelat -TextureResource: Undefined behaviour.\n"; }
	~TextureResource() { delete[] path; }
};

class Texture
{
private:
	TextureResource* textureResPtr;
	GLuint textureId;
public:
	Texture(TextureResource * const &_textureResPtr) :textureResPtr(_textureResPtr) {};
	Texture(){ cout << "Constructor implicit apelat - Texture: Undefined behaviour.\n"; }
	void Load();
	void Bind(int const &i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(textureResPtr->type, textureId);
	}
	~Texture() { glDeleteTextures(1, &textureId); };
};
