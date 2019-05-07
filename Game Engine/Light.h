#pragma once
#include <GL/gl3w.h>  
//#include"Component.h"
#include "Transformable.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/glm.hpp"

class Light :public Transformable
{
public:
	Light(Transform& transform);
	~Light();

	glm::vec4 color;
	int lightType;
	float Linear;
	float Quadratic;
	float offset;
	float intensity;
	bool isLighter;
	bool isFlare;

	void update(float deltaTime, float seconds);
	void processEvents(GLFWwindow *window, float deltaTime);
	glm::vec3 getPosition() {
		return Transformable::transform.getLighterPosition();
	}

private:

};

