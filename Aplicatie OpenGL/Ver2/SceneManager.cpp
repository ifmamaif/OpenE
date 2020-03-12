#include "stdafx.h"
#include "SceneManager.h"

object_type load_object_type(char * const &type)
{
	if (type[0] == 'n')
		return object_type::normal;
	if (type[0] == 't')
		return object_type::terrain;
	if (type[0] == 'f')
		return object_type::fire;
	return object_type::undefined;
}

SceneManager* SceneManager::uniqueSManager = NULL;

SceneManager::SceneManager() {}

SceneManager* SceneManager::getInstance()
{
	if (uniqueSManager == NULL)
		uniqueSManager = new SceneManager;
	return uniqueSManager;
}



void SceneManager::InitBackgroundColor(xml_node<>* const &pRoot)
{
	xml_node<> *RootBackgroundColor = pRoot->first_node("backgroundColor");
	xml_node<> *color = RootBackgroundColor->first_node();
	background_color.r = atof(color->value());
	color = color->next_sibling();
	background_color.g = atof(color->value());;
	color = color->next_sibling();
	background_color.b = atof(color->value());
}

void SceneManager::InitCameras(xml_node<>* const & pRoot)
{
	xml_node<> *RootCameras = pRoot->first_node("cameras"), *cam_node;
	int id;
	for (xml_node<> *Cameras = RootCameras->first_node(); Cameras; Cameras = Cameras->next_sibling())
	{
		id = atoi(Cameras->first_attribute("id")->value());
		cam_node = Cameras->first_node("position");
		Vector3 _pos(atof(cam_node->first_node("x")->value()), atof(cam_node->first_node("y")->value()), atof(cam_node->first_node("z")->value()));
		cam_node = Cameras->first_node("target");
		Vector3 _targ(atof(cam_node->first_node("x")->value()), atof(cam_node->first_node("y")->value()), atof(cam_node->first_node("z")->value()));
		cam_node = Cameras->first_node("up");
		Vector3 _up(atof(cam_node->first_node("x")->value()), atof(cam_node->first_node("y")->value()), atof(cam_node->first_node("z")->value()));
		cam_node = Cameras->first_node("translationSpeed");
		GLfloat _moveSpeed = atof(cam_node->value());
		cam_node = Cameras->first_node("rotationSpeed");
		GLfloat _rotateSpeed = atof(cam_node->value());
		cam_node = Cameras->first_node("fov");
		GLfloat _fov = atof(cam_node->value());
		cam_node = Cameras->first_node("near");
		GLfloat _near = atof(cam_node->value());
		cam_node = Cameras->first_node("far");
		GLfloat _far = atof(cam_node->value());
		cameras.try_emplace(id, _pos, _targ, _up, _moveSpeed, _rotateSpeed, _near, _far, _fov);
	}
	active_cameraId = atoi(pRoot->first_node("activeCamera")->value());
	changeActiveCamera(active_cameraId);
}

void SceneManager::InitSceneObjects(xml_node<>* const & pRoot)
{
	xml_node<> *RootObjects = pRoot->first_node("objects");
	//CommonAttributes
	GLuint _Id; 
	object_type _type;
	char * _name;
	Vector3 _position, _rotation, _scale;
	int _id_model, 
		_nr_textures, *_vec_id_texture,
		_id_shader;
	bool _blend, _useDepthTest;
	//Terrain Attributes
	int _nr_cells; 
	float _size_cell, _offset_y;
	Vector3 _height;
	//Fire Attributes

	//
	for (xml_node<> *Objects = RootObjects->first_node(); Objects; Objects = Objects->next_sibling())
	{
		LoadCommonAttributes(Objects, _Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest);
		switch (_type)
		{
		case object_type::normal:
			objects.emplace_back(new SceneObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest));
			break;
		case object_type::terrain:
			LoadTerrainAttributes(Objects, _nr_cells, _size_cell, _offset_y, _height);
			objects.emplace_back(new TerrainObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _nr_cells, _size_cell, _offset_y, _height));
			break;
		case object_type::fire:
			//LoadFireAttributes(Objects, )
			break;

		default:
			cout << "Obiect necunoscut";
		}
			
	}
}

void SceneManager::LoadCommonAttributes(xml_node<> * const &Objects,
	GLuint &_Id, object_type &_type, char * &_name,
	Vector3 &_position, Vector3 &_rotation, Vector3 &_scale,
	int &_id_model,
	int &_nr_textures, int * &_vec_id_texture,
	int &_id_shader,
	bool &_blend, bool &_useDepthTest)
{
	//
	xml_node<> *obj_node;
	//
	_Id = atoi(Objects->first_attribute("id")->value());
	//
	obj_node = Objects->first_node("position");
	_position.x = atof(obj_node->first_node("x")->value());
	_position.y = atof(obj_node->first_node("y")->value());
	_position.z = atof(obj_node->first_node("z")->value());
	//
	obj_node = Objects->first_node("rotation");
	_rotation.x = atof(obj_node->first_node("x")->value());
	_rotation.y = atof(obj_node->first_node("y")->value());
	_rotation.z = atof(obj_node->first_node("z")->value());
	//
	obj_node = Objects->first_node("scale");
	_scale.x = atof(obj_node->first_node("x")->value());
	_scale.y = atof(obj_node->first_node("y")->value());
	_scale.z = atof(obj_node->first_node("z")->value());
	//	
	obj_node = Objects->first_node("name");
	_name = new char[obj_node->value_size() + 1]; 
	strcpy(_name, obj_node->value());
	//
	obj_node = Objects->first_node("model");
	char *string_id_model = obj_node->value();
	_id_model = isdigit(string_id_model[0]) ? atoi(string_id_model) : -1;
	//
	obj_node = Objects->first_node("shader");
	_id_shader = atoi(obj_node->value());
	//
	obj_node = Objects->first_node("blend");
	_blend = obj_node->value()[0] == 't' ? true : false;
	//
	obj_node = Objects->first_node("type");
	_type = load_object_type(obj_node->value());
	//
	obj_node = Objects->first_node("textures");
	xml_node<> *textures;
	_nr_textures = 0;
	for (textures = obj_node->first_node(); textures; textures = textures->next_sibling(), _nr_textures++);
	_vec_id_texture = new int[_nr_textures];
	int i = 0;
	for (textures = obj_node->first_node(); textures; textures = textures->next_sibling(), i++)
		_vec_id_texture[i] = atoi(textures->first_attribute("id")->value());
	//

	_useDepthTest = true;
}

void SceneManager::LoadTerrainAttributes(xml_node<>* const &Objects, int &_nr_cells, float &_size_cell, float &_offset_y, Vector3 &_height)
{
	//
	xml_node<> *obj_node;
	//
	_nr_cells = 50;
	_size_cell = 5;
	_offset_y = 0.3;
	//
	obj_node = Objects->first_node("height");
	_height.x = atof(obj_node->first_node("x")->value());
	_height.y = atof(obj_node->first_node("y")->value());
	_height.z = atof(obj_node->first_node("z")->value());

}

void SceneManager::Init()
{
	
	glEnable(GL_DEPTH_TEST);
	xml_document<> doc;
	ifstream file("sceneManager.xml");
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);
	xml_node<> *pRoot = doc.first_node();

	InitBackgroundColor(pRoot);
	InitCameras(pRoot);
	
	InitSceneObjects(pRoot);
	
	content.clear();
	doc.clear();

	return;
}

void SceneManager::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(background_color.r, background_color.g, background_color.b, 0.0f);
	for (auto &x : objects)
		x->Draw(active_cameraPtr);
	
}
