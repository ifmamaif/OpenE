#include "stdafx.h"
#include "SceneObject.h"
#include <time.h>

float const LightObject::dir[3] = { 1.0,0.0,0.0 };
float const LightObject::point[3] = { 0.0,1.0,0.0 };
float const LightObject::spot[3] = { 0.0,0.0,1.0 };
float const LightObject::none[3] = { 0.0,0.0,0.0 };

map<int, LightObject> * SceneObject::lights = NULL;

void SceneObject::Init(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest, float const & _kDiffuse, float const & _kSpecular, int const & _nr_lights, int * const & _vec_id_light, Trajectory * const & _trajectory, Vector3 const & _wiredColor, Vector3 const & _AABBColor)
{
	Id = _Id; type = _type; name = _name;
	position = _position; rotation = _rotation; scale = _scale;
	id_model = _id_model;
	nr_textures = _nr_textures; vec_id_texture = _vec_id_texture;
	id_shader = _id_shader;
	blend = _blend; useDepthTest = _useDepthTest;
	kDiffuse = _kDiffuse; kSpecular = _kSpecular;
	nr_lights = _nr_lights; vec_id_light = _vec_id_light;
	trajectoryPtr = _trajectory;
	wiredColor = _wiredColor;
	AABBColor = _AABBColor;
	extremePoints = new float[6];
	AABB = new Vertex[8];
	indices = new GLushort[24];
	defineInternalModelMatrix();
}

void SceneObject::defineInternalModelMatrix()
{
	internalModelMatrix = Matrix().SetScale(scale) * Matrix().SetRotationX(rotation.x) *
		Matrix().SetRotationY(rotation.y) * Matrix().SetRotationZ(rotation.z);
	
}

void SceneObject::defineExternalModelMatrix()
{
	externalModelMatrix = internalModelMatrix * trajectoryPtr->trajectoryMatrix(clock()/10) * Matrix().SetTranslation(position); ;
}

void SceneObject::LoadModel(ResourceManager * const &ReqResources)
{
	modelPtr = ReqResources->reqModel(id_model);	
}

void SceneObject::LoadTextures(ResourceManager * const &ReqResources)
{
	texturePtrs = new Texture*[nr_textures];
	for (int i = 0; i < nr_textures; i++)
		texturePtrs[i] = ReqResources->reqTexture(vec_id_texture[i]);
}

void SceneObject::LoadShader(ResourceManager * const &ReqResources)
{
	shaderPtr = ReqResources->reqShader(id_shader);
}

void SceneObject::LoadLights()
{
	lightPtrs = new LightObject*[nr_lights];
	for (int i = 0; i < nr_lights; i++)
		lightPtrs[i] = (&(*lights)[vec_id_light[i]]);
}

 void SceneObject::Load()
{
	ResourceManager *ReqResources = ResourceManager::getInstance();
	LoadModel(ReqResources);
	LoadTextures(ReqResources);
	LoadShader(ReqResources);
	LoadLights();
}

 void SceneObject::BindFull(Camera * const & eye)
 {
	 modelPtr->Bind(draw_type::DRAW_FULL);
	 shaderPtr->Use();
	 int i;
	 for (i = 0; i < nr_textures; i++)
		 texturePtrs[i]->Bind(i);
	 for (int j = 0; j < nr_lights; j++)
		 lightPtrs[j]->lightDepthFrameBuffer.UseDepthTexture(i+j);
	 for (i = 0; i < nr_lights; i++)
		lightPtrs[i]->SendTo(shaderPtr, i);
	 for (i; i < 4; i++)
		 LightObject::SendNone(shaderPtr, i);
	 shaderPtr->SendViewProjectionMatrix(&eye->viewProjectionMatrix.m[0][0]);
	 shaderPtr->SendModelMatrix(&externalModelMatrix.m[0][0]);
	 shaderPtr->SendNormalMatrix(&normalMatrix.m[0][0]);
	 shaderPtr->SendKDiffuse(kDiffuse);
	 shaderPtr->SendKSpecular(kSpecular);
 }

 void SceneObject::BindLightDepth(Camera * const & eye, Shader * const &shader)
 {
	 modelPtr->Bind(draw_type::DRAW_FULL);
	 shader->Use();

	 shader->SendViewProjectionMatrix(&eye->viewProjectionMatrix.m[0][0]);
	 shader->SendModelMatrix(&externalModelMatrix.m[0][0]);
 }

 void SceneObject::BindWired(Camera * const & eye, Shader * const & debugShader)
 {
	 modelPtr->Bind(draw_type::DRAW_WIRED);
	 debugShader->Use();

	 debugShader->isWired();
	 debugShader->SendViewProjectionMatrix(&eye->viewProjectionMatrix.m[0][0]);
	 debugShader->SendModelMatrix(&externalModelMatrix.m[0][0]);
	 debugShader->SendNormalMatrix(&normalMatrix.m[0][0]);
	 debugShader->SendColor(&wiredColor.r);
 }

 void SceneObject::Move()
 {
	 defineExternalModelMatrix();
	 normalMatrix = externalModelMatrix;
	 modelPtr->getExtremePoints(externalModelMatrix, extremePoints);
 }

 SceneObject::SceneObject(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest, float const & _kDiffuse, float const & _kSpecular, int const & _nr_lights, int * const & _vec_id_light, Trajectory * const & _trajectory, Vector3 const & _wiredColor, Vector3 const & _AABBColor)
 {
	 Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor);
	 Load();
 }

