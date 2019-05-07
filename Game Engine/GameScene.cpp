#include "GameScene.h"



GameScene::GameScene(Scene::ID typeOfScene) :
	camerasInScene(0), lightsInScene(0),
	addObject(false)
{
	this->typeOfScene = typeOfScene;
	this->directionalLight = nullptr;
	this->randomLevel = 0;
}

GameScene::~GameScene()
{
	inZone.clear();
	clearGameObjects();
	
}

void GameScene::clearGameObjects()
{
	for (GameObject* gameObject_ptr : gameObjects)												//!Fix this so it deletes everything
		delete gameObject_ptr;

	gameObjects.clear();

}

void GameScene::update(float deltaTime, float seconds)
{
	lightCheck();

	if (addObject)
	{
		for (unsigned int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->getPlayer() != nullptr)
			{
				addGameObject(gameObjects[i]->transform->position, 3);
				addObject = false;
				break;
			}
		}
	}
	// Update player position, AI collision and Active gameObjects
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{

		if (gameObjects[i]->getPlayer() != nullptr)
		{
			// Update player zone;
			setZone(*gameObjects[i], false);

			if (gameObjects[i]->getIsBurning())
				gameObjects[i]->getPlayer()->takeDamange(5.f, deltaTime);

			for (int j = 0; j < gameObjects.size(); j++)
			{
				glm::vec2 UVS = gameObjects[i]->getPlayer()->setXZ();
				float u = UVS.x;
				float v = UVS.y;
				if (gameObjects[j]->getTerrain() != nullptr)
				{
					gameObjects[i]->getPlayer()->setIsWalkable(gameObjects[j]->getTerrain()->calculateNormal(u, v));
					gameObjects[i]->getPlayer()->setCurrentHeight(gameObjects[j]->getTerrain()->calculateY(u, v));
					break;
				}
			}
		}
		//gameObjects[i]->update(deltaTime, seconds);
		//collisionTest(*gameObjects[i], deltaTime);
		if (gameObjects[i]->isActive == false)
		{
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
			setZone(*gameObjects[0], true);
		}

		if (gameObjects[i]->getAI() != nullptr)
		{
			aiCollisionTest(*gameObjects[i]);
		}
	}
	// Update inZone GameObjects
	for (unsigned int i = 0; i < inZone.size(); i++)
	{
		inZone[i]->update(deltaTime, seconds);
		collisionTest(*inZone[i], deltaTime);
	}
}

void GameScene::processEvents(GLFWwindow * window, float deltaTime)
{
	for (int i = 0; i < inZone.size(); i++)
	{
		interactionTest(*inZone[i], window);
	}
	// Process Events in player or menuScene object
	for (int i = 0; gameObjects.size(); i++)
	{
		if (gameObjects[i]->getPlayer() != nullptr || gameObjects[i]->getMenuScene() != nullptr)
		{
			gameObjects[i]->processEvents(window, deltaTime);
			break;
		}
	}
	addEquipment();
	addNewObjectTest(window);
}

void GameScene::initScene(MeshLib * meshLibrary, MaterialLib * matertialLibrary, ShaderProgramLib & shader, Scene::ID typeOfScene)
{
	material = matertialLibrary;
	meshes = meshLibrary;

	if (typeOfScene == Scene::ID::LEVEL_1)
	{
		// Camera - (modify position with level file?)
		addPlayer(*meshLibrary, *matertialLibrary);
		// Terrain
		std::string heightMap = "heightMap.jpg";
		addTerrain(heightMap, shader.getShader<TerrainShaders>()->TerrainShaderProgram);
		// Read from level file and add level objects to scene

		LeapLevel * level;

		//Randomizer
		/*srand(time(NULL));
		randomLevel = rand() % 4;

		switch (randomLevel)
		{
		case 0:
			level = new LeapLevel("Lvl0.leap");
			break;
		case 1:
			level = new LeapLevel("Lvl1.leap");
			break;
		case 2:
			level = new LeapLevel("Lvl2.leap");
			break;
		case 3:
			level = new LeapLevel("Lvl3.leap");
			break;
		default:
			break;
		}*/
		level = new LeapLevel("Petter.leap");

		addLevelObjects(*meshLibrary, *matertialLibrary, level);
		delete level;

		makeObjectsInteractable();

	}
	else if (typeOfScene == Scene::ID::MENU)
	{
		// Put menu specific scene in here. 
		//...
		addMainMenu();

		std::string heightMap = "test1234.jpg";
		addTerrain(heightMap, shader.getShader<TerrainShaders>()->TerrainShaderProgram);

		LeapLevel* level = new LeapLevel("MenuLvl.leap");
		addLevelObjects(*meshLibrary, *matertialLibrary, level);
		delete level;
	}
	else
	{
		std::cout << "GAMESCENE::initScene() - Scene::ID : not valid" << std::endl;
	}
}

