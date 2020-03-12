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
	if (type[0] == 's')
		return object_type::skybox;
	if (type[0] == 'r')
		return object_type::reflectable;
	return object_type::undefined;
}

trajectory_type load_trajectory_type(char * const & type)
{
	if (type[0] == 'l')
		return trajectory_type::line_strip;
	if (type[0] == 'c')
		return trajectory_type::circle;
	return trajectory_type::undefined;
}

direction load_direction_type(char * const & type)
{
	if (type[0] == 'n')
		return direction::normal;
	if (type[0] == 'a')
		return direction::alternate;
	return direction::undefined;
}

light_type load_light_type(char * const & type)
{
	if (type[0] == 'a')
		return light_type::ambient;
	if (type[0] == 'p')
		return light_type::point;
	if (type[0] == 's')
		return light_type::spotlight;
	if (type[0] == 'd')
		return light_type::directional;
	return light_type::undefined;
}

int load_iteration_count(char * const & count)
{
	if (count[0] == 'i')
		return -1;
	return atoi(count);
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
	LoadRGB(pRoot->first_node("backgroundColor"), background_color);
}

void SceneManager::InitFog(xml_node<>* const & pRoot)
{
	xml_node<> *RootFog = pRoot->first_node("fog");
	LoadRGB(RootFog->first_node("color"), fog);
	LoadFloat(RootFog->first_node("r"), r);
	LoadFloat(RootFog->first_node("R"), R);
}

void SceneManager::InitDebugColor(xml_node<>* const & Root)
{
	LoadRGB(Root->first_node("wiredColor"), wiredColor);
	LoadRGB(Root->first_node("AABBColor"), AABBColor);
}

void SceneManager::InitCameras(xml_node<>* const & pRoot)
{
	//CameraAttributes
	Vector3 _pos, _targ, _up;
	GLfloat _moveSpeed, _rotateSpeed, _fov, _near, _far;
	xml_node<> *RootCameras = pRoot->first_node("cameras");
	int id;
	for (xml_node<> *Cameras = RootCameras->first_node(); Cameras; Cameras = Cameras->next_sibling())
	{
		id = atoi(Cameras->first_attribute("id")->value());
		LoadRGB(Cameras->first_node("position"), _pos);
		LoadRGB(Cameras->first_node("target"), _targ);
		LoadRGB(Cameras->first_node("up"), _up);
		LoadFloat(Cameras->first_node("translationSpeed"), _moveSpeed);
		LoadFloat(Cameras->first_node("rotationSpeed"), _rotateSpeed);
		LoadFloat(Cameras->first_node("fov"), _fov);
		LoadFloat(Cameras->first_node("near"), _near);
		LoadFloat(Cameras->first_node("far"), _far);
		cameras.try_emplace(id, _pos, _targ, _up, _moveSpeed, _rotateSpeed, _near, _far, _fov);
	}
	active_cameraId = atoi(pRoot->first_node("activeCamera")->value());
	changeActiveCamera(active_cameraId);
}

