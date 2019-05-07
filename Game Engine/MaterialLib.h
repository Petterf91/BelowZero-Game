#pragma once
#include<vector>
#include"Material.h"
#include<string>
#include "stb_image.h"

class MaterialLib
{
public:
	MaterialLib();

	~MaterialLib();

	void addMaterial(GLuint otherShaderProgram);
	Material* getMaterial(int key);
private:
	std::vector<Material*> materials;
};