#pragma once
#include <iostream>
#include "Globals.h"
#include "../Utilities/utilities.h"

using namespace std;

class FrameBuffer
{
private:
	GLuint frameBuffer, depthTexture, texture;
	bool hasDepth, isPointLight;
public:
	FrameBuffer(bool const &_hasDepth = false, bool const &_isPointLight = false);
	void Bind();
	void UseTexture(int const &ActiveTexture = 0);
	void UseDepthTexture(int const &ActiveTexture = 0);
	void BindFace(const int &face);
	~FrameBuffer()
	{
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &texture);
		if (hasDepth)
			glDeleteTextures(1, &depthTexture);
	}
};
