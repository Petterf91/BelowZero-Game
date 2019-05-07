#pragma once
#include "Transformable.h"
#include <iostream>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/glm.hpp"

#define MAX_DISTANCE 50
#define SPAWN_OFFSET 12
#define SPEED 3.0f
#define JUMP_HEIGHT 0.75f
#define JUMP_SPEED 2.0f

class AI :
	public Transformable
{
public:
	AI(Transform& transform);
	~AI();

	void update(float deltaTime, float seconds);
	void processEvents(GLFWwindow *window, float deltaTime);

	glm::vec2 getXY();
	void setCurrentHeight(float height);

	bool collision;
	glm::vec3 direction;
	//todo add function to rotate object towards the direction

private:
	glm::vec3 startPosition;
	glm::vec3 lastTarget;
	glm::vec3 target;
	glm::vec3 forward;

	float time;
	float bounce;
	float bounceTimer;
	int bounceCounter;
	bool wait;
	bool jumping;

	void setRotation();
	void setBounce(float deltaTime);
	void move(float deltaTime);
	void setNewTarget();
};