void GameScene::addAI(MeshLib & meshLibrary, MaterialLib & materialLibrary, LeapLevel level)
{
	int key = 54;

	//Create AI object
	GameObject* AiObject = new GameObject();
	AiObject->name = "AI ";
	AiObject->transform->position = gameObjects[0]->transform->position;
	AiObject->transform->position.x += 10;
	AiObject->transform->position.z -= 25;
	//
	MeshFilter* meshFilter = new MeshFilter(
		meshLibrary.getMesh(key)->gVertexBuffer,
		meshLibrary.getMesh(key)->gVertexAttribute,
		meshLibrary.getMesh(key)->leapMesh->getVertexCount(),
		meshLibrary.getMesh(key)->meshType, 1);
	AiObject->addComponent(meshFilter);
	AiObject->addComponent(materialLibrary.getMaterial(0));
	//Add AI component
	AI* ai = new AI(*AiObject->transform);
	AiObject->addComponent(ai);
	//...
	AiObject->setIsRenderable(true);
	//...
	for (int j = 0; j < meshLibrary.getMesh(1)->leapMesh->boundingBoxes.size(); j++)
	{
		bBox* box = new bBox();
		//add center
		box->center.x = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->center[0];
		box->center.y = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->center[1];
		box->center.z = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->center[2];
		//add max vector
		box->vMax.x = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->maxVector[0];
		box->vMax.y = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->maxVector[1];
		box->vMax.z = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->maxVector[2];
		//add min vector
		box->vMin.x = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->minVector[0];
		box->vMin.y = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->minVector[1];
		box->vMin.z = meshLibrary.getMesh(key)->leapMesh->boundingBoxes[j]->minVector[2];
		//push into gameobject
		AiObject->bbox.push_back(box);
	}

	//Add to scene
	gameObjects.push_back(AiObject);
}

void GameScene::addLight(glm::vec3 transform, int lightType)
{
	//Create light object
	GameObject* lightObject = new GameObject();
	lightObject->name = "Light " + std::to_string(lightsInScene);
	lightObject->transform->position = transform;
	//Add light component
	Light* light = new Light(*lightObject->transform);
	light->lightType = lightType;
	lightObject->addComponent(light);
	// Set directionalLightPointer
	directionalLight = lightObject;
	//Add to scene
	gameObjects.push_back(lightObject);
	lightsInScene++;
}

void GameScene::addPlayer(MeshLib & meshLibrary, MaterialLib& materialLibrary)
{
	//Create player object
	GameObject* playerObject = new GameObject();
	playerObject->transform->position = glm::vec3(0.f, 0.f, 0.f);
	playerObject->objectID = ObjectType::ID::Player;
	playerObject->name = "Player " + camerasInScene;
	//Add player component
	Player* player = new Player(*playerObject->transform);
	playerObject->addComponent(player);

	//Add Equipment Meshes
	int equipmenID[] = { 33, 34, 44, 45, 46, 35, 36, 47, 48, 49, 50, 51, 52, 53 };

	for (int i = 0; i < sizeof(equipmenID) / sizeof(equipmenID[0]); i++)
	{
		MeshFilter* meshFilter = new MeshFilter(
			meshLibrary.getMesh(equipmenID[i])->gVertexBuffer,
			meshLibrary.getMesh(equipmenID[i])->gVertexAttribute,
			meshLibrary.getMesh(equipmenID[i])->leapMesh->getVertexCount(),
			meshLibrary.getMesh(equipmenID[i])->meshType,
			equipmenID[i]);
		playerObject->addComponent(meshFilter);
		playerObject->addComponent(materialLibrary.getMaterial(0));
	}
	//Add to scene
	gameObjects.push_back(playerObject);
	camerasInScene++;
}

void GameScene::addEquipment()
{
	if( gameObjects[0]->getPlayer() != nullptr )
		if (gameObjects[0]->getPlayer()->getEquipedID() != -1)
			gameObjects[0]->updateMeshFilter(gameObjects[0]->getPlayer()->getEquipedID());
}

