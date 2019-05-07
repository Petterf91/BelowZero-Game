#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>

#include <GL/gl3w.h>  
#include "ShaderBase.h"
#include "Mesh.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

class UIShaders : public ShaderBase
{
public:
	UIShaders();
	UIShaders(int otherAssetID);
	~UIShaders();

	void CreateShaderData();

	
	GLuint UIShaderProgram;
	int assetID;
};
