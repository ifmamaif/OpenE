#include "stdafx.h"
#include "Shader.h"

void Shader::Load()
{
	
	vertexShaderId = esLoadShader(GL_VERTEX_SHADER, shaderResPtr->vertexShader);

	if (vertexShaderId == 0)
		return ;
	
	fragmentShaderId = esLoadShader(GL_FRAGMENT_SHADER, shaderResPtr->fragmentShader);

	if (fragmentShaderId == 0)
	{
		glDeleteShader(vertexShaderId);
		return ;
	}

	programId = esLoadProgram(vertexShaderId, fragmentShaderId);
	
	positionAttribute = glGetAttribLocation(programId, "a_position");
	colorAttribute = glGetAttribLocation(programId, "a_color"); 
	normAttribute = glGetAttribLocation(programId, "a_norm");
	binormAttribute = glGetAttribLocation(programId, "a_binorm");
	tgtAttribute = glGetAttribLocation(programId, "a_tgt");
	uvAttribute = glGetAttribLocation(programId, "a_uv");
	uvBlendAttribute = glGetAttribLocation(programId, "a_uvBlend");
	transfMatrixUniform = glGetUniformLocation(programId, "transfMatrix");
	moveTextureUniform = glGetUniformLocation(programId, "moveTexture");
	texture1Uniform = glGetUniformLocation(programId, "texture1");
	texture2Uniform = glGetUniformLocation(programId, "texture2");
	texture3Uniform = glGetUniformLocation(programId, "texture3");
	texture4Uniform = glGetUniformLocation(programId, "texture4");
	textureBlendUniform = glGetUniformLocation(programId, "textureBlend");
	modelMatrixUniform = glGetUniformLocation(programId, "modelMatrix");
	fogColorUniform = glGetUniformLocation(programId, "fogColor");
	rUniform = glGetUniformLocation(programId, "r");
	RUniform = glGetUniformLocation(programId, "R");
	cameraPosUniform = glGetUniformLocation(programId, "cameraPos");
	heightMapUniform = glGetUniformLocation(programId, "heightMap");
	glUseProgram(programId);
	int i = 0;
	if (texture1Uniform != -1)glUniform1i(texture1Uniform, i++);
	if (texture2Uniform != -1)glUniform1i(texture2Uniform, i++);
	if (texture3Uniform != -1)glUniform1i(texture3Uniform, i++);
	if (texture4Uniform != -1)glUniform1i(texture4Uniform, i++);
	if (textureBlendUniform != -1)glUniform1i(textureBlendUniform, i);
	glUseProgram(0);
}
