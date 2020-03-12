#pragma once
#include <unordered_map>
#include "../Utilities/utilities.h"
#include "rapidxml.hpp"
using namespace rapidxml;
using namespace std;
struct ModelResource
{
	int id;
	char* path;	
};

class Model
{
	ModelResource* mr;
	GLuint iboId; //Index Buffer Object Id
	GLuint wired_iboId;
	GLuint vboId; //Vertex Buffer Object Id
	GLuint nrIndices, wired_nrIndices;
public:
	Model() {};
	~Model() {};
	void Load();

};

struct TextureResource
{


};

class Texture
{
	TextureResource* tr;
	GLuint textureId;
public:
	Texture() {};
	~Texture() {};
	void Load();

};

struct ShaderResource
{


};

class Shader
{
	ShaderResource* sr;
	GLuint programId;
public:
	Shader() {};
	~Shader() {};
	void Load();

};

class ResourceManager
{
private:
	static ResourceManager* uniqueRManager;
	ResourceManager();
	unordered_map<int, Model> models;
	unordered_map<int, Texture> textures;
	unordered_map<int, Shader> shaders;

public:
	void Init();
	static ResourceManager* getInstance();
	~ResourceManager();



};
