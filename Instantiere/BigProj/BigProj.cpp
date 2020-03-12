#include "pch.h"
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <time.h>
using namespace std;
#include <vector>
#include <iostream>
#include <fstream>
#include "Globals.h"
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h
#define _USE_MATH_DEFINES
#include <math.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm/glm.hpp"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "Vertex.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "generator_3D.h"

void Initialize(void);
void Cleanup(void);
void RenderFunction(void);
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int xx, int yy);
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Globals::screenWidth, Globals::screenHeight);
	glutCreateWindow("BigProj");
	glewInit();

	//generate_hyperboloid_2_sheet(40);

	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
	return 0;
}
void Initialize(void)
{
	srand(time(0));
	ResourceManager *RM = ResourceManager::getInstance();
	RM->Init();
	SceneManager *SM = SceneManager::getInstance();
	SM->Init();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void Cleanup(void)
{
	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();
};
void RenderFunction(void)
{
	static SceneManager *SM = SceneManager::getInstance();
	SM->Update();
	SM->Draw();
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y) {
	Camera &eye = SceneManager::getInstance()->getActiveCamera();
	switch (key)
	{
	case 'w':eye.moveOz(-1); break;
	case 's':eye.moveOz(1); break;
	case 'a':eye.moveOx(-1); break;
	case 'd':eye.moveOx(1); break;
	case 'x':eye.moveOy(-1); break;
	case 'z':eye.moveOy(1); break;
	case '2':eye.rotateOx(-1); break;
	case '5':eye.rotateOx(1); break;
	case '3':eye.rotateOy(-1); break;
	case '1':eye.rotateOy(1); break;
	case '7':eye.rotateOz(-1); break;
	case '9':eye.rotateOz(1); break;
	}
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {
	Camera &eye = SceneManager::getInstance()->getActiveCamera();
	//cout << key << ' ' << xx << ' ' << yy << '\n';
	switch (key) {
	default:;
	}
};