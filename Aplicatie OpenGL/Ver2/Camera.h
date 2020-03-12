#pragma once
#include "../Utilities/utilities.h"
#include "Globals.h"
#include <iostream>

class Camera 
{
public:
	Vector3 position, target, up, xAxis, yAxis, zAxis;
	GLfloat moveSpeed, rotateSpeed, cam_near, cam_far, cam_fov, deltaTime;
	Matrix viewMatrix, worldMatrix, projectionMatrix, view_projMatrix;
	
	void moveOx(GLint directie);
	void moveOy(GLint directie);
	void moveOz(GLint directie);
	void rotateOx(GLint directie);
	void rotateOy(GLint directie);
	void rotateOz(GLint directie);
	void updateWorldView();
	Camera(Vector3 position, Vector3 target, Vector3 up, 
		   GLfloat moveSpeed, GLfloat rotateSpeed, 
		   GLfloat cam_near, GLfloat cam_far, GLfloat cam_fov);
	Camera() { std::cout << "Constructor implicit apelat -Camera:Undefined Behaviour"; }
	~Camera() { }
};