void SceneManager::InitLightObjects(xml_node<>* const & pRoot)
{
	xml_node<> *RootAmbientalLight = pRoot->first_node("ambientalLight");
	LoadRGB(RootAmbientalLight->first_node("color"), ambiental_light_color);
	LoadFloat(RootAmbientalLight->first_node("ratio"), ratio);

	xml_node<> *RootLights = pRoot->first_node("lights");
	int id;
	light_type _type;
	Vector3 _position, _direction, _light_color;
	GLfloat _specular_power, _alpha;
	for (xml_node<> *Lights = RootLights->first_node(); Lights; Lights = Lights->next_sibling())
	{
		id = atoi(Lights->first_attribute("id")->value());
		_type = load_light_type(Lights->first_node("type")->value());
		LoadRGB(Lights->first_node("position"), _position);
		LoadRGB(Lights->first_node("direction"), _direction);
		LoadRGB(Lights->first_node("light_color"), _light_color);
		LoadFloat(Lights->first_node("alpha"), _alpha);		
		LoadFloat(Lights->first_node("specular_power"), _specular_power);
		lights.try_emplace(id, id, _type, _position, _direction, _light_color, _specular_power, _alpha);
	}
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
	int _nr_lights, *_vec_id_light;
	bool _blend, _useDepthTest;
	float _kDiffuse, _kSpecular;
	Vector3 _wiredColor, _AABBColor;
	//Terrain Attributes
	int _nr_cells; 
	float _size_cell, _offset_y;
	Vector3 _height;
	//Fire Attributes
	float _u_DispMax;
	//SkyBox Attributes
	// float _offset_y; already declared
	// it has the same meaning, but for different object types
	//Reflectable attributes
	float _reflectiveness;
	//Trajectory
	Trajectory *_trajectory = nullptr;
	for (xml_node<> *Objects = RootObjects->first_node(); Objects; Objects = Objects->next_sibling())
	{
		LoadCommonAttributes(Objects, _Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor);
		switch (_type)
		{
		case object_type::normal:
			objects.emplace_back(new SceneObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor));
			break;
		case object_type::terrain:
			LoadTerrainAttributes(Objects, _nr_cells, _size_cell, _offset_y, _height);
			objects.emplace_back(new TerrainObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor, _nr_cells, _size_cell, _offset_y, _height));
			break;
		case object_type::fire:
			LoadFireAttributes(Objects, _u_DispMax);
			objects.emplace_back(new FireObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor, _u_DispMax));
			break;
		case object_type::skybox:
			LoadSkyBoxAttributes(Objects, _offset_y);
			objects.emplace_back(new SkyBoxObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor, _offset_y));
			break;
		case object_type::reflectable:
			LoadReflectableAttributes(Objects, _reflectiveness);
			objects.emplace_back(new ReflectableObject(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor, _reflectiveness));
			break;
		default:
			cout << "Obiect necunoscut";
		}
			
	}
}


void SceneManager::LoadFloat(xml_node<>* const & float_node, float & val)
{
	if (float_node != NULL)
		val = atof(float_node->value());
	else val = 0;
}

void SceneManager::LoadRGB(xml_node<>* const & xyz_root_node, Vector3 & point)
{
	if (xyz_root_node == NULL)throw "Nu s-a gasit xyz_root";
	xml_node<> *color = xyz_root_node->first_node();
	LoadFloat(color, point.x);
	color = color->next_sibling();
	LoadFloat(color, point.y);
	color = color->next_sibling();
	LoadFloat(color, point.z);
}

void SceneManager::LoadCommonAttributes(xml_node<> * const &Objects,
	GLuint &_Id, object_type &_type, char * &_name,
	Vector3 &_position, Vector3 &_rotation, Vector3 &_scale,
	int &_id_model,
	int &_nr_textures, int * &_vec_id_texture,
	int &_id_shader,
	bool &_blend, bool &_useDepthTest, float &_kDiffuse, float &_kSpecular, 
	int &_nr_lights, int * &_vec_id_light,
	Trajectory *&_trajectory,
	Vector3 & _wiredColor, Vector3 & _AABBColor)
{
	//
	xml_node<> *obj_node;
	//
	_Id = atoi(Objects->first_attribute("id")->value());
	LoadRGB(Objects->first_node("position"), _position);
	LoadRGB(Objects->first_node("rotation"), _rotation);
	LoadRGB(Objects->first_node("scale"), _scale);
	LoadFloat(Objects->first_node("kDiffuse"), _kDiffuse);
	LoadFloat(Objects->first_node("kSpecular"), _kSpecular);
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
	obj_node = Objects->first_node("lights");
	xml_node<> *lights;
	_nr_lights = 0;
	for (lights = obj_node->first_node(); lights; lights = lights->next_sibling(), _nr_lights++);
	_vec_id_light = new int[_nr_lights];
	i = 0;
	for (lights = obj_node->first_node(); lights; lights = lights->next_sibling(), i++)
		_vec_id_light[i] = atoi(lights->value());
	//
	_useDepthTest = true;

	obj_node = Objects->first_node("trajectory");
	LoadTrajectoryAttributes(obj_node, _trajectory);

	obj_node = Objects->first_node("wiredColor");
	if (obj_node != NULL)
		LoadRGB(obj_node, _wiredColor);
	else _wiredColor = wiredColor;

	obj_node = Objects->first_node("AABBColor");
	if (obj_node != NULL)
		LoadRGB(obj_node, _AABBColor);
	else _AABBColor = AABBColor;
	
}


