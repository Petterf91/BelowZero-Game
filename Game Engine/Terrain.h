#pragma once

#include <vector>
#include <string>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/glm.hpp"
#include "Component.h"
#include "Texture.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

struct TerrainVertex
{
	float x, y, z;
	float r, g, b;
};

class Terrain : public Component
{
private:
	float MAX_HEIGHT = 90;
	float MIN_HEIGHT = 0;
	int sizePerSide = 128;
	Texture HeightMap;
	Texture AlbedoMap;

	int offset = 8;

	int Length = 40;
	int Height = 40;

	float **Heights;
	glm::vec3 **Positions;

	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLuint EBO;
	GLuint PBO;
	GLuint FBO;

	bool foundAlbedo = true;
	bool foundNormal = false;
	bool foundSpecular = false;
	bool foundMetallic = false;
	bool foundAO = false;

public:
	Terrain();
	Terrain(const std::string & height, GLuint shader);
	~Terrain();



	std::vector<TerrainVertex> terrainVertices;

	std::vector<unsigned int> indices;

	short *heightMapIndexData;

	void setupVertexData();
	void setupBuffers(GLint gShaderProgram);
	//void calculateNormals(int triangleIndex);
	float getHeight(int x, int z);

	void loadHeighMap(const std::string & heightMap);

	void update(float deltaTime, float seconds);
	void processEvents(GLFWwindow *window, float deltaTime);
	GLuint getVBO();
	GLuint getEBO();
	GLuint getVAO();
	void bindVertexArray();
	void bindTextures(GLuint shader);

	float calculateY(float x, float z);
	bool calculateNormal(float x, float z);

	int distanceBetweenVertices();
	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};