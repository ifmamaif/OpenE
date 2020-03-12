#pragma once
#include <iostream>
#include <fstream>

#include "../Utilities/utilities.h"
#include "Vertex.h"

using namespace std;

class ShaderResource
{
private:
	friend class Shader;
	char* vertexShader;
	char* fragmentShader;
public:
	ShaderResource(char * const &_vs, char * const &_fs) :vertexShader(_vs), fragmentShader(_fs) {}
	ShaderResource(){ cout << "Constructor implicit apelat -ShaderResource: Undefined behaviour.\n"; }
	~ShaderResource() { delete[] vertexShader; delete[] fragmentShader; }
};

class Shader
{
protected:
	ShaderResource* shaderResPtr;
	GLuint vertexShaderId, fragmentShaderId, programId;

	GLint positionAttribute, colorAttribute,
		normAttribute, binormAttribute, tgtAttribute,
		uvAttribute, uvBlendAttribute,
		viewProjectionMatrixUniform, moveTextureUniform,
		lightMatrixUniform[6],
		texture1Uniform, texture2Uniform, texture3Uniform, texture4Uniform,
		lightDepth1Uniform[6], lightDepth2Uniform[6], lightDepth3Uniform[6], lightDepth4Uniform[6],
		textureBlendUniform,
		modelMatrixUniform, normalMatrixUniform, fogColorUniform,
		rUniform, RUniform, cameraPosUniform,
		heightMapUniform, u_TimeUniform, u_DispMaxUniform,
		reflectivenessUniform,
		colorUniform,
		debugObjectUniform,
		lightType1Uniform, lightType2Uniform, lightType3Uniform, lightType4Uniform,
		lightPos1Uniform, lightPos2Uniform, lightPos3Uniform, lightPos4Uniform,
		lightDir1Uniform, lightDir2Uniform, lightDir3Uniform, lightDir4Uniform,
		lightColor1Uniform, lightColor2Uniform, lightColor3Uniform, lightColor4Uniform,
		specularPow1Uniform, specularPow2Uniform, specularPow3Uniform, specularPow4Uniform,
		alpha1Uniform, alpha2Uniform, alpha3Uniform, alpha4Uniform,
		ambiental_light_colorUniform, ratioUniform,
		kDiffuseUniform, kSpecularUniform

