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
		transfMatrixUniform, moveTextureUniform, 
		texture1Uniform, texture2Uniform, texture3Uniform, texture4Uniform,
		textureBlendUniform,
		modelMatrixUniform, fogColorUniform,
		rUniform, RUniform, cameraPosUniform,
		heightMapUniform
		
		;
public:	
	Shader(ShaderResource * const &_shaderResPtr) :shaderResPtr(_shaderResPtr) {};
	Shader() { cout << "Constructor implicit apelat - Shader: Undefined behaviour.\n"; };
	void Load();
	void Use() {
		glUseProgram(programId);
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

		if (positionAttribute != -1)glEnableVertexAttribArray(positionAttribute);
		if (colorAttribute != -1)glEnableVertexAttribArray(colorAttribute);
		if (normAttribute != -1)glEnableVertexAttribArray(normAttribute);
		if (binormAttribute != -1)glEnableVertexAttribArray(binormAttribute);
		if (tgtAttribute != -1)glEnableVertexAttribArray(tgtAttribute);
		if (uvAttribute != -1)glEnableVertexAttribArray(uvAttribute);
		if (uvBlendAttribute != -1)glEnableVertexAttribArray(uvBlendAttribute);
	}
	void SendMVP(float * const &MVP)
	{
		if (transfMatrixUniform != -1)
			glUniformMatrix4fv(transfMatrixUniform, 1, GL_FALSE, MVP);
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
		glUniform1f(rUniform, r);
	}
	void Send_R(float const &R)
	{
		glUniform1f(RUniform, R);
	}
	void SendHeightMap(float * const &heightMap)
	{
		if (heightMapUniform != -1)
			glUniform3f(heightMapUniform, heightMap[0], heightMap[1], heightMap[2]);
	}
	~Shader() {
		glDeleteProgram(programId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}
};