#pragma once

#include "Component.h"
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>

#include <GL/gl3w.h>  
#include "ShaderBase.h"
#include "Mesh.h"
#include "stb_image.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

struct QuadVert
{
	float x, y, u, v;
};

class MainMenuScene : public Component
{
public:
	MainMenuScene();
	~MainMenuScene();

	void loadLoadingTexture(std::string loadingTextureName);
	int randomizeBackgroundImage();

	void loadBackgroundTexture(std::string backgroundTextureName);
	void loadButtonTexture(std::string buttonTextureName, int buttonNumber);
	void renderFrameQuad(GLuint shader);
	void fadeMenu(float deltaTime);

	void deleteObjects();

	void update(float deltaTime, float seconds);
	void processEvents(GLFWwindow *window, float deltaTime);

	GLuint backgroundTexture;
	GLuint loadingTexture;
	GLuint startButtonTexture;
	GLuint settingsButtonTexture;
	GLuint exitButtonTexture;

	unsigned int whichButtonIsSelected;
	unsigned int mainMenuShaderProgram;
	
	unsigned int backgroundFbo;
	unsigned int loadingFBO;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	
	unsigned int startButtonFBO;
	unsigned int settingButtonFBO;
	unsigned int ExitButtonFBO;
	
	glm::vec2 startButtonMinMax[2];
	glm::vec2 settingsButtonMinMax[2];
	glm::vec2 ExitButtonMinMax[2];

	int buttonTransformations;
	float scaling1;
	float scaling2;
	float scaling3;
	float fadeTimer;
	float fade;

	bool mouseIsOverButton;
	bool buttonHasBeenClicked;
	bool buttonHasBeenReleased;
	bool pressed;
	bool buttonIsUpScaled1;
	bool buttonIsUpScaled2;
	bool buttonIsUpScaled3;
	bool fadeMenuBool;

	double xPos;
	double yPos;

	int whichBackgroundToLoad;

private:
	Gamestate::StateOfGame stateOfGame;
};