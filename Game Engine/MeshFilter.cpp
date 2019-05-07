#include "MeshFilter.h"

MeshFilter::MeshFilter():Component()
{
	assetName = "MeshFilter";

}

MeshFilter::MeshFilter(GLuint otherVertexBuffer, GLuint otherVertexAttribute, int otherVertexCount, int meshType, int typeID )
{
	assetName = "MeshFilter";
	gVertexBuffer = otherVertexBuffer;
	gVertexAttribute=otherVertexAttribute;
	vertexCount = otherVertexCount;
	this->meshType = meshType;
	this->typeID = typeID;
	Component::id = ComponentType::ID::MESHFILTER;
}

MeshFilter::~MeshFilter()
{
}

void MeshFilter::update(float deltaTime, float seconds)
{
}

void MeshFilter::processEvents(GLFWwindow * window, float deltaTime)
{

}

void MeshFilter::bindVertexArray()
{
	glBindVertexArray(gVertexAttribute);
}

