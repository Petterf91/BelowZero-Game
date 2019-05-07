#include "MaterialLib.h"

MaterialLib::MaterialLib()
{
	//materials = std::vector<Material>();
}

MaterialLib::~MaterialLib()
{
	for (Material* material_ptr : materials)
		delete material_ptr;
	materials.clear();
}

void MaterialLib::addMaterial(GLuint otherShaderProgram)
{
	Material* newMat = new Material(otherShaderProgram);
	materials.push_back(newMat);
}

Material * MaterialLib::getMaterial(int key)
{
	return materials.at(key);
}
