#include "stdafx.h"
#include "Camera.h"

void Camera::moveOx(GLint directie)
{
	Vector3 vectorDeplasare = xAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}
void Camera::moveOy(GLint directie)
{
	Vector3 vectorDeplasare = yAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}
void Camera::moveOz(GLint directie)
{
	Vector3 vectorDeplasare = zAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	updateWorldView();
}
void Camera::rotateOx(GLint directie)
{
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Matrix mRotateOX(0);
	mRotateOX.SetRotationX(directie * rotateSpeed * deltaTime);
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	Vector4 up = (rotatedLocalUp * worldMatrix).Normalize();
	this->up = Vector3(up.x, up.y, up.z);
	Vector4 localTarget = Vector4(0, 0, -(this->target - this->position).Length(), 1);
	Vector4	rotatedTarget = localTarget * mRotateOX;
	Vector4 target = rotatedTarget * worldMatrix;
	this->target = Vector3(target.x, target.y, target.z);
	updateWorldView();
}
void Camera::rotateOy(GLint directie)
{
	Vector4 localTarget = Vector4(0, 0, -(this->target - this->position).Length(), 1);
	Matrix mRotateOY(0);
	mRotateOY.SetRotationY(directie * rotateSpeed * deltaTime);
	Vector4	rotatedTarget = localTarget * mRotateOY;
	Vector4 target = rotatedTarget * worldMatrix;
	this->target = Vector3(target.x, target.y, target.z);
	updateWorldView();
}
void Camera::rotateOz(GLint directie)
{
	Matrix mRotateOZ(0);
	mRotateOZ.SetRotationZ(directie * rotateSpeed * deltaTime);
	Vector4 localUp = Vector4(0, 1, 0, 0);
	Vector4 rotatedLocalUp = localUp * mRotateOZ;
	Vector4 up = (rotatedLocalUp * worldMatrix).Normalize();
	this->up = Vector3(up.x, up.y, up.z);
	updateWorldView();
}
void Camera::updateWorldView()
{
	zAxis = -(target - position).Normalize();
	xAxis = up.Cross(zAxis).Normalize();
	yAxis = zAxis.Cross(xAxis);
	Matrix R(0);
	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z;
	R.m[3][3] = 1;

	Matrix T(0);
	T.SetTranslation(position);
	worldMatrix = R * T;
	T.SetTranslation(-position);
	viewMatrix = T * R.Transpose();
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

Matrix Camera::getObjectWorldMatrix(Vector3 & position, Vector3 & target, Vector3  & up, float & lambda)
{
	Vector3 zAxis = (target - position).Normalize();
	if (zAxis == up || zAxis == -up)
		up = Vector3(1, 0, 0);
	Vector3 xAxis = up.Cross(zAxis).Normalize();
	Vector3 yAxis = zAxis.Cross(xAxis);
	Matrix R(0);
	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z;
	R.m[3][3] = 1;
	Matrix T(0);
	T.SetTranslation(position*(1 - lambda) + target * lambda);
	return R * T;
}

Matrix Camera::getObjectWorldMatrix(Vector3 & center, float & radius, float & lambda, float &rotX, float &rotZ)
{
	Matrix T(0);
	T.SetTranslation(center);
	return T * Matrix().SetRotationY(2*Constants::pi*lambda) * Matrix().SetRotationX(rotX) * Matrix().SetRotationZ(rotZ);
}


Camera::Camera(Vector3 position, Vector3 target, Vector3 up, 
	           GLfloat moveSpeed, GLfloat rotateSpeed,
	           GLfloat cam_near, GLfloat cam_far, GLfloat cam_fov)
{
	this->position = position;
	this->target = target;
	this->up = up;
	this->moveSpeed = moveSpeed*20;
	this->rotateSpeed = rotateSpeed*15;
	this->cam_near = cam_near;
	this->cam_far = cam_far;
	this->cam_fov = cam_fov;
	projectionMatrix.SetPerspective(cam_fov, Globals::screenWidth / (float)Globals::screenHeight, cam_near, cam_far);
	updateWorldView();
}

void Camera::Update(float const & _deltaTime)
{
	deltaTime = _deltaTime;
}

void Camera::Key(unsigned char const & key)
{
	switch (key)
	{
	case 'W':moveOz(-1); break;
	case 'S':moveOz(1); break;
	case 'A':moveOx(-1); break;
	case 'D':moveOx(1); break;
	case 'X':moveOy(-1); break;
	case 'Z':moveOy(1); break;
	case 'b':rotateOx(-1); break;
	case 'h':rotateOx(1); break;
	case 'f':rotateOy(-1); break;
	case 'd':rotateOy(1); break;
	case 'i':rotateOz(-1); break;
	case 'a':rotateOz(1); break;
	}
}