void GameScene::addLevelObjects(MeshLib & meshLibrary, MaterialLib& materialLibrary, LeapLevel* level)
{
	//Find terrain object
	Terrain* terrain;
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->getTerrain() != nullptr)
		{
			terrain = gameObject_ptr->getTerrain();
			break;
		}
	}
	//Create game objects from level file
	for (int i = 0; i < level->levelObjects.size(); i++)
	{
		std::cout << "LEVELOBJECT::ID::" << level->levelObjects[i]->id << std::endl;
		if (level->levelObjects[i]->id == ObjectType::ID::Player)
		{
			for (GameObject* gameObject_ptr : gameObjects)
			{
				// Find player object
				if (gameObject_ptr->getPlayer() != nullptr)
				{
					//Add new mesh component to player with data from mesh library
					MeshFilter* meshFilter = new MeshFilter(
						meshLibrary.getMesh(level->levelObjects[i]->id)->gVertexBuffer,
						meshLibrary.getMesh(level->levelObjects[i]->id)->gVertexAttribute,
						meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->getVertexCount(),
						meshLibrary.getMesh(level->levelObjects[i]->id)->meshType,
						level->levelObjects[i]->id);
					gameObject_ptr->addComponent(meshFilter);
					//Set player object position in world
					gameObject_ptr->transform->position = glm::vec3(level->levelObjects[i]->x, level->levelObjects[i]->y, level->levelObjects[i]->z);
					gameObject_ptr->transform->rotation = glm::vec3(level->levelObjects[i]->rotationX, level->levelObjects[i]->rotationY, level->levelObjects[i]->rotationZ);
					//Calculate new world Y-position from height map and update value
					float newPositionY = terrain->calculateY(gameObject_ptr->transform->position.x, gameObject_ptr->transform->position.z);
					gameObject_ptr->transform->position.y = newPositionY;
					//Add material to gameObject from materialLibrary
					gameObject_ptr->addComponent(materialLibrary.getMaterial(0));
					//Set customAttribute ID from Enum.H
					gameObject_ptr->objectID = (ObjectType::ID)level->levelObjects[i]->id;
					//Set customAttribute interactable - should probably be removed since player isn't interactable
					if ((int)meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->customMayaAttribute->meshType == 1 || 
						(int)meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->customMayaAttribute->meshType == 2 || 
						(int)meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->customMayaAttribute->meshType == 3)
						gameObject_ptr->isInteractable = true;
					//Add BBox from leapmesh to player object
					for (int j = 0; j < meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes.size(); j++)
					{
						bBox* box = new bBox();
						//add center
						box->center.x = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->center[0];
						box->center.y = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->center[1];
						box->center.z = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->center[2];

						//add max vector
						box->vMax.x = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->maxVector[0];
						box->vMax.y = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->maxVector[1];
						box->vMax.z = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->maxVector[2];

						/*glm::mat4 tempMatrix = glm::mat4(1);
						tempMatrix = glm::translate(tempMatrix, box->center);
						tempMatrix = glm::rotate(tempMatrix, glm::radians(level->levelObjects[i]->rotationY), glm::vec3(0, 1, 0));*/

						//add min vector
						box->vMin.x = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->minVector[0];
						box->vMin.y = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->minVector[1];
						box->vMin.z = meshLibrary.getMesh(0)->leapMesh->boundingBoxes[i]->minVector[2];

						//push into gameobject
						gameObject_ptr->bbox.push_back(box);
					}
					// set zone
					setZone(*gameObject_ptr, true);
					//break loop
					break;
				}
			}
		}
		else if (level->levelObjects[i]->id == ObjectType::ID::Rabbit)
		{
			//Create new mesh object
			GameObject* aiObject = new GameObject();
			aiObject->name = "AI " + std::to_string(i);
			//Set mesh object position in world
			aiObject->transform->position = glm::vec3(level->levelObjects[i]->x, level->levelObjects[i]->y, level->levelObjects[i]->z);
			aiObject->transform->rotation = glm::vec3(level->levelObjects[i]->rotationX, level->levelObjects[i]->rotationY, level->levelObjects[i]->rotationZ);
			//Calculate new world Y-position from height map and update value
			float newPositionY = terrain->calculateY(aiObject->transform->position.x, aiObject->transform->position.z);
			aiObject->transform->position.y = newPositionY;
			//Add new mesh component with data from mesh library
			MeshFilter* meshFilter = new MeshFilter(
				meshLibrary.getMesh(level->levelObjects[i]->id)->gVertexBuffer,
				meshLibrary.getMesh(level->levelObjects[i]->id)->gVertexAttribute,
				meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->getVertexCount(),
				meshLibrary.getMesh(level->levelObjects[i]->id)->meshType,
				level->levelObjects[i]->id);
			aiObject->addComponent(meshFilter);
			//Add material to gameObject from materialLibrary
			aiObject->addComponent(materialLibrary.getMaterial(0));
			//Add AI component to gameObject
			AI* ai = new AI(*aiObject->transform);
			aiObject->addComponent(ai);
			//Set customAttribute ID from Enum.H
			aiObject->objectID = (ObjectType::ID)level->levelObjects[i]->id;
			//Set customAttribute interactable
			if ((int)meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->customMayaAttribute->meshType == 1)
				aiObject->isInteractable = true;
			//Add BBox from leapmesh to gameObject
			for (int j = 0; j < meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes.size(); j++)
			{
				bBox* box = new bBox();
				//add center
				box->center.x = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->center[0];
				box->center.y = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->center[1];
				box->center.z = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->center[2];
				//add max vector
				box->vMax.x = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->maxVector[0];
				box->vMax.y = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->maxVector[1];
				box->vMax.z = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->maxVector[2];
				//add min vector
				box->vMin.x = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->minVector[0];
				box->vMin.y = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->minVector[1];
				box->vMin.z = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->minVector[2];
				//push into gameobject
				aiObject->bbox.push_back(box);
			}
			// set zone
			setZone(*aiObject, true);
			//Add to scene
			gameObjects.push_back(aiObject);
		}
		else 
		{
			//Create new mesh object
			GameObject* meshObject = new GameObject();
			meshObject->name = "Mesh " + std::to_string(i);
			//Set mesh object position in world
			meshObject->transform->position = glm::vec3(level->levelObjects[i]->x, level->levelObjects[i]->y, level->levelObjects[i]->z);
			meshObject->transform->rotation = glm::vec3(level->levelObjects[i]->rotationX, level->levelObjects[i]->rotationY, level->levelObjects[i]->rotationZ);
			//Calculate new world Y-position from height map and update value
		

			if (level->levelObjects[i]->id == ObjectType::ID::Cliffside_1
				|| level->levelObjects[i]->id == ObjectType::ID::Cliffside_2
				|| level->levelObjects[i]->id == ObjectType::ID::Cliffside_3
				|| level->levelObjects[i]->id == ObjectType::ID::Cliffside_4)
			{
				meshObject->transform->position.y = -10.0f;
			}
			//if (level->levelObjects[i]->id == ObjectType::ID::Wooden_Step)
			//	meshObject->transform->position.y = level->levelObjects[i]->y -8.0f;
			else
			{
				float newPositionY = terrain->calculateY(meshObject->transform->position.x, meshObject->transform->position.z);
				meshObject->transform->position.y = newPositionY;
			}
			//Add new mesh component with data from mesh library
			MeshFilter* meshFilter = new MeshFilter(
				meshLibrary.getMesh(level->levelObjects[i]->id)->gVertexBuffer,
				meshLibrary.getMesh(level->levelObjects[i]->id)->gVertexAttribute,
				meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->getVertexCount(),
				meshLibrary.getMesh(level->levelObjects[i]->id)->meshType,
				level->levelObjects[i]->id);
			meshObject->addComponent(meshFilter);
			//Add material to gameObject from materialLibrary
			meshObject->addComponent(materialLibrary.getMaterial(0));
			//Set customAttribute ID from Enum.H
			meshObject->objectID = (ObjectType::ID)level->levelObjects[i]->id;
			//Set customAttribute interactable
			if ((int)meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->customMayaAttribute->meshType == 1)
				meshObject->isInteractable = true;
			//Add BBox from leapmesh to gameObject
			for (int j = 0; j < meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes.size(); j++)
			{
				bBox* box = new bBox();
				//add center
				box->center.x = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->center[0];
				box->center.y = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->center[1];
				box->center.z = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->center[2];
				//add max vector
				box->vMax.x = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->maxVector[0];
				box->vMax.y = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->maxVector[1];
				box->vMax.z = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->maxVector[2];
				//add min vector
				box->vMin.x = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->minVector[0];
				box->vMin.y = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->minVector[1];
				box->vMin.z = meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->boundingBoxes[j]->minVector[2];
				//push into gameobject
				meshObject->bbox.push_back(box);
			}
			// set zone
			setZone(*meshObject, true);
			//Add to scene
			gameObjects.push_back(meshObject);
		}
	}
}

