#include "LeapImporter.h"

LeapImporter::LeapImporter()
{

}

LeapImporter::~LeapImporter()
{	
	deleteMesh();
	deleteLevel();
}

LeapMesh * LeapImporter::getMesh(const char* meshName)
{
	LeapMesh* mesh = new LeapMesh(meshName);
	leapMesh.push_back(mesh);
	return mesh;
}

LeapLevel * LeapImporter::getLevel(const char * levelName)
{
	LeapLevel* level = new LeapLevel(levelName);
	leapLevel.push_back(level);
	return level;
}

void LeapImporter::deleteLevel()
{
	for (LeapLevel* level_ptr : leapLevel)
		delete level_ptr;
	leapLevel.clear();
}

void LeapImporter::deleteMesh()
{
	for (LeapMesh* mesh_ptr : leapMesh)
		delete mesh_ptr;
	leapMesh.clear();
}

