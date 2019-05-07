#include "Game.h"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void mouse_enter_callback(GLFWwindow * window, int entered)
{
	if (entered)
		std::cout << "CURSOR::ENTER::WINDOW" << std::endl;
	else
		std::cout << "CURSOR::EXIT::WINDOW" << std::endl;
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "MOUSEBUTTON::LEFT::PRESS" << std::endl;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		std::cout << "MOUSEBUTTON::LEFT::RELEASE" << std::endl;
	}

	// entity.processMouseEvent(window, button, action);
	//
}

void Game::processInput(GLFWwindow *window, float deltaTime, GameScene& scene) //GameScene& scene
{
	if (glfwGetKey(window, GLFW_KEY_F5) && !fullscreen)
	{
		glfwSetWindowMonitor(window, primary[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mode->refreshRate);
		fullscreen = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_F5) && fullscreen)
	{
		glfwSetWindowMonitor(window, 0, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, mode->refreshRate);
		fullscreen = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && stateBool != true)
	{
		stateBool = true;
		if(stateOfGame.state == Gamestate::ID::RUN_LEVEL)
			stateOfGame.state = Gamestate::ID::CLEAR_LEVEL;
		else if (stateOfGame.state == Gamestate::ID::SHOW_MENU)
			stateOfGame.state = Gamestate::ID::CLEAR_MENU;
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE && stateBool != false)
	{
		stateBool = false;
	}
	//...
	scene.processEvents(window, deltaTime);
}

Game::Game() :
	shaderProgramLibrary(),
	windowName("Game Engine"),
	deltaTime(0), seconds(0),
	meshesLoaded(false), fullscreen(false), stateBool(false), texturesLoaded(false),
	count(0)
{
	this->gameScene = nullptr;
	this->renderer = nullptr;

	initWindow();
	initShaderProgramLib();
	addMeshName();
	stateOfGame.state = Gamestate::ID::INITIALIZE;
}

Game::~Game()
{
	if (gameScene != nullptr)
		delete gameScene;
	if (renderer != nullptr)
		delete renderer;
}

void Game::run()
{
	printCurrentState(stateOfGame.state);
	//Render
	auto startSeconds = chrono::high_resolution_clock::now();

	clock_t begin = clock();

	int initial_time = time(NULL);
	int final_time;
	int frameCount = 0;

	useShaderProgram();

	stateOfGame.state = Gamestate::ID::LOAD_MENU;
	printCurrentState(stateOfGame.state);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		clock_t end = clock();
		deltaTime = float(end - begin) / CLOCKS_PER_SEC;
		begin = end;

		/*	auto nowDeltaTime = chrono::high_resolution_clock::now();
		deltaTime = chrono::duration_cast<chrono::duration<float>>(nowDeltaTime - startDeltaTime).count() / 1000;
		startDeltaTime = nowDeltaTime;*/

		float secondsTime;
		auto nowSeconds = chrono::high_resolution_clock::now();
		seconds = (float)chrono::duration_cast<std::chrono::milliseconds>(nowSeconds - startSeconds).count();
		startSeconds = nowSeconds;

		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		runState();
		//...

		glfwSwapBuffers(window);

		frameCount++;
		final_time = time(NULL);
		if (final_time - initial_time > 0)
		{
			sprintf(windowName, "Game Engine FPS : %d", frameCount / (final_time - initial_time));
			glfwSetWindowTitle(window, windowName);
			frameCount = 0;
			initial_time = final_time;
		}

	}
	if (gameScene != nullptr)
		clearScene();

	glfwTerminate();
}

void Game::printCurrentState(Gamestate::ID stateOfGame)
{
	std::cout << "CURRENT_GAMESTATE:: " << stateOfGame << std::endl;
}