void SceneObject::DrawFull(Camera * const &eye)
{
	BindFull(eye);
	SendDataToShader();
	modelPtr->Draw(draw_type::DRAW_FULL);	
}

void SceneObject::DrawLightDepth(Camera * const &eye, Shader * const &shader)
{
	BindLightDepth(eye, shader);
	modelPtr->Draw(draw_type::DRAW_FULL);
}

void SceneObject::DrawWired(Camera * const & eye, Shader * const & debugShader)
{
	BindWired(eye, debugShader);
	modelPtr->Draw(draw_type::DRAW_WIRED);

	modelPtr->Bind(draw_type::DRAW_NORMALS);
	debugShader->SendColor(&AABBColor.r);
	debugShader->Use();
	modelPtr->Draw(draw_type::DRAW_NORMALS);

	Model::getAABB(extremePoints, AABB, indices);
	GLuint  AABBvbo, AABBibo;
	
	glGenBuffers(1, &AABBvbo);
	glBindBuffer(GL_ARRAY_BUFFER, AABBvbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), AABB, GL_STATIC_DRAW);
	//delete[] verticesData;
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &AABBibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLushort), indices, GL_STATIC_DRAW);
	//delete[] indicesData;
	debugShader->Use();
	//debugShader->isWired();
	Matrix identityMatrix;
	identityMatrix.SetIdentity();

	
	debugShader->SendViewProjectionMatrix(&eye->viewProjectionMatrix.m[0][0]);
	debugShader->SendModelMatrix(&identityMatrix.m[0][0]);
	debugShader->SendColor(&AABBColor.r);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);

	glDeleteBuffers(1, &AABBvbo);
	glDeleteBuffers(1, &AABBibo);

}

bool SceneObject::Colliding(SceneObject * other)
{	
	if (this->extremePoints[dreapta] >= other->extremePoints[stanga] &&
		this->extremePoints[stanga] <= other->extremePoints[dreapta] &&
		this->extremePoints[sus] >= other->extremePoints[jos] &&
		this->extremePoints[jos] <= other->extremePoints[sus] &&
		this->extremePoints[spate] >= other->extremePoints[fata] &&
		this->extremePoints[fata] <= other->extremePoints[spate])
	{
		if (this->Id != 4 && other->Id != 4)//sky
		{
			cout << this->name << ' ' << other->name << " se lovesc\n";
			if (this->Id == 100 && other->Id == 2)
				ResourceManager::getInstance()->playSound(SoundReference::toc);
			return true;
		}
	}

	return false;
}	

void SceneObject::Update(float const & deltaTime, Camera * const & eye)
{
	PrepareAllData(deltaTime, eye);
	Move();
}

void TerrainObject::defineInternalModelMatrix()
{
	internalModelMatrix = Matrix().SetScale(scale);
}