void GameScene::addTerrain(const std::string & heightMap, GLuint shader)
{
	//Create terrain object
	GameObject* terrainObject = new GameObject();
	terrainObject->name = "Terrain";
	//Add terrain component
	Terrain * terrain = new Terrain(heightMap, shader);
	terrainObject->addComponent(terrain);
	//Add to scene
	gameObjects.push_back(terrainObject);
}

void GameScene::addMainMenu()
{
	GameObject* MainMenuObject = new GameObject();
	MainMenuScene* mainMenuScene = new MainMenuScene();
	MainMenuObject->name = "MainMenu";
	MainMenuObject->addComponent(mainMenuScene);
	MainMenuObject->setIsRenderable(true);
	
	
	gameObjects.push_back(MainMenuObject);
}

void GameScene::checkInteractionResponse(GameObject & other, int objectID)
{
	if (objectID == (int)ObjectType::ID::Campfire)
	{
		other.setIsBurning(60.0f);
	}
}

void GameScene::interactionTest(GameObject & other, GLFWwindow * window)
{
	for (GameObject* gameObject_ptr : inZone)
	{
		if (gameObject_ptr->getPlayer() != nullptr)
		{
			if (gameObject_ptr->getPlayer()->click == false && (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS))
			{
				if (zoneTest(gameObject_ptr, &other))
				{
					float distance = glm::distance(other.transform->position, gameObject_ptr->transform->position);

					if (distance < 10 && other.isInteractable == true)
					{

						gameObject_ptr->getPlayer()->click = true;
						RayData ray = Ray::getWorldRay(
							SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f,
							gameObject_ptr->getViewMatrix(),
							SCREEN_WIDTH, SCREEN_HEIGHT,
							gameObject_ptr->transform->position);

						for (int i = 0; i < other.bbox.size(); i++)
						{
							if (Intersection::rayBoxTest(ray, *other.bbox[i], other.getModelMatrix()))
							{
								if (gameObject_ptr->getPlayer()->interactionResponse(other.objectID, other.isActive) == ObjectType::ID::FlareGun)
								{
									other.setGameEnd();
								}
							}
						}
					}
				}
			}

			if (gameObject_ptr->getPlayer()->click == false && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
			{
				if (zoneTest(gameObject_ptr, &other))
				{
					float distance = glm::distance(other.transform->position, gameObject_ptr->transform->position);
					if (distance < 10 && other.isInteractable == true)
					{

						gameObject_ptr->getPlayer()->click = true;
						RayData ray = Ray::getWorldRay(
							SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f,
							gameObject_ptr->getViewMatrix(),
							SCREEN_WIDTH, SCREEN_HEIGHT,
							gameObject_ptr->transform->position);

						for (int i = 0; i < other.bbox.size(); i++)
						{
							if (Intersection::rayBoxTest(ray, *other.bbox[i], other.getModelMatrix()))
							{
								if (gameObject_ptr->getPlayer()->actionResponse(other.objectID, other.isActive, other.playerHitCounter) == ObjectType::ID::Campfire)
								{
									other.setIsBurning(60.0f);
									meltIceWall(other);
								}
							}
						}
					}
				}
			}
			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) && gameObject_ptr->getPlayer()->click == true)
				gameObject_ptr->getPlayer()->click = false;
			break;
		}
	}
}

