#pragma once
#include <unordered_map>
#include "ResourceManager.h"
#include "../Utilities/utilities.h"

class SceneObject
{
	GLuint Id;
	Vector3 position;
	Vector3 rotation; //specify angle in radians
	Vector3 scale;
	Model* model;
	Texture** textures;
	Shader* shader;
	bool useDepthTest;
public:

};

class SceneManager
{
	
	static SceneManager* uniqueSManager;
	SceneManager();
	
public:
	void Init();
	static SceneManager* getInstance();
};

