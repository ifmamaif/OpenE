#include "stdafx.h"
#include "ResourceManager.h"



GLenum load_texture_type(char * const &type)
{
	if (type[0] == '2')return GL_TEXTURE_2D;
	return -1;
}

GLenum load_texture_filter(char * const &filter)
{
	if (filter[0] == 'L')return GL_LINEAR;
	return -1;
}

GLenum load_texture_wrap(char * const &wrap)
{
	if (wrap[0] == 'C')return GL_CLAMP_TO_EDGE;
	if (wrap[0] == 'R')return GL_REPEAT;
	if (wrap[0] == 'M')return GL_MIRRORED_REPEAT;
	return -1;
}

ResourceManager* ResourceManager::uniqueRManager = NULL;

ResourceManager* ResourceManager::getInstance()
{
	if (uniqueRManager == NULL)
		uniqueRManager = new ResourceManager();
	return uniqueRManager;
}

Model * ResourceManager::reqModel(int const & id_model)
{
	//unordered_map<int, Model>::iterator 
	auto &models_iterator = models.find(id_model);
	if (models_iterator != models.end())
		return &models_iterator->second;
	auto _modelPtr = &models.try_emplace(id_model, &modelResources[id_model]
	).first->second;
	_modelPtr->Load();
	return _modelPtr;
}

Model * ResourceManager::reqTerrainModel(int const & nr_cells, float const & size_cell, float const & offset_y)
{
	Model* modelPtr = new TerrainModel(nr_cells, size_cell, offset_y);
	modelPtr->Load();
	return modelPtr;
}

Texture * ResourceManager::reqTexture(int const & id_texture)
{
	//unordered_map<int, Texture>::iterator 
	auto &textures_iterator = textures.find(id_texture);
	if (textures_iterator != textures.end())
		return &textures_iterator->second;
	auto _TexturePtr = &textures.try_emplace(id_texture, &textureResources[id_texture]
	).first->second;
	_TexturePtr->Load();
	return _TexturePtr;
}

Shader * ResourceManager::reqShader(int const & id_shader)
{
	//unordered_map<int, Shader>::iterator 
	auto &shaders_iterator = shaders.find(id_shader);
	if (shaders_iterator != shaders.end())
		return &shaders_iterator->second;
	auto shaderPtr = &shaders.try_emplace(id_shader, &shaderResources[id_shader]
	).first->second;
	shaderPtr->Load();
	return shaderPtr;
}

void ResourceManager::InitModelResources(xml_node<>* const & pRoot)
{
	xml_node<> *RootModels = pRoot->first_node("models");
	xml_node<> *path; int id, size;
	for (xml_node<> *Models = RootModels->first_node();
		Models; Models = Models->next_sibling())
	{
		id = atoi(Models->first_attribute("id")->value());
		path = Models->first_node("path");
		size = path->value_size();
		char* modelRes_path = new char[size + 1];
		strcpy(modelRes_path, path->value());
		modelResources.try_emplace(id, modelRes_path);
	}
}

void ResourceManager::InitShaderResources(xml_node<>* const & pRoot)
{
	xml_node<> *RootShaders = pRoot->first_node("shaders");
	xml_node<> *vs, *fs; int id;
	for (xml_node<> *Shaders = RootShaders->first_node(); Shaders; Shaders = Shaders->next_sibling())
	{
		id = atoi(Shaders->first_attribute("id")->value());
		vs = Shaders->first_node("vs");
		fs = Shaders->first_node("fs");
		char* _vertexShader = new char[vs->value_size() + 1];
		char* _fragmentShader = new char[fs->value_size() + 1];
		strcpy(_vertexShader, vs->value());
		strcpy(_fragmentShader, fs->value());
		shaderResources.try_emplace(id, _vertexShader, _fragmentShader);
	}
}

void ResourceManager::InitTextureResources(xml_node<>* const & pRoot)
{
	xml_node<> *RootTextures = pRoot->first_node("textures");
	xml_node<> *path, *min_filter, *mag_filter, *wrap_s, *wrap_t; int id;
	for (xml_node<> *Textures = RootTextures->first_node(); Textures; Textures = Textures->next_sibling())
	{
		id = atoi(Textures->first_attribute("id")->value());
		path = Textures->first_node("path");
		min_filter = Textures->first_node("min_filter");
		mag_filter = Textures->first_node("mag_filter");
		wrap_s = Textures->first_node("wrap_s");
		wrap_t = Textures->first_node("wrap_t");

		GLenum _type = load_texture_type(Textures->first_attribute("type")->value());
		char* _path = new char[path->value_size() + 1];
		strcpy(_path, path->value());
		GLenum _min_filter = load_texture_filter(min_filter->value());
		GLenum _mag_filter = load_texture_filter(mag_filter->value());
		GLenum _wrap_s = load_texture_wrap(wrap_s->value());
		GLenum _wrap_t = load_texture_wrap(wrap_t->value());
		textureResources.try_emplace(id, _type, _path, _min_filter, _mag_filter, _wrap_s, _wrap_t);
	}
}

void ResourceManager::Init()
{
	xml_document<> doc;
	ifstream file("resourceManager.xml");
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);
	xml_node<> *pRoot = doc.first_node();
	InitModelResources(pRoot);
	InitShaderResources(pRoot);
	InitTextureResources(pRoot);
	content.clear();
	doc.clear();
	return;
}