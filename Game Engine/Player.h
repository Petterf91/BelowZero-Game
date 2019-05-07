#pragma once
#include<string>
#include <iostream>
#include "stb_image.h"
#include <GL/gl3w.h>
#include "Transformable.h"
#include "SoundMaster.h"
#include <time.h>

class Player : public Transformable
{
public:
	Player(Transform& transform);
	~Player();

	SoundMasterSFML SnowCrunch;
	SoundMasterSFML AmbientWind;
	SoundMasterSFML HeavySnow;
	SoundMasterSFML Swing;
	SoundMasterSFML pickUpSnowSound;
	SoundMasterSFML HitWAxe;
	SoundMasterSFML Eat;
	SoundMasterSFML Drink;
	SoundMasterSFML FlareSound;
	SoundMasterSFML HelicopterSound;
	SoundMasterSFML JacketSound;
	SoundMasterSFML wolf2;
	SoundMasterSFML wolf3;

	Gamestate::StateOfGame stateOfGame;

	bool click;

	float hp;
	float cold;
	float coldMeter;
	float water;
	float waterMeter;
	float food;
	float foodMeter;
	float coldTick;
	float waterTick;
	float foodTick;
	float damage;
	float fade;
	float winFade;
	float flareTimer;

	std::string equipedItem;

	bool startGame;
	bool textOnScreen;
	bool inInventory[5];
	bool win;

	int currentlyEquipedItem;
	int initializer;
	int textInitializer;
	int inventory[5];
	int maxAmountOfItems;
	int inventoryCount;

	std::string imageNames[5] = { "InventoryAxeIcon", "InventoryLighterIcon", "InventoryLogIcon", "InventoryFoodIcon", "InventoryBucketIconTexture" };
	std::string imagesCurrentlyInInventory[5] = { "EmptyImage", "EmptyImage", "EmptyImage", "EmptyImage", "EmptyImage" };

	unsigned int equipedFBO;
	unsigned int equipedTexture;
	unsigned int inventoryFBO[5];
	unsigned int inventoryTexture[5];
	unsigned int textFBO;
	unsigned int textTexture;

	bool flareS = false;

	void setCold(float value);
	void setWater(float value);
	void setFood(float value);

	void initiateInventoryTextures(std::string item);
	void addToInventory(int item);
	void equip(std::string item);
	void addImageToInventory(std::string item, int inventorySlot);
	bool checkInventory(std::string item);
	void addTextToScreen(std::string item);
	void swappingItem(float deltaTime);
	void dropItem();
	void useItem(GLFWwindow * window);

	void recieveTerrainInformation(float currentHeight, float frontV, float backV, float leftV, float rightV, float distance, int nrof);
	void setCurrentHeight(float height);
	void setIsWalkable(bool walkable);
	glm::vec2 setXZ();
	//Physics
	void update(float deltaTime, float seconds);
	void processEvents(GLFWwindow *window, float deltaTime);

	//glm::mat4 getViewMatrix()const;
	void swingTest();
	void eatFood();
	int interactionResponse(const ObjectType::ID id, bool & isAlive);
	int actionResponse(const ObjectType::ID id, bool & isAlive, int & counter);
	int collisionResponse(const ObjectType::ID);
	void heatResponse(float deltaTime);
	void takeDamange(float damage, float deltaTime);
	int getEquipedItem();
	const int getEquipedID();
	bool addClick;

	int axeSwing;
	float oldPitch;
	float oldYaw;
	float pickUp;
	float rotateSwing;
	bool swing;

	void wolfHowl(float nightTimer);

	int test;
	
private:
	bool isColliding = false;

	float foodTimer = 0.0f;
	float waterTimer = 0.0f;

	bool movingForward = false;
	bool movingBackwards = false;
	bool movingLeft = false;
	bool movingRight = false;

	bool isPressed;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::vec3 lastPos;
	glm::vec3 lastPosTemp;

	int equipedID;
	bool swapItem;
	bool pullDown;
	int equipItem;
	bool jacket;
	int bucketContent;
	bool pickUpSnow;
	bool warning;
	float textureTimer;

	float coldResistance;

	bool firstMouse;
	float lastX, lastY;
	double xpos, ypos;
	int mouseDisable;
	float xoffset;
	float yoffset;
	float sensitivity;
	float pitch;
	float yaw;

	float time = 0.0f;
	float textTimer = 0.0f;
	float meltTimer = 0.0f;

	float jumpSpeed = 7.64f;
	float cameraSpeed = 7.06f;
	float fallSpeed = 9.82f;
	//--------=====Jumping=====-----------

	bool jumpReady = true;
	bool inAir = false;
	float timeInAir = 0.3f;
	bool gravity = false;

	//------=====Walking=====----
	bool isWalking = false;

	//Terrain
	float previousY;
	float currentY;
	float frontVertexHeight;
	float backVertexHeight;
	float leftVertexHeight;
	float rightVertexHeight;
	float distanceToNextVertex;

	int vertexLength;

	void equipItemMesh();
	void textWarnings();
	void findY();
	void swingAxe(float deltaTime);


	int howlNumber = 2;
	float howlTime = 0.0f;
	float temphowlTime = 0.0f;
	bool howlReset = true;
};
