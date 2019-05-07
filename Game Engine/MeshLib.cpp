#include "MeshLib.h"

MeshLib::MeshLib()
{
}

MeshLib::~MeshLib()
{
	for (Mesh* mesh_ptr : meshes)
		delete mesh_ptr;
	meshes.clear();
	/*animatedMeshes.clear();*/
}

void MeshLib::addMesh(std::string filePath, GLuint gShaderProgram)
{
	Mesh* newMesh = new Mesh(filePath, gShaderProgram);
	meshes.push_back(newMesh);
}

//void MeshLib::addAnimatedMesh(std::string filePath, GLuint gShaderProgram)
//{
//	AnimatedMesh newAnimMesh = AnimatedMesh(filePath, gShaderProgram);
//	animatedMeshes.push_back(newAnimMesh);
//}

Mesh * MeshLib::getMesh(int key)
{
	return meshes.at(key);
}

//AnimatedMesh & MeshLib::getAnimatedMesh(int key)
//{
//	return animatedMeshes.at(key);
//}

int MeshLib::getNumberOfMeshes()
{
	return meshes.size();
}
