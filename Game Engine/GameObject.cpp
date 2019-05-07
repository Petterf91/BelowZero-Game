#include "GameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
	name = "";
    isActive = true;
    isRenderable = false;
    hasLight = false;
	isInteractable = false;
	isBurning = false;
	gameEnd = false;
	delayFlare = false;
	lighterEquipped = false;
	this->moveBelowTerrain = false;
	timeLimit = 0.f;
	timeAlive = 0.0f;
	timeToBurn = 10.f;
	modelMatrix = glm::mat4();
	objectID = ObjectType::ID::Stone_1;
	fireComponent = nullptr;
	lighterComponent = nullptr;
	flareComponent = nullptr;
	playerHitCounter = 0;
}

GameObject::~GameObject()
{
	deleteAllComponents();
	
	delete transform;

	for (bBox* bbox_ptr : bbox)
		delete bbox_ptr;
	bbox.clear();

	if (fireComponent != nullptr)
	{
		delete fireComponent;
	}
	if (lighterComponent != nullptr)
	{
		delete lighterComponent;
	}
	if (flareComponent != nullptr)
	{
		delete flareComponent;
	}
}

void GameObject::update(float deltaTime, float seconds)
{
	if (isBurning == true)
	{
		timeAlive += deltaTime;
		if (timeAlive >= timeToBurn)
		{
			burning.stopSound();
			isBurning = false;
			if (fireComponent != nullptr)
			{
				delete fireComponent;
				fireComponent = nullptr;
			}
		}
	}

	if (delayFlare)
	{
		if (flareComponent == nullptr)
		{
			flareComponent = new Light(*transform);
			flareComponent->lightType = 1;
			flareComponent->color = glm::vec4(0.9, 0, 0, 0.5);
			flareComponent->Linear = 0.0009;
			flareComponent->Quadratic = 0.0032;
			flareComponent->offset = 9;
			flareComponent->intensity = 3.0;
			flareComponent->isFlare = true;
		}
	}

	if (moveBelowTerrain)
		moveDown(deltaTime);

	for (Component* components_ptr : components)
	{
		Component& component = *components_ptr;
		component.update(deltaTime, seconds);
	}
}

void GameObject::processEvents(GLFWwindow * window, float deltaTime)
{
	for (Component* component_ptr : components)
	{
		component_ptr->processEvents(window, deltaTime);
	}
}

void GameObject::updateMaterialAndMeshFilterPointers() {
    bool matTest = false;
    bool meshTest = false;

    for (int i = 0; i < components.size(); i++) {
        Material* temp = getComponent<Material>();
        if (temp != nullptr) {
            materialComponent = temp;
            matTest = true;

        }
    }

    for (int i = 0; i < components.size(); i++) {
        MeshFilter* temp = getComponent<MeshFilter>();
        if (temp != nullptr) {
			meshFilterComponent = temp;
            meshTest = true;
        }
    }

    if (meshTest==true && matTest==true) {
        isRenderable = true;
    } else {
        isRenderable = false;
    }
}

void GameObject::updateHasLight() {
    bool foundLight = false;

    for (int i = 0; i < components.size(); i++) {
       
        Light* temp = getComponent<Light>();
        if (temp != nullptr) {
            foundLight = true;
			lightComponent = temp;
        }
    }

    if (foundLight == true) {
        hasLight = true;
    } else {
        hasLight = false;
    }
}

void GameObject::addComponent(Component* otherComponent)
{
	components.push_back(otherComponent);
	updateMaterialAndMeshFilterPointers();
	updateHasLight();
}

void GameObject::updateMeshFilter(int id)
{
	for (Component* component_ptr : components)
	{
		if (component_ptr->id == ComponentType::ID::MESHFILTER)
		{
			MeshFilter* meshFilter = static_cast<MeshFilter*>(component_ptr);
			if (meshFilter->typeID == id)
			{
				meshFilterComponent = meshFilter;
			}
		}
	}
}

void GameObject::deleteComponent(Component* otherComponent)
{
	////find component
	//int index = -1;
	//for (int i = 0; i < components.size(); i++)
	//{
	//	if (*otherComponent == *components[i]) {
	//		index = i;
	//	}
	//}
	//if (index != -1) {
	//	//delete components[index];
	//	components.erase(components.begin() + index);
	//}
 //   updateMaterialAndMeshFilterPointers();
}