void GameScene::collisionTest(GameObject & other, float deltaTime)
{
	for (GameObject* gameObject_ptr : inZone)
	{
		if (gameObject_ptr->getPlayer() != nullptr && other.objectID != ObjectType::ID::Player)
		{
			float distance = glm::distance(other.transform->position, gameObject_ptr->transform->position);
			if (distance < 25)
			{
				for (int i = 0; i < gameObject_ptr->bbox.size(); i++)
				{
					for (int j = 0; j < other.bbox.size(); j++)
					{
						if (Intersection::collisionTest(*gameObject_ptr->bbox[i], gameObject_ptr->transform->position, *other.bbox[j], other.transform->position))
						{
							Intersection::collisionResponse(*gameObject_ptr->bbox[i], *gameObject_ptr->transform, *other.bbox[j], other.transform->position);
							
							if (other.getIsBurning())
							{
								gameObject_ptr->setIsBurning(10);
							}
						}
					}
				}
			}
			if (distance < 15 && other.getIsBurning())
			{
				gameObject_ptr->getPlayer()->heatResponse(deltaTime);
			}
		}
	}
}

void GameScene::aiCollisionTest(GameObject & other)
{
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (zoneTest(gameObject_ptr, &other))
		{
			float distance = glm::distance(other.transform->position, gameObject_ptr->transform->position);
			if (distance < 25 && gameObject_ptr->getAI() == nullptr)
			{
				for (int i = 0; i < other.bbox.size(); i++)
				{
					for (int j = 0; j < gameObject_ptr->bbox.size(); j++)
					{
						if (Intersection::collisionTest(*other.bbox[i], other.transform->position + (other.getAI()->direction * 4.f), *gameObject_ptr->bbox[j], gameObject_ptr->transform->position))
						{
							other.getAI()->collision = true;
							if (Intersection::collisionTest(*other.bbox[i], other.transform->position, *gameObject_ptr->bbox[j], gameObject_ptr->transform->position))
								Intersection::collisionResponse(*other.bbox[i], *other.transform, *gameObject_ptr->bbox[j], gameObject_ptr->transform->position);
							break;
						}
						else
						{
							other.getAI()->collision = false;
						}
					}
				}
			}
			break;
		}
	}
}

