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
	viewProjectionMatrixUniform = glGetUniformLocation(programId, "viewProjectionMatrix");
	moveTextureUniform = glGetUniformLocation(programId, "moveTexture");
	texture1Uniform = glGetUniformLocation(programId, "texture1");
	texture2Uniform = glGetUniformLocation(programId, "texture2");
	texture3Uniform = glGetUniformLocation(programId, "texture3");
	texture4Uniform = glGetUniformLocation(programId, "texture4");
	textureBlendUniform = glGetUniformLocation(programId, "textureBlend");
	modelMatrixUniform = glGetUniformLocation(programId, "modelMatrix");
	normalMatrixUniform = glGetUniformLocation(programId, "normalMatrix");
	fogColorUniform = glGetUniformLocation(programId, "fogColor");
	rUniform = glGetUniformLocation(programId, "r");
	RUniform = glGetUniformLocation(programId, "R");
	cameraPosUniform = glGetUniformLocation(programId, "cameraPos");
	heightMapUniform = glGetUniformLocation(programId, "heightMap");
	u_TimeUniform = glGetUniformLocation(programId, "u_Time");
	u_DispMaxUniform = glGetUniformLocation(programId, "u_DispMax");
	reflectivenessUniform = glGetUniformLocation(programId, "reflectiveness");
	colorUniform = glGetUniformLocation(programId, "color");
	debugObjectUniform = glGetUniformLocation(programId, "debugObject");

	lightType1Uniform = glGetUniformLocation(programId, "lightType1");
	lightType2Uniform = glGetUniformLocation(programId, "lightType2");
	lightType3Uniform = glGetUniformLocation(programId, "lightType3");
	lightType4Uniform = glGetUniformLocation(programId, "lightType4");

	lightPos1Uniform = glGetUniformLocation(programId, "lightPos1");
	lightPos2Uniform = glGetUniformLocation(programId, "lightPos2");
	lightPos3Uniform = glGetUniformLocation(programId, "lightPos3");
	lightPos4Uniform = glGetUniformLocation(programId, "lightPos4");

		lightDir1Uniform = glGetUniformLocation(programId, "lightDir1");
		lightDir2Uniform = glGetUniformLocation(programId, "lightDir2");
		lightDir3Uniform = glGetUniformLocation(programId, "lightDir3");
		lightDir4Uniform = glGetUniformLocation(programId, "lightDir4");

		lightColor1Uniform = glGetUniformLocation(programId, "lightColor1");
		lightColor2Uniform = glGetUniformLocation(programId, "lightColor2");
		lightColor3Uniform = glGetUniformLocation(programId, "lightColor3");
		lightColor4Uniform = glGetUniformLocation(programId, "lightColor4");

		specularPow1Uniform = glGetUniformLocation(programId, "specularPow1");
		specularPow2Uniform = glGetUniformLocation(programId, "specularPow2");
		specularPow3Uniform = glGetUniformLocation(programId, "specularPow3");
		specularPow4Uniform = glGetUniformLocation(programId, "specularPow4");

		alpha1Uniform = glGetUniformLocation(programId, "alpha1");
		alpha2Uniform = glGetUniformLocation(programId, "alpha2");
		alpha3Uniform = glGetUniformLocation(programId, "alpha3");
		alpha4Uniform = glGetUniformLocation(programId, "alpha4");

		ambiental_light_colorUniform = glGetUniformLocation(programId, "ambiental_light_color");
		ratioUniform = glGetUniformLocation(programId, "ratio");

		kDiffuseUniform = glGetUniformLocation(programId, "kDiffuse");
		kSpecularUniform = glGetUniformLocation(programId, "kSpecular");
	glUseProgram(programId);
	int i = 0;
	if (texture1Uniform != -1)glUniform1i(texture1Uniform, i++);
	if (texture2Uniform != -1)glUniform1i(texture2Uniform, i++);
	if (texture3Uniform != -1)glUniform1i(texture3Uniform, i++);
	if (texture4Uniform != -1)glUniform1i(texture4Uniform, i++);
	if (textureBlendUniform != -1)glUniform1i(textureBlendUniform, i);
	glUseProgram(0);
}
