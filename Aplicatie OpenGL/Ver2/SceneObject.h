#pragma once
#include "ResourceManager.h"
#include "../Utilities/utilities.h"
#include "Camera.h"

enum class object_type : int
{
	undefined = -1,
	normal = 1,
	terrain,
	fire
};

class SceneObject
{
protected:
	GLuint Id;
	object_type type;
	
	char* name;

	Vector3 position;
	Vector3 rotation; //specify angle in radians
	Vector3 scale;

	Matrix modelMatrix;

	int id_model;
	Model* modelPtr;

	int nr_textures;
	int *vec_id_texture;
	Texture** texturePtrs;

	int id_shader;
	Shader* shaderPtr;

	bool blend;
	bool useDepthTest;

	SceneObject() {}
	inline void Init(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest);
	inline virtual void defineModelMatrix();
	inline virtual void LoadModel(ResourceManager * const &ReqResources);
	virtual void inline LoadTextures(ResourceManager * const &ReqResources);
	virtual inline void LoadShader(ResourceManager * const &ReqResources);
	inline void Load();
	void Bind();
public:
	SceneObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3 const &_rotation, Vector3 const &_scale,
		int const &_id_model, 
		int const &_nr_textures, int * const &_vec_id_texture, 
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest);
	SceneObject(SceneObject const &) = delete;
	
	virtual void Draw(Camera * const &eye);
	virtual void Update() {};
	virtual ~SceneObject() {
			delete[] name;
			delete[] vec_id_texture;
			delete[] texturePtrs;
	}
};

class TerrainObject : public SceneObject
{
	inline void defineModelMatrix();
	inline void LoadModel(ResourceManager * const &ReqResources);
	int nr_cells;
	float size_cell, offset_y;
	Vector3 height;

public:
	TerrainObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest,
		int const &_nr_cells, float const &_size_cell, float const &_offset_y, 
		Vector3 const &_height);
	void Draw(Camera * const &eye);
	~TerrainObject() { delete modelPtr; 
	//((SceneObject*)this)->~SceneObject();
	//delete[] name;
	//delete[] vec_id_texture;
	//delete[] texturePtrs;
	}

};

class FireObject : public SceneObject
{
	inline void defineModelMatrix() {};
	inline void LoadModel(ResourceManager * const &ReqResources) {};

public:
	FireObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest) {};
	void Draw(Camera * const &eye) {};

};