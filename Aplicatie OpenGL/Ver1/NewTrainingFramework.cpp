// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include "Camera.h"

using namespace std;

POINT pct, pct_screen;
GLuint vboId, vboIndex, nr_vertices, nr_indexes, nr_triangles, id_textura;
Shaders myShaders;
Matrix rot(0), I4(0), S(0), T(0);
GLfloat theta=0;
Camera eye(Vector3(0, 0, -50), Vector3(0, 25, 0), Vector3(0, 1, 0), 10, 10, 0.2, 1000, 3.14f/4);

inline bool ShouldUpdate(float deltaTime)
{
	static float updateRate = 0.05, timePassed = 0;
	timePassed += deltaTime;
	if (timePassed > updateRate)
	{
		timePassed -= updateRate;
		return true;
	}
	return false;
}

inline void MouseActions(ESContext *esContext)
{
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		GetCursorPos(&pct_screen);
		ScreenToClient(esContext->hWnd, &pct_screen);		
	}
}

int Init ( ESContext *esContext )
{
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	I4.SetIdentity();
	S.SetScale(Vector3(0.25, 0.25, 0.25));
	T.SetTranslation(Vector3(0, 0, 0));
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	//triangle data (heap)
	std::ifstream model("../Resources/Models/bus.nfg");
	std::string line;
	std::getline(model, line);
	std::istringstream line_reader(line);
	line_reader.ignore(20, ':');
	line_reader >> nr_vertices;
	Vertex *verticesData = new Vertex[nr_vertices];
	int i;
	for(i=0;i<nr_vertices;i++)
	{
		std::getline(model, line);
		line_reader.clear();
		line_reader.str(line);
		line_reader.ignore(30, '[');
		line_reader >> verticesData[i].pos.x;	
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].pos.y;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].pos.z;
		line_reader.ignore(30, '[');
		line_reader >> verticesData[i].norm.x;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].norm.y;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].norm.z;
		line_reader.ignore(30, '[');
		line_reader >> verticesData[i].binorm.x;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].binorm.y;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].binorm.z;
		line_reader.ignore(30, '[');
		line_reader >> verticesData[i].tgt.x;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].tgt.y;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].tgt.z;
		line_reader.ignore(30, '[');
		line_reader >> verticesData[i].uv.x;
		line_reader.ignore(2, ',');
		line_reader >> verticesData[i].uv.y;
	}
	std::getline(model, line);
	line_reader.str(line);
	line_reader.ignore(20, ':');
	line_reader >> nr_indexes;
	unsigned short *indexesData = new unsigned short[nr_indexes];
	nr_triangles = nr_indexes / 3;
	for (i = 0; i < nr_triangles; i++)
	{
		std::getline(model, line);
		line_reader.clear();
		line_reader.str(line);
		line_reader.ignore(30, '.');
		line_reader >> indexesData[i*3];
		line_reader.ignore(2, ',');
		line_reader >> indexesData[i*3+1];
		line_reader.ignore(2, ',');
		line_reader >> indexesData[i*3+2];	
	}

	glGenTextures(1, &id_textura);
	glBindTexture(GL_TEXTURE_2D, id_textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	int width, height, bpp;
	char* pointer_textura = LoadTGA("../Resources/Textures/Bus.tga", &width, &height, &bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pointer_textura);
	delete[] pointer_textura;
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, nr_vertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nr_indexes * sizeof(unsigned short), indexesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	glBindTexture(GL_TEXTURE_2D, id_textura);
	if(myShaders.positionAttribute != -1 && myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));

		glEnableVertexAttribArray(myShaders.uvAttribute);
		glVertexAttribPointer(myShaders.uvAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4*sizeof(Vector3)));
		
		Matrix M(S*rot*T);
		Matrix MVP(M * eye.viewMatrix * eye.projectionMatrix);
		
		glUniformMatrix4fv(myShaders.rotzUniform,1,GL_FALSE,&MVP.m[0][0]);

		
	}
	glDrawElements(GL_TRIANGLES, nr_indexes, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	if (ShouldUpdate(deltaTime))
	{
		MouseActions(esContext);
		eye.deltaTime = deltaTime;
		theta += 0.9f*deltaTime;
		rot.SetRotationY(theta);
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if(bIsPressed)
		switch(key)
		{
		case 'W':eye.moveOz(-1); break;
		case 'S':eye.moveOz(1); break;
		case 'A':eye.moveOx(-1); break;
		case 'D':eye.moveOx(1); break;
		case 'X':eye.moveOy(-1); break;
		case 'Z':eye.moveOy(1); break;
		case 'b':eye.rotateOx(-1); break;
		case 'h':eye.rotateOx(1); break;
		case 'f':eye.rotateOy(-1); break;
		case 'd':eye.rotateOy(1); break;
		case 'i':eye.rotateOz(-1); break;
		case 'a':eye.rotateOz(1); break;
		}
	
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