void GameScene::lightCheck()
{
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->getPlayer() != nullptr)
		{
			if (gameObject_ptr->getEquippedItem() == 1)
			{
				gameObject_ptr->lighterEquipped = true;
				gameObject_ptr->setLighterEquipped();
			}
			else
			{
				gameObject_ptr->lighterEquipped = false;
				gameObject_ptr->resetLighterEquipped();
			}

			break;
		}
	}
}

void GameScene::makeObjectsInteractable()
{
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->objectID == ObjectType::ID::Campfire)
		{
			gameObject_ptr->isInteractable = 1;
		}
		if (gameObject_ptr->objectID == ObjectType::ID::Rabbit)
		{
			gameObject_ptr->isInteractable = 1;
		}
	}
}

void GameScene::addGameObject(const glm::vec3 position, const int key)
{
	Terrain* terrain;
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->getTerrain() != nullptr)
		{
			terrain = gameObject_ptr->getTerrain();
			break;
		}
	}

	// Direction vector of the camera
	glm::vec3 offsetVector;
	// Distance to place object infront of the position vector
	float distance = 7.5f;

	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->getPlayer() != nullptr)
		{
			if (key == (int)ObjectType::ID::Campfire)
			{
				gameObject_ptr->getPlayer()->dropItem();
				gameObject_ptr->getPlayer()->equip("EmptyImage");
			}

			offsetVector = gameObject_ptr->transform->forward;
			break;
		}
	}

	//Create new mesh object
	GameObject* meshObject = new GameObject();
	//Set mesh object position in world
	meshObject->transform->position = position;
	// Add offset values to position
	meshObject->transform->position += offsetVector * distance;
	//Calculate new world Y-position from height map and update value
	float newPositionY = terrain->calculateY(meshObject->transform->position.x, meshObject->transform->position.z);
	meshObject->transform->position.y = newPositionY;
	//Add new mesh component with data from mesh library
	MeshFilter* meshFilter = new MeshFilter(
		meshes->getMesh(key)->gVertexBuffer,
		meshes->getMesh(key)->gVertexAttribute,
		meshes->getMesh(key)->leapMesh->getVertexCount(),
		meshes->getMesh(key)->leapMesh->customMayaAttribute->meshType,
		0);
	meshObject->addComponent(meshFilter);
	meshObject->meshFilterComponent = meshFilter;

	//Add material to gameObject from materialLibrary
	meshObject->addComponent(material->getMaterial(0));
	//Set customAttribute ID from Enum.H
	meshObject->objectID = (ObjectType::ID)key;
	//Set customAttribute interactable
