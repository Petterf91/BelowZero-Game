#pragma once
#include "Mesh.h"
#include "GameObject.h"
#include "CharacterMovement.h"
#include <GL/gl3w.h> 
#include <Windows.h>
#include "glfw/include/GLFW/glfw3.h"

class Player : public GameObject
{
private:

	Mesh playerMesh;
	float gravity;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 10.0f);

public:
	Player();
	Player(glm::vec3 Position);
	~Player();

	CharacterMovement camera;

	void Update();
	void setPlayerMesh();
	void setupCamera(GLFWwindow * window);



};