		;
public:	
	Shader(ShaderResource * const &_shaderResPtr) :shaderResPtr(_shaderResPtr) {};
	Shader() { cout << "Constructor implicit apelat - Shader: Undefined behaviour.\n"; };
	void Load();
	void Activate(){ glUseProgram(programId); }
	void Use() {
		glUseProgram(programId);
		if (positionAttribute != -1)glEnableVertexAttribArray(positionAttribute);
		if (colorAttribute != -1)glEnableVertexAttribArray(colorAttribute);
		if (normAttribute != -1)glEnableVertexAttribArray(normAttribute);
		if (binormAttribute != -1)glEnableVertexAttribArray(binormAttribute);
		if (tgtAttribute != -1)glEnableVertexAttribArray(tgtAttribute);
		if (uvAttribute != -1)glEnableVertexAttribArray(uvAttribute);
		if (uvBlendAttribute != -1)glEnableVertexAttribArray(uvBlendAttribute);

		if (positionAttribute != -1)
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		if (colorAttribute != -1)
		glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		if (normAttribute != -1)
		glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
		if (binormAttribute != -1)
		glVertexAttribPointer(binormAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, binorm));
		if (tgtAttribute != -1)
		glVertexAttribPointer(tgtAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tgt));
		if (uvAttribute != -1)
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		if (uvBlendAttribute != -1)
		glVertexAttribPointer(uvBlendAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvBlend));
		
	}
	void SendViewProjectionMatrix(float * const &ViewProjectionMatrix)
	{
		if (viewProjectionMatrixUniform != -1)
			glUniformMatrix4fv(viewProjectionMatrixUniform, 1, GL_FALSE, ViewProjectionMatrix);
	}
	void SendNormalMatrix(float * const &NormalMatrix)
	{
		if (normalMatrixUniform != -1)
			glUniformMatrix4fv(normalMatrixUniform, 1, GL_FALSE, NormalMatrix);
	}
	void SendMoveTexture(float * const &move_texture)
	{
		if (moveTextureUniform != -1)
			glUniform2f(moveTextureUniform, move_texture[0], move_texture[1]);
	}
	void SendFogColor(float * const &fogColor)
	{
		if (fogColorUniform != -1)
			glUniform3f(fogColorUniform, fogColor[0], fogColor[1], fogColor[2]);
	}
	void SendModelMatrix(float * const &modelMatrix)
	{
		if (modelMatrixUniform != -1)
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	}
	void SendCameraPos(float * const &cameraPos)
	{
		if (cameraPosUniform != -1)
			glUniform3f(cameraPosUniform, cameraPos[0], cameraPos[1], cameraPos[2]);
	}
	void Send_r(float const &r)
	{
		if (rUniform != -1)
			glUniform1f(rUniform, r);
	}
	void Send_R(float const &R)
	{
		if (RUniform != -1)
			glUniform1f(RUniform, R);
	}
	void SendHeightMap(float * const &heightMap)
	{
		if (heightMapUniform != -1)
			glUniform3f(heightMapUniform, heightMap[0], heightMap[1], heightMap[2]);
	}
	void Send_u_Time(float const &u_Time)
	{
		if (u_TimeUniform != -1)
			glUniform1f(u_TimeUniform, u_Time);
	}
	void Send_u_DispMax(float const &u_DispMax)
	{
		if (u_DispMaxUniform != -1)
			glUniform1f(u_DispMaxUniform, u_DispMax);
	}
	void SendReflectiveness(float const &reflectiveness)
	{
		if (reflectivenessUniform != -1)
			glUniform1f(reflectivenessUniform, reflectiveness);
	}
	void SendColor(float * const &color)
	{
		if (colorUniform != -1)
			glUniform3f(colorUniform, color[0], color[1], color[2]);
	}
	void SendAmbientalLightColor(float * const &color)
	{
		if (ambiental_light_colorUniform != -1)
			glUniform3f(ambiental_light_colorUniform, color[0], color[1], color[2]);
	}
	void SendRatio(float const &ratio)
	{
		if (ratioUniform != -1)
			glUniform1f(ratioUniform, ratio);
	}
	void SendKDiffuse(float const &kDiffuse)
	{
		if (kDiffuseUniform != -1)
			glUniform1f(kDiffuseUniform, kDiffuse);
	}
	void SendKSpecular(float const &kSpecular)
	{
		if (kSpecularUniform != -1)
			glUniform1f(kSpecularUniform, kSpecular);
	}
	void isWired()
	{
		if (debugObjectUniform != -1)
			glUniform1i(debugObjectUniform, 1);
	}
	void isAABB()
	{
		if (debugObjectUniform != -1)
			glUniform1i(debugObjectUniform, 0);
	}
	void SendLightType(float const * const &type, int const&i)
	{
		switch (i)
		{
		case 0:if (lightType1Uniform != -1)
			glUniform3f(lightType1Uniform, type[0], type[1], type[2]);
			break;
		case 1:if (lightType2Uniform != -1)
			glUniform3f(lightType2Uniform, type[0], type[1], type[2]);
			break;
		case 2:if (lightType3Uniform != -1)
			glUniform3f(lightType3Uniform, type[0], type[1], type[2]);
			break;
		case 3:if (lightType4Uniform != -1)
			glUniform3f(lightType4Uniform, type[0], type[1], type[2]);
			break;
		}
	}

	void SendLightViewProjectionMatrix(float * const &ViewProjectionMatrix, int const &i)
	{
		if (lightMatrixUniform[i] != -1)
			glUniformMatrix4fv(lightMatrixUniform[i], 1, GL_FALSE, ViewProjectionMatrix);
	}

	void SendLightPos(float * const &position, int const &i)
	{
		switch (i)
		{
		case 0:if (lightPos1Uniform != -1)
			glUniform3f(lightPos1Uniform, position[0], position[1], position[2]);
			break;
		case 1:if (lightPos2Uniform != -1)
			glUniform3f(lightPos2Uniform, position[0], position[1], position[2]);
			break;
		case 2:if (lightPos3Uniform != -1)
			glUniform3f(lightPos3Uniform, position[0], position[1], position[2]);
			break;
		case 3:if (lightPos4Uniform != -1)
			glUniform3f(lightPos4Uniform, position[0], position[1], position[2]);
			break;
		}
	}
	void SendLightDir(float * const &direction, int const &i)
	{
		switch (i)
		{
		case 0:if (lightDir1Uniform != -1)
			glUniform3f(lightDir1Uniform, direction[0], direction[1], direction[2]);
			break;
		case 1:if (lightDir2Uniform != -1)
			glUniform3f(lightDir2Uniform, direction[0], direction[1], direction[2]);
			break;
		case 2:if (lightDir3Uniform != -1)
			glUniform3f(lightDir3Uniform, direction[0], direction[1], direction[2]);
			break;
		case 3:if (lightDir4Uniform != -1)
			glUniform3f(lightDir4Uniform, direction[0], direction[1], direction[2]);
			break;
		}
	}
	void SendLightColor(float * const &color, int const &i)
	{
		switch (i)
		{
		case 0:if (lightColor1Uniform != -1)
			glUniform3f(lightColor1Uniform, color[0], color[1], color[2]);
			break;
		case 1:if (lightColor2Uniform != -1)
			glUniform3f(lightColor2Uniform, color[0], color[1], color[2]);
			break;
		case 2:if (lightColor3Uniform != -1)
			glUniform3f(lightColor3Uniform, color[0], color[1], color[2]);
			break;
		case 3:if (lightColor4Uniform != -1)
			glUniform3f(lightColor4Uniform, color[0], color[1], color[2]);
			break;
		}
	}
	void SendSpecularPow(float const &specular_power, int const &i)
	{
		switch (i)
		{
		case 0:if (specularPow1Uniform != -1)
			glUniform1f(specularPow1Uniform, specular_power);
			break;
		case 1:if (specularPow2Uniform != -1)
			glUniform1f(specularPow2Uniform, specular_power);
			break;
		case 2:if (specularPow3Uniform != -1)
			glUniform1f(specularPow3Uniform, specular_power);
			break;
		case 3:if (specularPow4Uniform != -1)
			glUniform1f(specularPow4Uniform, specular_power);
			break;
		}
	}
	void SendAlpha(float const &alpha, int const &i)
	{
		switch (i)
		{
		case 0:if (alpha1Uniform != -1)
			glUniform1f(alpha1Uniform, alpha);
			break;
		case 1:if (alpha2Uniform != -1)
			glUniform1f(alpha2Uniform, alpha);
			break;
		case 2:if (alpha3Uniform != -1)
			glUniform1f(alpha3Uniform, alpha);
			break;
		case 3:if (alpha4Uniform != -1)
			glUniform1f(alpha4Uniform, alpha);
			break;
		}
	}
	~Shader() {
		glDeleteProgram(programId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}
};