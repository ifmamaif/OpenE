#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "rapidxml.hpp"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "../Utilities/utilities.h"
using namespace rapidxml;
using namespace std;

GLenum load_texture_type(char * const &type);
GLenum load_texture_filter(char * const &filter);
GLenum load_texture_wrap(char * const & wrap);

class ResourceManager
{
private:
	static ResourceManager* uniqueRManager;
	ResourceManager() {};
	unordered_map<int, Model> models;
	unordered_map<int, Texture> textures;
	unordered_map<int, Shader> shaders;
	unordered_map<int, ModelResource> modelResources;
	unordered_map<int, TextureResource> textureResources;
	unordered_map<int, ShaderResource> shaderResources;
	void InitModelResources(xml_node<> * const &pRoot);
	void InitShaderResources(xml_node<> * const &pRoot);
	void InitTextureResources(xml_node<> * const &pRoot);
public:
	void Init();
	static ResourceManager* getInstance();
	Model* reqModel(int const &id_model);
	Model* reqTerrainModel(int const &nr_cells, float const &size_cell, float const &offset_y);
	Texture* reqTexture(int const &id_texture);
	Shader* reqShader(int const &id_shader);
	~ResourceManager() { uniqueRManager = NULL; };
};
