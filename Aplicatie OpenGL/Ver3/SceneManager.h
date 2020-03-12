#pragma once
//#include <unordered_map>
#include <map>
#include "ResourceManager.h"
#include "SceneObject.h"
#include "../Utilities/utilities.h"
#include "Camera.h"

object_type load_object_type(char * const &type);
trajectory_type load_trajectory_type(char * const &type);
direction load_direction_type(char * const &type);
light_type load_light_type(char * const &type);
int load_iteration_count(char * const &count);

class SceneManager
{	
private:

	static SceneManager* uniqueSManager;

	Vertex quadVertices[4];
	GLushort quadIndices[6];
	GLuint quadVbo, quadIbo;

	draw_type draw_state;

	Vector3 background_color, wiredColor, AABBColor;

	Vector3 ambiental_light_color;
	float ratio;

	Vector3 fog;
	float r, R;

	map<int, Camera> cameras;
	map<int, LightObject> lights;
	vector<SceneObject *> objects;
	int active_cameraId;
	Camera* active_cameraPtr;

	SceneManager();

	void InitBackgroundColor(xml_node<> * const &pRoot);
	void InitFog(xml_node<> * const &pRoot);
	void InitDebugColor(xml_node<> * const &Root);
	void InitCameras(xml_node<> * const &pRoot);
	void InitLightObjects(xml_node<> * const &pRoot);
	void InitSceneObjects(xml_node<> * const &pRoot);
	
	void LoadFloat(xml_node<> * const &float_node, float &val);//includes attribute
	void LoadRGB(xml_node<> * const &xyz_root_node, Vector3 &point);//xyz
	void LoadCommonAttributes(xml_node<> * const &Objects,
		GLuint &_Id, object_type &_type, char * &_name,
		Vector3 &_position, Vector3 &_rotation, Vector3 &_scale,
		int &_id_model,
		int &_nr_textures, int * &_vec_id_texture,
		int &_id_shader,
		bool &_blend, bool &_useDepthTest, float &_kDiffuse, float &_kSpecular,
		int &_nr_lights, int * &_vec_id_light,
		Trajectory *&_trajectory,
		Vector3 &_wiredColor, Vector3 &_AABBColor);
	void LoadTrajectoryAttributes(xml_node<> * const &obj_node, Trajectory *&_trajectory);
	void LoadTerrainAttributes(xml_node<> * const &Objects,
		int &_nr_cells, float &_size_cell, float &_offset_y,
		Vector3 &_height);
	void LoadFireAttributes(xml_node<> * const &Objects,
		float &_u_DispMax);
	void LoadSkyBoxAttributes(xml_node<> * const &Objects,
		float &_offset_y);
	void LoadReflectableAttributes(xml_node<> * const &Objects,
		float &_reflectiveness);
	void SendCommonConstantUniforms();
	void SendCommonUniforms();
public:
	static SceneManager* getInstance();

	void Init();
	void Draw();
	void Update(float const &deltaTime);
	void Key(unsigned char const &key);

	 Camera& getActiveCamera() { return *active_cameraPtr; }
	 void changeActiveCamera(int Id) { active_cameraId = Id; active_cameraPtr = &cameras[Id]; }
	~SceneManager() {
		for (auto &x : objects)delete x;
		glDeleteBuffers(1, &quadVbo);
		glDeleteBuffers(1, &quadIbo);
		uniqueSManager = NULL;
	}
};

