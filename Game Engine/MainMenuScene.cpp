#include "MainMenuScene.h"

MainMenuScene::MainMenuScene()
{
	Component::id = ComponentType::ID::MAINMENU;

	this->mouseIsOverButton = false;
	this->buttonHasBeenClicked = false;
	this->buttonHasBeenReleased = false;
	this->pressed = false;
	this->buttonIsUpScaled1 = false;
	this->buttonIsUpScaled2 = false;
	this->buttonIsUpScaled3 = false;
	this->scaling1 = 1.0;
	this->scaling2 = 1.0;
	this->scaling3 = 1.0;
	this->whichButtonIsSelected = -1;

	this->fade = 0.0f;
	this->fadeTimer = 0.0f;
	this->fadeMenuBool = false;

	this->startButtonMinMax[0].x = 30;
	this->startButtonMinMax[0].y = 381;
	this->startButtonMinMax[1].x = 250;
	this->startButtonMinMax[1].y = 410;
	
	this->settingsButtonMinMax[0].x = 30;
	this->settingsButtonMinMax[0].y = 455;
	this->settingsButtonMinMax[1].x = 205;
	this->settingsButtonMinMax[1].y = 483;
	
	this->ExitButtonMinMax[0].x = 30;
	this->ExitButtonMinMax[0].y = 525;
	this->ExitButtonMinMax[1].x = 223;
	this->ExitButtonMinMax[1].y = 555;

	stbi_set_flip_vertically_on_load(true);

	this->whichBackgroundToLoad = randomizeBackgroundImage();
	loadLoadingTexture("LoadingTexture");

	if (whichBackgroundToLoad == 0)
	{
		loadBackgroundTexture("MainMenuLight");
		loadButtonTexture("StartGameButton", 1);
		loadButtonTexture("SettingsGameButton", 2);
		loadButtonTexture("ExitGameButton", 3);
	
	}
	if (whichBackgroundToLoad == 1)
	{
		loadBackgroundTexture("MainMenuDark");
		loadButtonTexture("StartGameButtonWhite", 1);
		loadButtonTexture("SettingsGameButtonWhite", 2);
		loadButtonTexture("ExitGameButtonWhite", 3);

	}
}

MainMenuScene::~MainMenuScene()
{
	deleteObjects();
}

void MainMenuScene::loadLoadingTexture(std::string loadingTextureName)
{
	std::string texturePNG = ".png";
	std::string filePath = loadingTextureName + texturePNG;
	int width, height, nrOfChannels;
	// ----------========== Equipment FrameBuffer ==========----------
	unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);
	glGenFramebuffers(1, &loadingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, loadingFBO);
	glGenTextures(1, &loadingTexture);
	glBindTexture(GL_TEXTURE_2D, loadingTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Failed to load Loading Texture from path" << std::endl;

	stbi_image_free(data);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, loadingTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Loading Framebuffer not complete!" << std::endl;

}

int MainMenuScene::randomizeBackgroundImage()
{
	srand(time(NULL));
	return (rand() % 2);
}

void MainMenuScene::loadBackgroundTexture(std::string backgroundTextureName)
{
	std::string texturePNG = ".png";
	std::string filePath = backgroundTextureName + texturePNG;
	int width, height, nrOfChannels;
	// ----------========== Equipment FrameBuffer ==========----------
	unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);
	glGenFramebuffers(1, &backgroundFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, backgroundFbo);
	glGenTextures(1, &backgroundTexture);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Failed to load Background Texture from path" << std::endl;

	stbi_image_free(data);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backgroundTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Background Framebuffer not complete!" << std::endl;
}

