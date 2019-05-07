#include "PlayerClass.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update()
{
}

void Player::setPlayerMesh()
{
}

void Player::setupCamera(GLFWwindow * window)
{
	camera.assetName = "CharacterMovement";
	camera.window = window;
	camera.cameraSpeed = 0.03f;
	camera.cameraPos = this->Position;
	camera.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	camera.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	camera.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.pitch = 0;
	camera.yaw = 0;
	camera.firstMouse = true;
	camera.lastX = 400;
	camera.lastY = 300;
	camera.xpos = 0;
	camera.ypos = 0;
	camera.mouseDisable = 0;
	camera.xoffset = 0;
	camera.yoffset = 0;
	camera.sensitivity = 0.002f;
}

