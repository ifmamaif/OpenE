#pragma once
#include <map>
#include "ResourceManager.h"
#include "../Utilities/utilities.h"
#include "Camera.h"

enum class object_type : int
{
	undefined = -1,
	normal = 1,
	terrain,
	fire, 
	skybox,
	reflectable
};

enum class trajectory_type : int
{
	undefined = -1,
	line_strip = 1,
	circle
};

enum class direction : int
{
	undefined = -1,
	normal = 1,
	alternate
};

enum class light_type : int
{
	undefined = -1,
	ambient = 1,
	directional,
	point, 
	spotlight
};
class Trajectory
{
	trajectory_type type;
	direction dir;
	float speed;
	int iteration_count;
	vector<Vector3> *points;
	Vector3 center, rotX, rotZ;
	float radius;

	float total_length;
	//friend class SceneObject;
public:
	Trajectory(trajectory_type const &_type,direction const &_dir, float const &_speed, int const &_iteration_count,
		vector<Vector3> * const &_points, Vector3 const &_center, Vector3 const &_rotX, Vector3 const &_rotZ,
		float const &_radius);
	~Trajectory() { 
		delete points; 
	}
	Matrix trajectoryMatrix(float const &time);
};

class LightObject
{
	GLuint Id;
	light_type type;
	Vector3 position;
	Vector3 direction;
	Vector3 light_color;
	static float const dir[3];
	static float const point[3];
	static float const spot[3];
	static float const none[3];
	GLfloat specular_power, alpha;
	

public :
	bool isPointLight() { return type == light_type::point; }
	Camera *lightPOV[6];
	FrameBuffer lightDepthFrameBuffer;
	void updateTarget(Camera * const & cameraPtr);
	void SendTo(Shader* const &shaderPtr, int const &i);
	LightObject() { std::cout << "Constructor implicit apelat -LightObject:Undefined Behaviour"; }
	LightObject(GLuint const &_Id, light_type const &_type,
		Vector3 const &_position, Vector3 const &_direction, Vector3 const &_light_color,
		GLfloat const &_specular_power, GLfloat const &_alpha);
	static void SendNone(Shader * const &shaderPtr, int const &i);
	~LightObject() 
	{
		if (type != light_type::point)
			delete lightPOV[0];
		else
		{
			for (int i = 0; i < 6; i++)
				delete lightPOV[i];
		}
		//delete[] lightPOV;
	}
};

class SceneObject
{
protected:
	static map<int, LightObject> *lights;

	GLuint Id;
	object_type type;
	
	char* name;

	Vector3 position;
	Vector3 rotation; //specify angle in radians
	Vector3 scale;

	GLfloat kDiffuse, kSpecular;

	Matrix internalModelMatrix;
	Matrix externalModelMatrix;
	Matrix normalMatrix;

	int id_model;
	Model* modelPtr;

	int nr_textures;
	int *vec_id_texture;
	Texture** texturePtrs;

	int id_shader;
	Shader* shaderPtr;

	bool blend;
	bool useDepthTest;

	Trajectory* trajectoryPtr;
	float* extremePoints;
	Vertex* AABB;
	GLushort* indices;

	Vector3 wiredColor;
	Vector3 AABBColor;

	int nr_lights;
	int *vec_id_light;
	LightObject** lightPtrs;

	SceneObject() {}
	void Init(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest, 
		float const &_kDiffuse, float const &_kSpecular, 
		int const &_nr_lights, int * const &_vec_id_light,
		Trajectory * const &_trajectory,
		Vector3 const &_wiredColor, Vector3 const &_AABBColor);
	 virtual void defineInternalModelMatrix();
	 virtual void defineExternalModelMatrix();
	 virtual void LoadModel(ResourceManager * const &ReqResources);
	virtual void  LoadTextures(ResourceManager * const &ReqResources);
	virtual void LoadShader(ResourceManager * const &ReqResources);
	void LoadLights();
	void Load();
	virtual void PrepareAllData(float const & deltaTime, Camera * const &eye) {};