void MainMenuScene::loadButtonTexture(std::string buttonTextureName, int buttonNumber)
{
	
	if (buttonNumber == 1)
	{
		std::string texturePNG = ".png";
		std::string filePath = buttonTextureName + texturePNG;
		int width, height, nrOfChannels;

		// ----------========== Equipment FrameBuffer ==========----------
		unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

		glGenFramebuffers(1, &startButtonFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, startButtonFBO);

		glGenTextures(1, &startButtonTexture);
		glBindTexture(GL_TEXTURE_2D, startButtonTexture);

		stbi_set_flip_vertically_on_load(true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load Background Texture from path" << std::endl;

		stbi_image_free(data);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, startButtonTexture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Background Framebuffer not complete!" << std::endl;
	}
	if (buttonNumber == 2)
	{
		std::string texturePNG = ".png";
		std::string filePath = buttonTextureName + texturePNG;
		int width, height, nrOfChannels;

		// ----------========== Equipment FrameBuffer ==========----------
		unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

		glGenFramebuffers(1, &settingButtonFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, settingButtonFBO);

		glGenTextures(1, &settingsButtonTexture);
		glBindTexture(GL_TEXTURE_2D, settingsButtonTexture);

		stbi_set_flip_vertically_on_load(true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load Background Texture from path" << std::endl;

		stbi_image_free(data);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, settingsButtonTexture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Background Framebuffer not complete!" << std::endl;
	}
	if (buttonNumber == 3)
	{
		std::string texturePNG = ".png";
		std::string filePath = buttonTextureName + texturePNG;
		int width, height, nrOfChannels;

		// ----------========== Equipment FrameBuffer ==========----------
		unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

		glGenFramebuffers(1, &ExitButtonFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ExitButtonFBO);

		glGenTextures(1, &exitButtonTexture);
		glBindTexture(GL_TEXTURE_2D, exitButtonTexture);

		stbi_set_flip_vertically_on_load(true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load Background Texture from path" << std::endl;

		stbi_image_free(data);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, exitButtonTexture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Background Framebuffer not complete!" << std::endl;
	}
	
}

void MainMenuScene::renderFrameQuad(GLuint shader)
{
	if (VAO == 0)
	{
		unsigned int vertexPos;
		unsigned int uvPos;

		vertexPos = glGetAttribLocation(shader, "aPos");
		uvPos = glGetAttribLocation(shader, "aTexCoords");

		//create vertices
		QuadVert vertices[] = {
			// pos and normal and uv for each vertex
		{ 1.0,  1.0, 1.0f, 1.0f },
		{ 1.0, -1.0, 1.0f, 0.0f },
		{ -1.0, -1.0, 0.0f, 0.0f },
		{ -1.0,  1.0, 0.0f, 1.0f },
		};

		unsigned int indices[] = {
			0,1,3,
			1,2,3,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);

		if (vertexPos == -1) {
			OutputDebugStringA("Error, can't find aPos attribute in vertex shader\n");
			return;
		}

		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(QuadVert),
			BUFFER_OFFSET(0)
		);

		glEnableVertexAttribArray(1);

		if (uvPos == -1) {
			OutputDebugStringA("Error, cannt find aTexCoords attribute in vertex shader\n");
			return;
		}

		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(QuadVert),
			BUFFER_OFFSET(sizeof(float) * 2)
		);

		//ebo
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void MainMenuScene::fadeMenu(float deltaTime)
{
	fadeTimer += deltaTime;
	fade += deltaTime / 2;
	
	if (fadeTimer >= 2.0f)
	{
		printf("STATE OF GAME: %d", stateOfGame);
		stateOfGame.state = Gamestate::ID::CLEAR_MENU;
		printf("STATE OF GAME: %d", stateOfGame);
	}
}

void MainMenuScene::deleteObjects()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void MainMenuScene::update(float deltaTime, float seconds)
{

}

void MainMenuScene::processEvents(GLFWwindow * window, float deltaTime)
{
	glfwGetCursorPos(window, &xPos, &yPos);

	if (fadeMenuBool)
	{
		fadeMenu(deltaTime);
	}

	if (startButtonMinMax[0].x < xPos && xPos < startButtonMinMax[1].x && startButtonMinMax[0].y < yPos && yPos < startButtonMinMax[1].y)
	{
		//std::cout << "------------------- CURSOR IS INSIDE STARTBUTTONBOX -------------------" << std::endl;
		this->whichButtonIsSelected = 1;
		this->mouseIsOverButton = true;

		if (this->scaling1 <= 1.3f)
		{
			this->scaling1 += 0.02f;
			this->buttonIsUpScaled1 = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			this->buttonHasBeenClicked = true;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE )
		{
			this->buttonHasBeenReleased = true;
		}
	}
	else
	{
		if (this->buttonIsUpScaled1 == true)
		{
			if (scaling1 > 1.0f)
				scaling1 -= 0.01f;
			else if (scaling1 < 1.0f)
			{
				scaling1 = 1.0f;
				this->buttonIsUpScaled1 = false;
			}
		}
	}

	if (settingsButtonMinMax[0].x < xPos && xPos < settingsButtonMinMax[1].x && settingsButtonMinMax[0].y < yPos && yPos < settingsButtonMinMax[1].y)
	{
		//std::cout << "------------------- CURSOR IS INSIDE SETTINGSBUTTONBOX -------------------" << std::endl;
		this->whichButtonIsSelected = 2;
		this->mouseIsOverButton = true;

		if (this->scaling2 <= 1.3f)
		{
			this->scaling2 += 0.01f;
			this->buttonIsUpScaled2 = true;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			this->buttonHasBeenClicked = true;
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			{
				this->buttonHasBeenReleased = true;
			}
		}
	}
	else
	{
		if (this->buttonIsUpScaled2 == true)
		{
			if (scaling2 > 1.0f)
				scaling2 -= 0.01f;
			else if (scaling2 < 1.0f)
			{
				scaling2 = 1.0f;
				this->buttonIsUpScaled2 = false;
			}
		}
	}

	if (ExitButtonMinMax[0].x < xPos && xPos < ExitButtonMinMax[1].x && ExitButtonMinMax[0].y < yPos && yPos < ExitButtonMinMax[1].y)
	{
		//std::cout << "------------------- CURSOR IS INSIDE EXITBUTTONBOX -------------------" << std::endl;
		this->whichButtonIsSelected = 3;
		this->mouseIsOverButton = true;

		if (this->scaling3 <= 1.3f)
		{
			this->scaling3 += 0.01f;
			this->buttonIsUpScaled3 = true;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			this->buttonHasBeenClicked = true;
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			{
				this->buttonHasBeenReleased = true;
			}
		}
	}
	else
	{
		if (this->buttonIsUpScaled3 == true)
		{
			if (scaling3 > 1.0f)
				scaling3 -= 0.01f;
			else if (scaling3 < 1.0f)
			{
				scaling3 = 1.0f;
				this->buttonIsUpScaled3 = false;
			}
		}
	}

	if (this->buttonHasBeenClicked == true )//&& this->buttonHasBeenReleased == true)
	{
		 pressed = false;
		if (whichButtonIsSelected == 1 && pressed == false)
		{
			fadeMenuBool = true;
			pressed = true;
		}
		
		else if (whichButtonIsSelected == 2 && pressed == false)
		{
			//stateOfGame.state = Gamestate::ID::CLOSE_GAME;
			pressed = true;
		}
		
		else if (whichButtonIsSelected == 3 && pressed == false)
		{
				stateOfGame.state = Gamestate::ID::CLOSE_GAME;
				pressed = true;
		}

		this->mouseIsOverButton = false;
		this->buttonHasBeenClicked = false;
		this->buttonHasBeenReleased = false;
	}

}

