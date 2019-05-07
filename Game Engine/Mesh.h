#pragma once
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))


#include <windows.h>
#include <string>
#include <GL/gl3w.h>  
#include "Vertex.h"
#include "LeapImporter.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "glm/glm/glm.hpp"
#include <algorithm>    // std::unique, std::distance
#include "Terrain.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::string filePath, GLuint gShaderProgram);
	~Mesh();

	void CreateMeshData(std::string filePath, GLuint gShaderProgram);
	void deleteLeapMesh();
	//std::vector<Vertex> Vertices;
	//std::vector<unsigned int> indices;
	//LeapImporter importer;
	LeapMesh* leapMesh;
	//int vertexCount;
	unsigned int meshType; //remove this later and use *leapMesh
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;
};

