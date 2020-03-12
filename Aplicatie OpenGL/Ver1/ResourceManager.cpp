#include <stdafx.h>
#include "ResourceManager.h"


ResourceManager* ResourceManager::uniqueRManager = NULL;

ResourceManager* ResourceManager::getInstance()
{
	if (uniqueRManager == NULL);
		uniqueRManager = new ResourceManager();
	return uniqueRManager;
}
ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

	uniqueRManager = NULL;
}