#include "Material.h"

Material::Material():Component()
{
	assetName = "Material";
	Component::id = ComponentType::ID::MATERIAL;
}

Material::Material(GLuint otherShaderProgram)
{
	Component::id = ComponentType::ID::MATERIAL;
	assetName = "Material";
	geometryShaderProgram = otherShaderProgram;
}

Material::~Material()
{

}

void Material::update(float deltaTime, float seconds)
{
}

void Material::processEvents(GLFWwindow * window, float deltaTime)
{
}

void Material::bindTextures()
{
	glUniform1i(glGetUniformLocation(geometryShaderProgram, "gAlbedoIn"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo[0]);
}

void Material::addAlbedo(int otherTexture)
{
	albedo.push_back(otherTexture);
}