//	if ((int)meshLibrary.getMesh(level->levelObjects[i]->id)->leapMesh->customMayaAttribute->meshType == 1)
	meshObject->isInteractable = true;
	//Add BBox from leapmesh to gameObject

	for (int i = 0; i < meshes->getMesh(key)->leapMesh->boundingBoxes.size(); i++)
	{
		bBox* box = new bBox();
		//add center
		box->center.x = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->center[0];
		box->center.y = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->center[1];
		box->center.z = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->center[2];
		//add max vector
		box->vMax.x = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->maxVector[0];
		box->vMax.y = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->maxVector[1];
		box->vMax.z = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->maxVector[2];
		//add min vector
		box->vMin.x = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->minVector[0];
		box->vMin.y = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->minVector[1];
		box->vMin.z = meshes->getMesh(i)->leapMesh->boundingBoxes[i]->minVector[2];
		//push into gameobject
		meshObject->bbox.push_back(box);
	}
	meshObject->setIsRenderable(true);
	// Set Zone
	setZone(*meshObject, false);
	//Add to scene
	gameObjects.push_back(meshObject);
	//...
	//Check if we created a new campfire and makes stuff happen if true
	if (gameObjects[gameObjects.size() - 1]->objectID == ObjectType::ID::Campfire)
	{
		setBurningByDistance(5.f, *gameObjects[gameObjects.size() - 1]);
		//push to inZone
		inZone.push_back(gameObjects[gameObjects.size() - 1]);
	}
}

void GameScene::addNewObjectTest(GLFWwindow * window)
{
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->getPlayer() != nullptr)
		{
			if (gameObject_ptr->getPlayer()->addClick == false && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) && gameObject_ptr->getPlayer()->currentlyEquipedItem == 2)
			{
				gameObject_ptr->getPlayer()->addClick = true;
				if (gameObject_ptr->getPlayer()->getEquipedItem() == 2)
				{
					addObject = true;
				}
			}
			if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) && gameObject_ptr->getPlayer()->addClick == true)
				gameObject_ptr->getPlayer()->addClick = false;
			break;
		}
	}

}

void GameScene::setBurningByDistance(const float distance, GameObject & other)
{
	//todo add timer to start burning
	//? add objects to a list and check timers each loop if size is bigger than 0;
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->objectID == ObjectType::ID::Campfire &&
			other.objectID == ObjectType::ID::Campfire &&
			glm::distance(gameObject_ptr->transform->position, other.transform->position) < distance)
		{
			if(gameObject_ptr->getIsBurning())
				other.setIsBurning(60);
		}
	}
}

void GameScene::meltIceWall(GameObject & other)
{
	for (GameObject* gameObject_ptr : gameObjects)
	{
		if (gameObject_ptr->objectID == ObjectType::ID::IceWall && other.objectID == ObjectType::ID::Campfire)
		{
			// Check distance between campfire and icewall
			// todo start timer in game object or similar.
			// todo move icewall -y until it's under map and then delete it
			if (glm::distance(gameObject_ptr->transform->position, other.transform->position) < 40)
			{
				gameObject_ptr->moveBelowTerrain = true;
			}
		}
	}
}

void GameScene::setZone(GameObject & other, const bool forceUpdate)
{
	glm::ivec2 previousZone = other.zone.zoneXY;

	float X = other.transform->position.x;
	float Z = other.transform->position.z;

	other.zone.zoneXY.x = (int)(X / 128.0f) + 0.5f;
	other.zone.zoneXY.y = (int)(Z / 128.0f) + 0.5f;

	if (previousZone != other.zone.zoneXY && other.getPlayer() != nullptr)
	{
		inZone.clear();

		//..
		std::cout << other.name << " " << "new zone: " << other.zone.zoneXY.x << " " << other.zone.zoneXY.y << std::endl;
		for (GameObject* gameObject_ptr : gameObjects)
		{
			if (zoneTest(&other, gameObject_ptr))
			{
				inZone.push_back(gameObject_ptr);
			}

		}
	}
	else if (forceUpdate)
	{
		//force inZone update
		inZone.clear();
		for (GameObject* gameObject_ptr : gameObjects)
		{
			if (zoneTest(gameObjects[0], gameObject_ptr))
			{
				inZone.push_back(gameObject_ptr);
			}
		}
	}
}

bool GameScene::zoneTest(GameObject* target1, GameObject* target2)
{
	if (target1->zone.zoneXY == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(0, 1)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(0, -1)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(1, 0)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(-1, 0)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(1, -1)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(1, 1)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(-1, -1)) == target2->zone.zoneXY ||
		(target1->zone.zoneXY + glm::ivec2	(-1, 1)) == target2->zone.zoneXY
		)
	{
		return true;
	}
	return false;
}