void SceneManager::LoadTrajectoryAttributes(xml_node<>* const & obj_node, Trajectory *& _trajectory)
{
	trajectory_type _type = trajectory_type::undefined;
	direction _dir;
	float _speed = 0;
	int _iteration_count = 0;
	vector<Vector3> *_points = new vector<Vector3>;
	Vector3 _center, _rotX, _rotZ;
	float _radius = 0;
	if (obj_node != NULL)
	{
		_type = load_trajectory_type(obj_node->first_attribute("type")->value());
		_dir = load_direction_type(obj_node->first_attribute("direction")->value());
		LoadFloat(obj_node->first_node("speed"), _speed);
		_iteration_count = load_iteration_count(obj_node->first_attribute("iteration")->value());
		xml_node<> *obj_points = obj_node->first_node("points"), *point;
		float x, y, z;
		if (_type == trajectory_type::line_strip)
		{
			for (point = obj_points->first_node(); point; point = point->next_sibling())
			{
				LoadFloat(point->first_node("x"), x);
				LoadFloat(point->first_node("y"), y);
				LoadFloat(point->first_node("z"), z);
				_points->emplace_back(x, y, z);
			}
			if (!strcmp(obj_node->first_attribute("type")->value(), "line_loop"))
			{
				_points->emplace_back((*_points)[0]);
			}
		}
		if (_type == trajectory_type::circle)
		{
			point = obj_node->first_node("center");
			LoadRGB(point, _center);
			point = obj_node->first_node("rotationPlane");
			LoadFloat(point->first_node("x"), _rotX.x);
			LoadFloat(point->first_node("z"), _rotZ.z);
			LoadFloat(obj_node->first_node("radius"), _radius);
		}
	}

	_trajectory = new Trajectory(_type, _dir, _speed, _iteration_count,
		_points, _center, _rotX, _rotZ,
		_radius);
}


void SceneManager::LoadTerrainAttributes(xml_node<>* const &Objects, int &_nr_cells, float &_size_cell, float &_offset_y, Vector3 &_height)
{
	//
	xml_node<> *obj_node;
	//
	_nr_cells = 85;
	_size_cell = 30;
	_offset_y = -850;
	//
	obj_node = Objects->first_node("height");
	LoadRGB(obj_node, _height);
}

void SceneManager::LoadFireAttributes(xml_node<>* const & Objects, float & _u_DispMax)
{
	LoadFloat(Objects->first_node("u_DispMax"), _u_DispMax);
}

void SceneManager::LoadSkyBoxAttributes(xml_node<>* const & Objects, float & _offset_y)
{
	LoadFloat(Objects->first_node("offset_y"), _offset_y);
}

void SceneManager::LoadReflectableAttributes(xml_node<>* const & Objects, float & _reflectiveness)
{
	LoadFloat(Objects->first_node("reflectiveness"), _reflectiveness);
}

void SceneManager::SendCommonConstantUniforms()
{
	ResourceManager::getInstance()->SendCommonConstantUniforms(&fog.r, r, R, &ambiental_light_color.r, ratio);
}
void SceneManager::SendCommonUniforms()
{
	ResourceManager::getInstance()->SendCommonUniforms(&active_cameraPtr->position.x);
}

void SceneManager::Init()
{
	quadVertices[0].pos = Vector3(-1, -1, 0);
	quadVertices[0].uv = Vector2(0, 0);
	quadVertices[1].pos = Vector3(-1, 1, 0);
	quadVertices[1].uv = Vector2(0, 1);
	quadVertices[2].pos = Vector3(1, 1, 0);
	quadVertices[2].uv = Vector2(1, 1);
	quadVertices[3].pos = Vector3(1, -1, 0);
	quadVertices[3].uv = Vector2(1, 0);

	quadIndices[0] = 0; quadIndices[1] = 1; quadIndices[2] = 2;
	quadIndices[3] = 0; quadIndices[4] = 2; quadIndices[5] = 3;

	glGenBuffers(1, &quadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), quadVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &quadIbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), quadIndices, GL_STATIC_DRAW);

	draw_state = draw_type::DRAW_FULL;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	xml_document<> doc;
	ifstream file("sceneManager.xml");
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);
	xml_node<> *pRoot = doc.first_node();

	InitBackgroundColor(pRoot);
	InitFog(pRoot);
	InitDebugColor(pRoot);

	InitCameras(pRoot);
	InitLightObjects(pRoot);

	
	SceneObject::SetLights(&lights);
	InitSceneObjects(pRoot);


	content.clear();
	doc.clear();

	Key(0);
	Update(0);
	glClearColor(background_color.r, background_color.g, background_color.b, 0.0f);
	SendCommonConstantUniforms();
	return;
}

