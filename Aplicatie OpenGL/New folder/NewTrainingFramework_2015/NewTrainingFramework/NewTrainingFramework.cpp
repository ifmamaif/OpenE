// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Globals.h"
#include "Camera.h"

using namespace std;

POINT pct_screen;

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

 void MouseActions(ESContext *esContext)
{
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		GetCursorPos(&pct_screen);
		ScreenToClient(esContext->hWnd, &pct_screen);		
	}
}

int Init ( ESContext *esContext )
{
	ResourceManager *RM = ResourceManager::getInstance();
	RM->Init();
	SceneManager *SM = SceneManager::getInstance();
	SM->Init();
	return 0; 
}

void Draw ( ESContext *esContext )
{
	//int err = glGetError();
	SceneManager::getInstance()->Draw();
	//err;
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	
	if (ShouldUpdate(deltaTime))
	{
		SceneManager::getInstance()->Update(deltaTime);
		MouseActions(esContext);
	}
	
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
		SceneManager::getInstance()->Key(key);		
}

void CleanUp()
{
	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB);

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

