#pragma once
#include "MeshStructs.h"
#include <vector>
#include <string>
#include <fstream>


class LeapLevel
{
private:
	void loader(const char* fileName);

public:
	LeapLevel();
	LeapLevel(const char* fileName);
	~LeapLevel();

	std::string fileName;

	Counter counterReader;
	std::vector <LevelObject*> levelObjects;
};