void GameObject::deleteAllComponents()
{
	for (Component* component_ptr : components)
	{
		if(component_ptr->id != ComponentType::ID::MATERIAL)
			delete component_ptr;
	}
	components.clear();
}

const bool GameObject::getIsRenderable() {
    return isRenderable;
}

void GameObject::setIsRenderable(bool isRenderable)
{
	this->isRenderable = isRenderable;
}

void GameObject::setIsBurning(float timeToBurn)
{
	this->timeToBurn = timeToBurn;
	timeAlive = 0.0f;
	if (fireComponent == nullptr)
	{
		fireComponent = new Light(*transform);
		fireComponent->lightType = 1;
		fireComponent->color = glm::vec4(0.9f, 0.2f, 0, 0.5f);
		fireComponent->Linear = 0.0025;
		fireComponent->Quadratic = 0.0032f;
		fireComponent->offset = 6;
		fireComponent->intensity = 1.5f;
	}
	isBurning = true;
}

void GameObject::setGameEnd()
{
	gameEnd = true;
}

void GameObject::setLighterEquipped()
{
	if (lighterComponent == nullptr)
	{
		lighterComponent = new Light(*transform);
		lighterComponent->lightType = 1;
		lighterComponent->color = glm::vec4(0.9, 0.2, 0, 0.5);
		lighterComponent->Linear = 0.0009;
		lighterComponent->Quadratic = 0.0032;
		lighterComponent->offset = 3;
		lighterComponent->intensity = 0.5;
		lighterComponent->isLighter = true;
	}
}

const bool GameObject::getIsBurning()
{
	return this->isBurning;
}

void GameObject::resetLighterEquipped()
{
	if (!lighterEquipped)
	{
		if (lighterComponent != nullptr)
		{
			delete lighterComponent;
			lighterComponent = nullptr;
		}
	}
}

void GameObject::resetFlareLight()
{
	if (!gameEnd)
	{
		if (flareComponent != nullptr)
		{
			delete flareComponent;
			flareComponent = nullptr;
		}
	}
}

int GameObject::getEquippedItem()
{
	Player* tempPlayer = getPlayer();
	
	return tempPlayer->currentlyEquipedItem;
}

Player * GameObject::getPlayer()
{
	for (Component* component_ptr : components)
	{
		if (component_ptr->id == ComponentType::ID::PLAYER)
		{
			Player* player = static_cast<Player*>(component_ptr);
			return player;
		}
	}
	return false;
}

MainMenuScene * GameObject::getMenuScene()
{
	for (Component* component_ptr : components)
	{
		if (component_ptr->id == ComponentType::ID::MAINMENU)
		{
			MainMenuScene* mainMenu = static_cast<MainMenuScene*>(component_ptr);
			return mainMenu;
		}
	}
	return nullptr;
}

AI * GameObject::getAI()
{
	for (Component* component_ptr : components)
	{
		if (component_ptr->id == ComponentType::ID::AI)
		{
			AI* ai = static_cast<AI*>(component_ptr);
			return ai;
		}
	}
	return nullptr;
}

void GameObject::moveDown(float deltaTime)
{
	timeLimit += deltaTime;
	if (timeLimit > 20)
		isActive = false;

	if (!addedWallSound) {
		moveWallSound.addSound("meltWall.ogg");
		moveWallSound.setVolume(30.0f);
		addedWallSound = true;
	}
	if (!moveWallSound.isPlaying() && !wallSoundPlayed) {
		moveWallSound.playSound();
		wallSoundPlayed = true;
	}

	transform->position.y -= 2 * deltaTime;
}

glm::mat4 GameObject::getModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1), transform->position);
	//... 
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), transform->rotation);
	return this->modelMatrix;
}

glm::mat4 GameObject::getViewMatrix()
{
	return glm::lookAt(transform->position, transform->position + transform->forward, transform->up);
}

Terrain * GameObject::getTerrain()
{
	for (Component* component_ptr : components)
	{
		if (component_ptr->id == ComponentType::ID::TERRAIN)
		{
			Terrain* terrain = static_cast<Terrain*>(component_ptr);
			return terrain;
		}
	}
	return false;
}
