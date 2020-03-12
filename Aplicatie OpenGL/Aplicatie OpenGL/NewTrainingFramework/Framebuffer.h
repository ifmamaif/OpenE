#pragma once
#include <iostream>
#include "Globals.h"
#include "../Utilities/utilities.h"

using namespace std;

class FrameBuffer
{
private:
	GLuint frameBuffer, depthTexture[6], texture;
	bool hasDepth, isPointLight;
public:
	FrameBuffer(bool const &_hasDepth = false, bool const &_isPointLight = false);
	void Bind();
	void BindForRendering();
	void UseTexture(int const &ActiveTexture = 0);
	void UseDepthTexture(int const &ActiveTexture = 0, int const &face = 0);
	void BindFace(const int &face);
	~FrameBuffer()
	{
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &texture);
		if (hasDepth)
		{
			glDeleteTextures(1, &depthTexture[0]);
			if (isPointLight)
				for (int i = 1; i < 6; i++)
					glDeleteTextures(1, &depthTexture[i]);
		}
		
	}
};