	void BindFull(Camera * const &eye);
	void BindLightDepth(Camera * const &eye, Shader * const &shader);
	void BindWired(Camera * const &eye, Shader * const &debugShader);
	virtual void SendDataToShader() {};
	void Move();
public:
	static void SetLights(static map<int, LightObject> *_lights) { lights = _lights; }
	SceneObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3 const &_rotation, Vector3 const &_scale,
		int const &_id_model, 
		int const &_nr_textures, int * const &_vec_id_texture, 
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest, 
		float const &_kDiffuse, float const &_kSpecular,
		int const &_nr_lights, int * const &_vec_id_light,
		Trajectory * const &_trajectory,
		Vector3 const &_wiredColor, Vector3 const &_AABBColor);
	SceneObject(SceneObject const &) = delete;
	
	void DrawFull(Camera * const &eye);
	void DrawLightDepth(Camera * const &eye, Shader * const &shader);
	void DrawWired(Camera * const &eye, Shader * const &debugShader);
	bool Colliding(SceneObject* other);
	void Update(float const & deltaTime, Camera * const &eye);
	virtual void Key(unsigned char const &key, Camera * const &eye) {};
	virtual ~SceneObject() {
			delete[] name;
			delete[] vec_id_texture;
			delete[] texturePtrs;
			delete[] vec_id_light;
			delete[] lightPtrs;
			delete trajectoryPtr;
			delete[] extremePoints;
			delete[] AABB;
			delete[] indices;
	}
};

class TerrainObject : public SceneObject
{
	 void defineInternalModelMatrix();
	 void defineExternalModelMatrix();
	 void LoadModel(ResourceManager * const &ReqResources);
	int nr_cells;
	float size_cell, offset_y;
	Vector3 height;
	 Matrix CenterTerrainOnCamera;
	 GLfloat u, v, moveTexture[2];

	 void SendDataToShader();
public:
	TerrainObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest, 
		float const &_kDiffuse, float const &_kSpecular, 
		int const &_nr_lights, int * const &_vec_id_light,
		Trajectory * const &_trajectory,
		Vector3 const &_wiredColor, Vector3 const &_AABBColor,
		int const &_nr_cells, float const &_size_cell, float const &_offset_y,
		Vector3 const &_height);
	void Key(unsigned char const &key, Camera * const &eye);
	~TerrainObject() { delete modelPtr; 
	}

};

class FireObject : public SceneObject
{
	float u_DispMax;
	float u_Time;
	void PrepareAllData(float const & deltaTime, Camera * const &eye);
	void SendDataToShader();
public:
	FireObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest, 
		float const &_kDiffuse, float const &_kSpecular, 
		int const &_nr_lights, int * const &_vec_id_light,
		Trajectory * const &_trajectory,
		Vector3 const &_wiredColor, Vector3 const &_AABBColor,
		float const &_u_DispMax);

};

class SkyBoxObject : public SceneObject
{
	void defineExternalModelMatrix();
	float  offset_y;
	Matrix CenterSkyBoxOnCamera;

public:
	SkyBoxObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest,
		float const &_kDiffuse, float const &_kSpecular,
		int const &_nr_lights, int * const &_vec_id_light,
		Trajectory * const &_trajectory,
		Vector3 const &_wiredColor, Vector3 const &_AABBColor,
	    float const &_offset_y);
	void Key(unsigned char const &key, Camera * const &eye);
};

class ReflectableObject : public SceneObject
{
	float reflectiveness;
	void SendDataToShader();
public:
	ReflectableObject(
		GLuint const &_Id, object_type const &_type, char * const &_name,
		Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
		int const &_id_model,
		int const &_nr_textures, int * const &_vec_id_texture,
		int const &_id_shader,
		bool const &_blend, bool const &_useDepthTest,
		float const &_kDiffuse, float const &_kSpecular,
		int const &_nr_lights, int * const &_vec_id_light,
		Trajectory * const &_trajectory,
		Vector3 const &_wiredColor, Vector3 const &_AABBColor,
		float const &_reflectiveness);

};