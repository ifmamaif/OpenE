#include "stdafx.h"
#include "SceneObject.h"

void SceneObject::Init(GLuint const & _Id, object_type const & _type, char * const & _name, Vector3 const & _position, Vector3 const & _rotation, Vector3 const & _scale, int const & _id_model, int const & _nr_textures, int * const & _vec_id_texture, int const & _id_shader, bool const & _blend, bool const & _useDepthTest)
{
	Id = _Id; type = _type; name = _name; 
	position = _position; rotation = _rotation; scale = _scale;
	id_model = _id_model;
	nr_textures = _nr_textures; vec_id_texture = _vec_id_texture;
	id_shader = _id_shader;
	blend =_blend; useDepthTest = _useDepthTest;
	defineModelMatrix();
}

void SceneObject::defineModelMatrix()
{
	modelMatrix = Matrix().SetScale(scale) * Matrix().SetRotationX(rotation.x) *
		Matrix().SetRotationY(rotation.y) * Matrix().SetRotationZ(rotation.z) *
		Matrix().SetTranslation(position);
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

inline void SceneObject::Load()
{
	ResourceManager *ReqResources = ResourceManager::getInstance();
	LoadModel(ReqResources);
	LoadTextures(ReqResources);
	LoadShader(ReqResources);
}

void SceneObject::Bind()
{
	modelPtr->Bind(draw_type::DRAW_FULL);
	for (int i = 0; i < nr_textures; i++)
		texturePtrs[i]->Bind(i);
	shaderPtr->Use();
}

SceneObject::SceneObject(
	GLuint const &_Id, object_type const &_type, char * const &_name,
	Vector3 const &_position, Vector3 const &_rotation, Vector3 const &_scale,
	int const &_id_model,
	int const &_nr_textures, int * const &_vec_id_texture,
	int const &_id_shader,
	bool const &_blend, bool const &_useDepthTest)
	{
	Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest);	
	Load();
}

void SceneObject::Draw(Camera * const &eye)
{
	Bind();
	Matrix MVPmatrix = modelMatrix * eye->view_projMatrix;
	shaderPtr->SendMVP(&MVPmatrix.m[0][0]);

	float fogColor[]{ 0.0,0.0,0.0 };
	shaderPtr->SendFogColor(fogColor);
	Matrix mMatrix = modelMatrix;
	shaderPtr->SendModelMatrix(&mMatrix.m[0][0]);
	shaderPtr->SendCameraPos(&eye->position.x);
	shaderPtr->Send_r(70.0);
	shaderPtr->Send_R(150.0);


	modelPtr->Draw(draw_type::DRAW_FULL);
	
}

void TerrainObject::defineModelMatrix()
{
	modelMatrix = Matrix().SetScale(scale);
}

inline void TerrainObject::LoadModel(ResourceManager * const &ReqResources)
{
	modelPtr = ReqResources->reqTerrainModel(nr_cells, size_cell, offset_y);
}

TerrainObject::TerrainObject(
	GLuint const &_Id, object_type const &_type, char * const &_name,
	Vector3 const &_position, Vector3  const &_rotation, Vector3 const &_scale,
	int const &_id_model,
	int const &_nr_textures, int * const &_vec_id_texture,
	int const &_id_shader,
	bool const &_blend, bool const &_useDepthTest,
	int const &_nr_cells, float const &_size_cell, float const &_offset_y,
	Vector3 const &_height)
 {
	Init(_Id, _type, _name, _position, _rotation, _scale, _id_model, _nr_textures, _vec_id_texture, _id_shader, _blend, _useDepthTest);
	nr_cells = _nr_cells; size_cell = _size_cell; offset_y = _offset_y;
	height = _height;
	Load();
}

 void TerrainObject::Draw(Camera * const & eye)
 {
	 Bind();
	 Matrix CenterTerrainOnCamera;
	// GLfloat size_cell = ((TerrainModel*)modelPtr)->GetSizeCell();
	 GLfloat u = size_cell * round(eye->position.x / size_cell);
	 GLfloat v = size_cell * round(eye->position.z / size_cell);
	 CenterTerrainOnCamera.SetTranslation(u,0,v);
	 Matrix MVPmatrix = modelMatrix * CenterTerrainOnCamera * eye->view_projMatrix;
	 //Matrix MVPmatrix = modelMatrix * eye->view_projMatrix;
	 //GLint nr_cells = ((TerrainModel*)modelPtr)->GetNrCells();
	 GLfloat moveTexture[]{ u/size_cell/nr_cells, v/size_cell/nr_cells };

	 shaderPtr->SendMVP(&MVPmatrix.m[0][0]);
	 shaderPtr->SendMoveTexture(moveTexture);
	 float fogColor[]{ 0.0,0.0,0.0 };
	 shaderPtr->SendFogColor(fogColor);
	 Matrix mMatrix = modelMatrix * CenterTerrainOnCamera;
	 shaderPtr->SendModelMatrix(&mMatrix.m[0][0]);
	 shaderPtr->SendCameraPos(&eye->position.x);
	 shaderPtr->Send_r(100.0);
	 shaderPtr->Send_R(150.0);
	 shaderPtr->SendHeightMap(&height.x);
	 modelPtr->Draw(draw_type::DRAW_FULL);
	 
 }
