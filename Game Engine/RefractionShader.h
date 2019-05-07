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


class RefractionShader : public ShaderBase
{
public:
	RefractionShader();
	RefractionShader(int otherAssetID);
	~RefractionShader();

	void CreateShaderData();

	GLuint RefractionShaderProgram;
	int assetID;
};