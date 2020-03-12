#include "stdafx.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(bool const & _hasDepth, bool const & _isPointLight)
{
	hasDepth = _hasDepth;
	isPointLight = _isPointLight;
	glGenFramebuffers(1, &frameBuffer);
	glGenTextures(1, &texture);
	if (hasDepth)
	{
		glGenTextures(1, depthTexture);
		if (isPointLight)
				glGenTextures(5, depthTexture + 1);
	}


	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Globals::screenWidth, Globals::screenHeight,
		0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (hasDepth)
	{
		if (isPointLight == false)
		{
			glBindTexture(GL_TEXTURE_2D, depthTexture[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Globals::screenWidth,
				Globals::screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT,
				NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			for (int i = 0; i < 6; i++)
			{
				glBindTexture(GL_TEXTURE_2D, depthTexture[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 900,
					900, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT,
					NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	if (hasDepth && !isPointLight)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture[0], 0);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::BindForRendering()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	if (hasDepth)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::UseTexture(int const & ActiveTexture)
{
	glActiveTexture(GL_TEXTURE0 + ActiveTexture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void FrameBuffer::UseDepthTexture(int const & ActiveTexture, int const & face)
{
	if (!isPointLight)
	{
		glActiveTexture(GL_TEXTURE0 + ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture[0]);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture[face]);
	}
}

void FrameBuffer::BindFace(const int & face)
{
	if (face == -1)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, 0,
			0);
		return;
	}
	if (isPointLight)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, depthTexture[face],
			0);
	else cout << "Not a point light.";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindTexture(GL_TEXTURE_2D, 0);
}