void Game::runState()
{
	//... Menu
	if (stateOfGame.state == Gamestate::ID::LOAD_MENU || stateOfGame.state == Gamestate::ID::SHOW_MENU || stateOfGame.state == Gamestate::ID::CLEAR_MENU)
	{
		menuState();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}
	//... Level
	else if (stateOfGame.state == Gamestate::ID::LOAD_LEVEL || stateOfGame.state == Gamestate::ID::RUN_LEVEL || stateOfGame.state == Gamestate::ID::CLEAR_LEVEL)
	{
		levelState();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glfwDisable(GLFW_MOUSE_CURSOR);
	}
	else if (stateOfGame.state == Gamestate::ID::CLOSE_GAME)
	{
		exitState();
	}
	//... 
	else
	{
		return;
	}
}

void Game::menuState()
{
	if (stateOfGame.state == Gamestate::ID::LOAD_MENU)
	{
		printCurrentState(stateOfGame.state);
		initScene(Scene::ID::MENU);
		renderer->createMainMenuBuffer();
		renderer->createButtonQuads();
		
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		stateOfGame.state = Gamestate::ID::SHOW_MENU;
		printCurrentState(stateOfGame.state);
	}
	else if (stateOfGame.state == Gamestate::ID::SHOW_MENU)
	{
		gameScene->update(deltaTime, seconds);
		processInput(window, deltaTime, *gameScene);
		renderer->setDeltaTime(deltaTime);
		renderer->setSeconds(seconds);
		renderer->renderMainMenu();
	}
	else if (stateOfGame.state == Gamestate::ID::CLEAR_MENU)
	{
		clearScene();
		stateOfGame.state = Gamestate::ID::LOAD_LEVEL;
	}
}

void Game::levelState()
{
	if (stateOfGame.state == Gamestate::ID::LOAD_LEVEL)
	{
		printCurrentState(stateOfGame.state);
		initScene(Scene::ID::LEVEL_1);
		renderer->createBuffers();
		//renderer->renderDepthQuadsForVisualization();
		stateOfGame.state = Gamestate::ID::RUN_LEVEL;
		printCurrentState(stateOfGame.state);
	}
	else if (stateOfGame.state == Gamestate::ID::RUN_LEVEL)
	{
		processInput(window, deltaTime, *gameScene);
		gameScene->update(deltaTime, seconds);
		renderer->setDeltaTime(deltaTime);
		renderer->setSeconds(seconds);
		renderer->Render();
	}
	else if (stateOfGame.state == Gamestate::ID::CLEAR_LEVEL)
	{
		printCurrentState(stateOfGame.state);
		clearScene();
		clearScene();
		stateOfGame.state = Gamestate::ID::LOAD_MENU;
	}
}

void Game::exitState()
{
	clearScene();
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void Game::initWindow()
{
	// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	primary = glfwGetMonitors(&count);
	mode = glfwGetVideoMode(primary[0]);
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowName, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	gl3wInit();
}

void Game::initScene(Scene::ID sceneID)
{
	// RenderManager probably needs a rework when loaded with scene. 
	gameScene = new GameScene(sceneID);
	renderer = new RenderManager(gameScene, window, &shaderProgramLibrary);
	//... Read OBJ and MTL File
	if (!meshesLoaded)
	{
		//Load the meshes once and store them.
		readMeshName(*gameScene);
		meshesLoaded = true;
	}
	//...
	gameScene->initScene(&meshLibrary, &materialLibrary, shaderProgramLibrary, gameScene->typeOfScene);
}

void Game::clearScene()
{
	if (gameScene != nullptr)
	{
		gameScene->clearGameObjects();
		delete gameScene;
		gameScene = nullptr;
	}
	if (renderer != nullptr)
	{
		delete renderer;
		renderer = nullptr;
	}
}

void Game::initShaderProgramLib()
{
	shaderProgramLibrary.addGeometryPassShaders();
	shaderProgramLibrary.addLightpassShaders();
	shaderProgramLibrary.addShadowMapShaders();
	shaderProgramLibrary.addUIShaders();
	shaderProgramLibrary.addVFXFireShaders();
	shaderProgramLibrary.addVFXSnowShaders();
	shaderProgramLibrary.addVFXFlareShaders();
	shaderProgramLibrary.addVFXLighterShaders();
	shaderProgramLibrary.addTerrainShaders();
	shaderProgramLibrary.addMainMenuShaders();
	shaderProgramLibrary.addRefractionShaders();
}

void Game::initInputOptions()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorEnterCallback(window, mouse_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
}

void Game::useShaderProgram()
{
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);
}

