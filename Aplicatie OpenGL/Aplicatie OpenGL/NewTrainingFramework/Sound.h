#pragma once
#include <iostream>
#include <fstream>

#include "../Utilities/utilities.h"

using namespace std;

enum class SoundReference : int
{
	toc = 1,
	hapciu = 2,
	about_time = 3

};

class SoundResource
{
private:
	char* path;	
public:
	char const * const SoundPath() const;
	SoundResource(char * const &_path):path(_path){}
	SoundResource() { cout << "Constructor implicit apelat - SoundResource: Undefined behaviour.\n"; }
	~SoundResource() { delete[] path; }
};
