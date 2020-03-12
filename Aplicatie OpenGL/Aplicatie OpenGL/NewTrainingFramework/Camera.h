#pragma once
#include "../Utilities/utilities.h"
#include "Globals.h"
#include <iostream>

class Camera 
{
public:
	Vector3 position, target, up, xAxis, yAxis, zAxis;
	GLfloat moveSpeed, rotateSpeed, cam_near, cam_far, cam_fov, deltaTime;
	GLfloat left, right, top, bottom;
	Matrix viewMatrix, worldMatrix, projectionMatrix, viewProjectionMatrix;
	Vector3 normalized_target;

	void moveOx(GLint directie);
	void moveOy(GLint directie);
	void moveOz(GLint directie);
	void rotateOx(GLint directie);
	void rotateOy(GLint directie);
	void rotateOz(GLint directie);
	void updateWorldView();
	static Matrix getObjectWorldMatrix(Vector3 &position, Vector3 &target, Vector3 &up, float &lambda);
	static Matrix getObjectWorldMatrix(Vector3 &center, float &radius,  float &lambda, float &rotX, float &rotZ);
	Camera(Vector3 position, Vector3 target, Vector3 up, 
		   GLfloat moveSpeed, GLfloat rotateSpeed, 
		   GLfloat cam_near, GLfloat cam_far, GLfloat cam_fov);
	Camera(Vector3 const & position, Vector3 const & target, Vector3 const & up,
		GLfloat const & moveSpeed, GLfloat const & rotateSpeed,
		GLfloat left, GLfloat right,
		GLfloat top, GLfloat bottom,
		GLfloat cam_near, GLfloat cam_far);
	Camera() { std::cout << "Constructor implicit apelat -Camera:Undefined Behaviour"; }
	~Camera() { }
	void Update(float const &_deltaTime);
	void Key(unsigned char const &key);
};