void SceneManager::Draw()
{
	GLuint frameBuffer1, depthRenderBuffer1, depthTexture1, texture1,
		frameBuffer10, depthRenderBuffer10, texture10,
		frameBuffer2, texture2,
		frameBuffer3, texture3;
	
	glGenFramebuffers(1, &frameBuffer1);
	glGenRenderbuffers(1, &depthRenderBuffer1);
	glGenTextures(1, &texture1);
	glGenTextures(1, &depthTexture1);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Globals::screenWidth, Globals::screenHeight,
		0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glBindTexture(GL_TEXTURE_2D, depthTexture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Globals::screenWidth,
		Globals::screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT,
		NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//glEnable(GL_DEPTH_TEST);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Globals::screenWidth, Globals::screenHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture1, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer1);


	//framebuffer 2
	glGenFramebuffers(1, &frameBuffer2);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Globals::screenWidth, Globals::screenHeight,
		0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);

	//framebuffer 3
	glGenFramebuffers(1, &frameBuffer3);
	glGenTextures(1, &texture3);

	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Globals::screenWidth, Globals::screenHeight,
		0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer3);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture3, 0);
	//

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (draw_state == draw_type::DRAW_FULL)
		{
		SendCommonUniforms();
		for (auto &x : objects)
			x->DrawFull(active_cameraPtr);
		}
		else
		{
		for (auto &x : objects)
			x->DrawWired(active_cameraPtr, ResourceManager::getInstance()->reqDebugShader());
		}
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer2);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	ResourceManager::getInstance()->reqFilterShader()->Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer3);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	ResourceManager::getInstance()->reqBlurShader()->Use();
	ResourceManager::getInstance()->reqBlurShader()->Send_r(0.002);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	int n = 7;
	for (int i = 0; i < n; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer2);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		ResourceManager::getInstance()->reqBlurShader()->Use();
		ResourceManager::getInstance()->reqBlurShader()->Send_r(0.002*(2*i+1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer3);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ResourceManager::getInstance()->reqBlurShader()->Use();
		ResourceManager::getInstance()->reqBlurShader()->Send_r(0.002*(2*i+1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	ResourceManager::getInstance()->reqBloomShader()->Use();
	ResourceManager::getInstance()->reqBloomShader()->Send_r(active_cameraPtr->cam_near);
	ResourceManager::getInstance()->reqBloomShader()->Send_R(active_cameraPtr->cam_far);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glDeleteRenderbuffers(1, &depthRenderBuffer1);
	glDeleteFramebuffers(1, &frameBuffer1);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &depthTexture1);
	glDeleteFramebuffers(1, &frameBuffer2);
	glDeleteFramebuffers(1, &frameBuffer3);
	glDeleteTextures(1, &texture2);
	glDeleteTextures(1, &texture3);
}

void SceneManager::Update(float const &deltaTime)
{
	active_cameraPtr->Update(deltaTime);
	for (auto &x : objects)
		x->Update(deltaTime, active_cameraPtr);
	
	if (false)//collision
	{
		vector<SceneObject*>::iterator i, j;
		for (i = objects.begin(); i < objects.end(); i++)
			for (j = i + 1; j != objects.end(); j++)
			{
				if ((*i)->Colliding(*j))
				{
					//cout << "Obiectele se lovesc";
				}
			}
	}
}

void SceneManager::Key(unsigned char const & key)
{
	//cout << key;
	if (key == VK_UP)
	{
		if (draw_state == draw_type::DRAW_FULL)
			draw_state = draw_type::DRAW_WIRED;
		else draw_state = draw_type::DRAW_FULL;
		ResourceManager::getInstance()->playSound(SoundReference::about_time);
	}
	else
	{
		active_cameraPtr->Key(key);
		for (auto &x : objects)
			x->Key(key, active_cameraPtr);
	}
}
