#pragma once
#include <GL/gl3w.h>  
#include "stb_image.h"
#include <string>
#include<iostream>
#include"Component.h"
#include <vector>
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\glm.hpp"

class Material:public Component
{
public:
	Material();
	Material(GLuint otherShaderProgram);
	~Material();

	std::vector<GLuint> albedo;

	GLuint geometryShaderProgram;

	void update(float deltaTime, float seconds);
	void processEvents(GLFWwindow *window, float deltaTime);
	
	void bindTextures();
	void addAlbedo(int otherTexture);

private:
	int width, height, nrChannels = 0;
};