void TerrainObject::defineExternalModelMatrix()
{
	externalModelMatrix = internalModelMatrix * CenterTerrainOnCamera;
}

 void TerrainObject::LoadModel(ResourceManager * const &ReqResources)
{
	modelPtr = ReqResources->reqTerrainModel(nr_cells, size_cell, offset_y);
}

 void TerrainObject::SendDataToShader()
 {
	 shaderPtr->SendMoveTexture(moveTexture);
	 shaderPtr->SendHeightMap(&height.x);
 }

 TerrainObject::TerrainObject(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest, float const & _kDiffuse, float const & _kSpecular, int const & _nr_lights, int * const & _vec_id_light, Trajectory * const & _trajectory, Vector3 const & _wiredColor, Vector3 const & _AABBColor, int const & _nr_cells, float const & _size_cell, float const & _offset_y, Vector3 const & _height)
 {
	 Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor);
	 nr_cells = _nr_cells; size_cell = _size_cell; offset_y = _offset_y;
	 height = _height;
	 Load();
 }

 void TerrainObject::Key(unsigned char const & key, Camera * const & eye)
  {
	  u = size_cell * round(eye->position.x / size_cell);
	  v = size_cell * round(eye->position.z / size_cell);
	  CenterTerrainOnCamera.SetTranslation(u, offset_y, v);
	  moveTexture[0] = u / size_cell / nr_cells;
	  moveTexture[1] = v / size_cell / nr_cells;
	  Move();
  }

  void FireObject::PrepareAllData(float const & deltaTime, Camera * const &eye)
  {
	  u_Time = 3.5*clock();
	  u_Time /= 10000;
	  u_Time -= (int)u_Time;
  }

  void FireObject::SendDataToShader()
  {
	  shaderPtr->Send_u_DispMax(u_DispMax);
	  shaderPtr->Send_u_Time(u_Time);
  }

  FireObject::FireObject(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest, float const & _kDiffuse, float const & _kSpecular, int const & _nr_lights, int * const & _vec_id_light, Trajectory * const & _trajectory, Vector3 const & _wiredColor, Vector3 const & _AABBColor, float const & _u_DispMax)
  {
	  Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor);
	  u_DispMax = _u_DispMax;
	  Load();
  }

  void SkyBoxObject::defineExternalModelMatrix()
  {
	  externalModelMatrix = internalModelMatrix * CenterSkyBoxOnCamera;
  }

  SkyBoxObject::SkyBoxObject(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest, float const & _kDiffuse, float const & _kSpecular, int const & _nr_lights, int * const & _vec_id_light, Trajectory * const & _trajectory, Vector3 const & _wiredColor, Vector3 const & _AABBColor, float const & _offset_y)
  {
	  Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor);
	  offset_y = _offset_y;
	  Load();
  }

  void SkyBoxObject::Key(unsigned char const & key, Camera * const & eye)
  {
	  CenterSkyBoxOnCamera.SetTranslation(eye->position.x, offset_y, eye->position.z);
	  Move();
  }

  void ReflectableObject::SendDataToShader()
  {
	  shaderPtr->SendReflectiveness(reflectiveness);
  }

  ReflectableObject::ReflectableObject(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest, float const & _kDiffuse, float const & _kSpecular, int const & _nr_lights, int * const & _vec_id_light, Trajectory * const & _trajectory, Vector3 const & _wiredColor, Vector3 const & _AABBColor, float const & _reflectiveness)
  {
	  Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest, _kDiffuse, _kSpecular, _nr_lights, _vec_id_light, _trajectory, _wiredColor, _AABBColor);
	  reflectiveness = _reflectiveness;
	  Load();
  }

  Trajectory::Trajectory(trajectory_type const & _type, direction const & _dir, float const & _speed, int const & _iteration_count, vector<Vector3>* const & _points, Vector3 const & _center, Vector3 const & _rotX, Vector3 const & _rotZ, float const & _radius)
  {
	  type = _type;
	  dir = _dir;
	  speed = _speed;
	  iteration_count = _iteration_count;
	  points = _points;
	  center = _center;
	  rotX = _rotX;
	  rotZ = _rotZ;
	  radius = _radius;

	  total_length = 0;
	  if (type == trajectory_type::line_strip)
	  {
		  vector<Vector3>::iterator x, y;
		  for (x = points->begin(), y = x + 1; y != points->end(); x++, y++)
			  total_length += (*y - *x).Length();
	  }
	  else if (type == trajectory_type::circle)
		  total_length = 2 * Constants::pi * radius;

  }

  Matrix Trajectory::trajectoryMatrix(float const & time)
  {
	  Matrix identityMatrix;
	  identityMatrix.SetIdentity();
	  Vector3 up(0, 1, 0);
	  if (type == trajectory_type::line_strip)
	  {
		  float length_traversed = time * speed;

		  if (dir == direction::normal)
		  {
			  float iterations_traversed = length_traversed / total_length;
			  int actual_iteration = iterations_traversed;//float -> int conversion

			  if (actual_iteration > iteration_count && iteration_count != -1) return identityMatrix;
			  else
			  {
				  float local_length = length_traversed - actual_iteration * total_length;

				  vector<Vector3>::iterator x, y;
				  for (x = points->begin(), y = x + 1; y != points->end(); x++, y++)
				  {
					  float L = (*y - *x).Length();
					  if (local_length > L)
						  local_length -= L;
					  else
					  {
						  float lambda = local_length / L;
						  if (lambda < 0 || lambda>1)cout << "Eroare";
						  return Camera::getObjectWorldMatrix(*x, *y, up, lambda);
					  }
				  }
			  }
		  }
		  else if (dir == direction::alternate)
		  {
			  float iterations_traversed = length_traversed / total_length;
			  int actual_iteration = iterations_traversed;//float -> int conversion

			  if (actual_iteration > iteration_count && iteration_count != -1) return identityMatrix;
			  else
			  {
				  float local_length = length_traversed - actual_iteration * total_length;


				  vector<Vector3>::iterator x, y;
				  if (actual_iteration % 2 == 0)
				  {
					  for (x = points->begin(), y = x + 1; y != points->end(); x++, y++)
					  {
						  float L = (*y - *x).Length();
						  if (local_length > L)
							  local_length -= L;
						  else
						  {
							  float lambda = local_length / L;
							  if (lambda < 0 || lambda>1)cout << "Eroare";
							  return Camera::getObjectWorldMatrix(*x, *y, up, lambda);
						  }
					  }
				  }
				  else
				  {
					  for (x = points->end()-1, y = x - 1; x != points->begin(); x--, y--)
					  {
						  float L = (*y - *x).Length();
						  if (local_length > L)
							  local_length -= L;
						  else
						  {
							  float lambda = local_length / L;
							  if (lambda < 0 || lambda>1)cout << "Eroare";
							  return Camera::getObjectWorldMatrix(*x, *y, up, lambda);
						  }
					  }
				  }
			  }
		  }
	  }
	  else if (type == trajectory_type::circle)
	  {
		  float length_traversed = time * speed;

			  float iterations_traversed = length_traversed / total_length;
			  int actual_iteration = iterations_traversed;//float -> int conversion

			  if (actual_iteration > iteration_count && iteration_count != -1) return identityMatrix;
			  else
			  {
				  float local_length = length_traversed - actual_iteration * total_length;
						  float lambda = local_length / total_length;
						  if (lambda < 0 || lambda>1)cout << "Eroare";
						  return Camera::getObjectWorldMatrix(center, radius, lambda, rotX.x, rotZ.z);
				  
			  }		 
	  }
	  return identityMatrix;
  }

  void LightObject::SendTo(Shader * const & shaderPtr, int const & i)
  {
	  switch (type)
	  {
	  case light_type::directional: shaderPtr->SendLightType(dir, i);
		  break;
	  case light_type::point: shaderPtr->SendLightType(point, i);
		  break;
	  case light_type::spotlight: shaderPtr->SendLightType(spot, i);
		  break;
	  }	 
	  if(!isPointLight())
		shaderPtr->SendLightViewProjectionMatrix(&lightPOV[0]->viewProjectionMatrix.m[0][0], i);
	  else
	  {

	  }
	  shaderPtr->SendLightPos(&position.x, i);
	  shaderPtr->SendLightDir(&direction.x, i);
	  shaderPtr->SendLightColor(&light_color.r, i);
	  if (type == light_type::ambient)
		  shaderPtr->SendSpecularPow(specular_power, i);
	  else shaderPtr->SendAlpha(alpha, i);
  }

  LightObject::LightObject(GLuint const & _Id, light_type const & _type, Vector3 const & _position, Vector3 const & _direction, Vector3 const & _light_color, GLfloat const & _specular_power, GLfloat const & _alpha):
	  lightDepthFrameBuffer(true, _type == light_type::point)
  {
	  Id = _Id;
	  type = _type;
	  position = _position;
	  direction = _direction;
	  light_color = _light_color;
	  specular_power = _specular_power;
	  alpha = _alpha;
	  if (type == light_type::directional)
		  lightPOV[0] = new Camera(Vector3(0, 0, 0) - direction*50, Vector3(0, 0, 0) + direction, Vector3(0, 1, 0), 0.1, 0.1, 0.2, 3000, 2*alpha);
	  else if(type == light_type::spotlight)
		  lightPOV[0] = new Camera(position, position + direction, Vector3(0, 1, 0), 0.1, 0.1, 0.2, 3000, 2*alpha);
	  else
	  {
		  lightPOV[0] = new Camera(position, Vector3(1, 0, 0), Vector3(0, 1, 0), 0.1, 0.1, 0.2, 3000, 1.57);
		  lightPOV[1] = new Camera(position, Vector3(-1, 0, 0), Vector3(0, 1, 0), 0.1, 0.1, 0.2, 3000, 1.57);
		  lightPOV[2] = new Camera(position, Vector3(0, 1, 0), Vector3(1, 0, 0), 0.1, 0.1, 0.2, 3000, 1.57);
		  lightPOV[3] = new Camera(position, Vector3(0, -1, 0), Vector3(1, 0, 0), 0.1, 0.1, 0.2, 3000, 1.57);
		  lightPOV[4] = new Camera(position, Vector3(0, 0, 1), Vector3(0, 1, 0), 0.1, 0.1, 0.2, 3000, 1.57);
		  lightPOV[5] = new Camera(position, Vector3(0, 0, -1), Vector3(0, 1, 0), 0.1, 0.1, 0.2, 3000, 1.57);
	  }
  }

  void LightObject::SendNone(Shader * const & shaderPtr, int const & i)
  {
	  shaderPtr->SendLightType(none, i);
  }