void Game::addMeshName()
{
	//Add file names to vector to load when reading mesh data. 
	//std::string meshLoader[] = { "Stone.leap", "Bucket.leap", "Stump.leap", "Tree.leap", "TreeWithSnow.leap", "Floor.leap" };
	std::string meshLoader[] = { 
		"Player_temp.leap",				//0
		"Bucket_Empty.leap",			//1
		"Stone_1.leap",					//2
		"Campfire.leap",				//3
		"Cliffside_1.leap",				//4
		"Cliffside_2.leap",				//5
		"Cliffside_3.leap",				//6
		"Cliffside_4.leap",				//7
		"Campfire_NoSnow.leap",			//8
		"WoodenSign.leap",				//9
		"PineTree.leap",				//10
		"PineTree_Snow.leap",			//11
		"BrokenTree.leap",				//12
		"BrokenTree_Snow.leap",			//13
		"IceTaps_1.leap",				//14
		"IceTaps_2.leap",				//15
		"WaterBottle.leap",				//16
		"Flag.leap",					//17
		"Can.leap",						//18
		"Can.leap", // Bucket_water		//19
		"Can.leap", // Bucket_snow		//20
		"Stump.leap",					//21
		"Tree.leap",					//22
		"Tree_Snow.leap",				//23
		"FlareGunBox.leap",				//24
		"Axe.leap",						//25
		"Jacket.leap",					//26
		"Stone_2.leap",					//27
		"Stone_3.leap",					//28
		"Stone_4.leap",					//29
		"Stone_5.leap",					//30
		"Cabin.leap",					//31
		"DeadTree.leap",				//32
		"EquipedAxe.leap",				//33
		"EquipedBucketEmpty.leap",		//34
		"EquipedBucketSnow.leap",		//35
		"EquipedBucketWater.leap",		//36
		"Tree_Small.leap",				//37
		"DeadTree_Small.leap",			//38
		"DeadTreeSnow.leap",			//39
		"DeadTreeSnow_Small.leap",		//40
		"Tree_Small_Snow.leap",			//41
		"FlareGun.leap",				//42
		"IceWall.leap",					//43
		"EquipedLighter.leap",			//44
		"EquipedWood.leap",				//45
		"EquipedCan.leap",				//46
		"EquipedBucketEmptyWarm.leap",	//47
		"EquipedBucketSnowWarm.leap",	//48
		"EquipedBucketWaterWarm.leap",	//49
		"EquipedAxeWarm.leap",			//50
		"EquipedCanWarm.leap",			//51
		"EquipedLighterWarm.leap",		//52
		"EquipedWoodWarm.leap",			//53
		"Rabbit.leap",					//54
		"Cabin_rotate.leap",			//55
		"BrokenTree_rotate.leap",		//56
		"Balloon.leap",					//57
		"MatchBox.leap",				//58
		"Wooden_Step.leap",				//59
		"Backpack.leap"					//60
	};

	for (int i = 0; i < sizeof(meshLoader) / sizeof(meshLoader[0]); i++)
	{
		meshName.push_back(meshLoader[i]);
	}
}

void Game::readMeshName(GameScene &scene)
{
	for (int i = 0; i < meshName.size(); i++)
	{
		meshLibrary.addMesh(meshName[i], shaderProgramLibrary.getShader<GeometryShaders>()->geometryShaderProgram);
	}
	if (!texturesLoaded)
	{
		materialLibrary.addMaterial(shaderProgramLibrary.getShader<GeometryShaders>()->geometryShaderProgram);
		textureLibrary.addAlbedo("Colors_2k.png");
		materialLibrary.getMaterial(0)->addAlbedo(textureLibrary.getAlbedo(0)->gTexture);
		texturesLoaded = true;
	}
}