#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "rapidxml.hpp"
#include "fmod.hpp"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Sound.h"
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
	FMOD::System *fmodSystem;

	unordered_map<int, Model> models;
	unordered_map<int, Texture> textures;
	unordered_map<int, Shader> shaders;
	unordered_map<int, FMOD::Sound*> sounds;

	unordered_map<int, ModelResource> modelResources;
	unordered_map<int, TextureResource> textureResources;
	unordered_map<int, ShaderResource> shaderResources;
	unordered_map<int, SoundResource> soundResources;

	void InitModelResources(xml_node<> * const &pRoot);
	void InitShaderResources(xml_node<> * const &pRoot);
	void InitTextureResources(xml_node<> * const &pRoot);
	void InitSoundResources(xml_node<> * const &pRoot);
public:
	void Init();
	void SendCommonConstantUniforms(float * const & fog, float const & r, float const & R, float * const & ambiental_light_color, float const & ratio);
	void SendCommonUniforms(float * const &cameraPos);
	static ResourceManager* getInstance();
	Model* reqModel(int const &id_model);
	Model* reqTerrainModel(int const &nr_cells, float const &size_cell, float const &offset_y);
	Texture* reqTexture(int const &id_texture);
	Shader* reqShader(int const &id_shader);
	FMOD::Sound** reqSound(int const &id_sound);
	void playSound(SoundReference const &id_sound);
	Shader* reqDebugShader();
	Shader* reqGrayScaleShader();
	Shader* reqBlurShader();
	Shader* reqBloomShader();
	Shader* reqDepthShader();
	Shader* reqSimpleTextureShader();
	Shader* reqLightDepthShader();
	~ResourceManager() { uniqueRManager = NULL; 
	for (auto &x : sounds)
		x.second->release();
	};
};
