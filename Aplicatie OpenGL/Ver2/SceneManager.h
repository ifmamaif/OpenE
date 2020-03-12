#pragma once
//#include <unordered_map>
#include <map>
#include "ResourceManager.h"
#include "SceneObject.h"
#include "../Utilities/utilities.h"
#include "Camera.h"

object_type load_object_type(char * const &type);

class SceneManager
{	
private:
	static SceneManager* uniqueSManager;

	Vector3 background_color;
	map<int, Camera> cameras;
	vector<SceneObject *> objects;
	int active_cameraId;
	Camera* active_cameraPtr;

	SceneManager();

	void InitBackgroundColor(xml_node<> * const &pRoot);
	void InitCameras(xml_node<> * const &pRoot);
	void InitSceneObjects(xml_node<> * const &pRoot);

	void LoadCommonAttributes(xml_node<> * const &Objects,
		GLuint &_Id, object_type &_type, char * &_name,
		Vector3 &_position, Vector3 &_rotation, Vector3 &_scale,
		int &_id_model,
		int &_nr_textures, int * &_vec_id_texture,
		int &_id_shader,
		bool &_blend, bool &_useDepthTest);
	void LoadTerrainAttributes(xml_node<> * const &Objects,
		int &_nr_cells, float &_size_cell, float &_offset_y,
		Vector3 &_height);
public:
	static SceneManager* getInstance();

	void Init();
	void Draw();
	void Update();

	Camera& getActiveCamera() { return *active_cameraPtr; }
	void changeActiveCamera(int Id) { active_cameraId = Id; active_cameraPtr = &cameras[Id]; }
	~SceneManager() {
		for (auto &x : objects)delete x;
		uniqueSManager = NULL;
	}
};

