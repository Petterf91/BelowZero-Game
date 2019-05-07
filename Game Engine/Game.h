#pragma once
//...
// random comment

#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include "glfw\include\GLFW\glfw3.h"
#include <stdio.h>
#include <time.h>
#include <ctime>

/////////////

#include "TextureLib.h"
#include "ShaderProgramLib.h"
#include "GameScene.h"
#include "RenderManager.h"
//#include "Transform.h"
#include "MeshFilter.h"
//#include "CharacterMovement.h"
#include <string>
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glm\glm\glm.hpp"
//#include "Light.h"
#include <ctime>
#include "GeometryShaders.h"
#include "LightpassShaders.h"
#include "VFXFireShaders.h"
#include "VFXSnowShaders.h"

#include <chrono>
////////////

class Game
{
public:
	Game();
	~Game();

	void run(); // Main function that holds the main loop

private:
	GLFWwindow * window;
	GLFWmonitor** primary;
	const GLFWvidmode* mode;

	//LevelScene
	GameScene* gameScene;
	RenderManager* renderer;

	ShaderProgramLib shaderProgramLibrary;
	MaterialLib materialLibrary;
	TextureLib textureLibrary;
	MeshLib meshLibrary;
	
	//
	vector<string> meshName; // Filepaths for the importer?
	vector<GLuint> meshType;

	//Gamestate::ID stateOfGame; // EnumID.h, make this one global(?) 
	Gamestate::StateOfGame stateOfGame;

	void printCurrentState(Gamestate::ID stateOfGame); // Prints to console

	// state functions
	void runState();
	void menuState();
	void levelState();
	void exitState();

	char windowName[20];

	void initWindow();
	void initScene(Scene::ID sceneID);
	void clearScene();
	void initShaderProgramLib();
	void initInputOptions();

	void useShaderProgram();

	void addMeshName();
	void readMeshName(GameScene &scene);
	
	//...
	void processInput(GLFWwindow *window, float deltaTime, GameScene& scene);

	//Time Variables
	float deltaTime;
	float seconds;
	//
	bool meshesLoaded;
	bool fullscreen;
	bool texturesLoaded;

	int count;
	// bool in processInput for state swapping
	bool stateBool;
};

