#pragma once
#include "LeapMesh.h"
#include "LeapLevel.h"

#include <vector>
#include <iostream>
#include <list>
#include <memory>

using namespace std;

class LeapImporter
{
public:
	LeapImporter();
	~LeapImporter();

	LeapMesh* getMesh(const char* meshName);
	LeapLevel* getLevel(const char* levelName);

	list<LeapMesh*> leapMesh;
	list<LeapLevel*> leapLevel;

	void deleteLevel();
	void deleteMesh();

private:

};