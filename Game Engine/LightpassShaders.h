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


class LightpassShaders :public ShaderBase
{
public:
	LightpassShaders();
	LightpassShaders(int otherAssetID);
	~LightpassShaders();

	void CreateShaderData();

	//GLuint gVertexBuffer;
	//GLuint gVertexAttribute;
	//GLuint gElementBuffer;
	GLuint lightpassShaderProgram;
	int assetID;

};