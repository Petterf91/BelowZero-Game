#include "Player.h"

Player::Player(Transform& transform) : Transformable(transform)
{
	Component::id = ComponentType::ID::PLAYER;

	click = false;
	addClick = false;
	this->coldResistance = 1.0f;
	this->currentlyEquipedItem = -1;
	this->equipedID = -1;
	this->pickUp = -1;
	this->rotateSwing = 0;
	this->swapItem = false;
	this->pullDown = false;
	this->jacket = false;
	this->bucketContent = 0;
	this->swing = false;
	this->axeSwing = 0;
	this->pickUpSnow = false;
	this->warning = false;
	this->textureTimer = 2;

	this->hp = 80;
	this->cold = 100;
	this->coldMeter = 0;
	this->water = 100;
	this->waterMeter = 0;
	this->food = 100;
	this->foodMeter = 0;
	this->coldTick = 0.5;
	this->waterTick = 0.5;
	this->foodTick = 0.5;
	this->damage = 0;
	this->initializer = 0;
	this->textInitializer = 0;
	this->inventoryCount = 0;
	this->maxAmountOfItems = 5;
	this->fade = 0.9f;
	this->winFade = 0;
	this->flareTimer = 0;
	this->startGame = true;
	this->isPressed = false;
	this->win = false;
	for (int i = 0; i < 5; i++)
		this->inventory[i] = 0;
	this->inventoryCount = 0;
	/**/
	assetName = "CharacterMovement";
	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0;
	yaw = 0;
	oldPitch = 0;
	oldYaw = 0;
	firstMouse = true;
	lastX = 400;
	lastY = 300;
	xpos = 0;
	ypos = 0;
	mouseDisable = 0;
	xoffset = 0;
	yoffset = 0;
	sensitivity = 0.002f;

	for (int i = 0; i < sizeof(inInventory); i++)
	{
		inInventory[i] = false;
	}

	equip("EmptyImage");
	for (int i = 0; i < 5; i++)
		initiateInventoryTextures("EmptyImage");
	addTextToScreen("EmptyImageTexture");

	SnowCrunch.addSound("Snow.wav");
	SnowCrunch.setVolume(30.0f);

	AmbientWind.addSound("Wind.wav");
	AmbientWind.playSound();
	AmbientWind.loop(true);

	HeavySnow.addSound("HeavyWind.ogg");
	HeavySnow.loop(true);
	HeavySnow.setVolume(0.0f);

	Swing.addSound("woosh.wav");
	pickUpSnowSound.addSound("PickUpSnow.ogg");
	pickUpSnowSound.setVolume(35.0f);
	HitWAxe.addSound("AxeHit.ogg");
	HitWAxe.setVolume(35.0f);
	Eat.addSound("Eating.ogg");
	Eat.setVolume(35.0f);
	Drink.addSound("DrinkWater.wav");
	Drink.setVolume(35.0f);
	FlareSound.addSound("FlareSound.ogg");
	FlareSound.setVolume(100.0f);
	HelicopterSound.addSound("HelicopterSound.wav");

	JacketSound.addSound("JacketSwoosh.wav");
	wolf2.addSound("WolfHowl2.ogg");
	wolf3.addSound("WolfHowl3.wav");
	wolf2.setVolume(10.0f);
	wolf3.setVolume(10.0f);
}

Player::~Player()
{

}

void Player::setCold(float value)
{
	this->cold += value;

	if (this->cold > 100)
		this->cold = 100;
}

void Player::setWater(float value)
{
	this->water += value;

	if (this->water > 100)
		this->water = 100;
}

void Player::setFood(float value)
{
	this->food += value;

	if (this->food > 100)
		this->food = 100;
}

void Player::initiateInventoryTextures(std::string item)
{
	std::string texturePNG = "Texture.png";
	std::string filePath = item + texturePNG;
	int width, height, nrOfChannels;

	// ----------========== Equipment FrameBuffer ==========----------
	unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

	glGenFramebuffers(1, &inventoryFBO[inventoryCount]);
	glBindFramebuffer(GL_FRAMEBUFFER, inventoryFBO[inventoryCount]);

	glGenTextures(1, &inventoryTexture[inventoryCount]);
	glBindTexture(GL_TEXTURE_2D, inventoryTexture[inventoryCount]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load Inventory Texture from path" << std::endl;
	}

	stbi_image_free(data);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inventoryTexture[inventoryCount], 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Inventory Framebuffer not complete!" << std::endl;
	inventoryCount++;
	if (inventoryCount == 5)
		inventoryCount = 0;
}

void Player::addToInventory(int item)
{
	this->inventory[this->inventoryCount] = item;

	if (inventoryCount < 5)
		inventoryCount++;
}

void Player::equip(std::string item)
{
	std::string texturePNG = "Texture.png";
	std::string filePath = item + texturePNG;
	int width, height, nrOfChannels;

	// ----------========== Equipment FrameBuffer ==========----------
	unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

	if (this->initializer == 0)
		glGenFramebuffers(1, &equipedFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, equipedFBO);

	if (this->initializer == 0)
	{
		glGenTextures(1, &equipedTexture);
		this->initializer = 1;
	}
	glBindTexture(GL_TEXTURE_2D, equipedTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Failed to load Equiped Texture from path" << std::endl;

	stbi_image_free(data);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, equipedTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Equiped Framebuffer not complete!" << std::endl;

}

void Player::addImageToInventory(std::string item, int inventorySlot)
{
	if (checkInventory(item) && item != "EmptyImage")
	{
		if (this->textTimer >= textureTimer || this->textOnScreen == false && currentlyEquipedItem != 1)
		{
			std::cout << "Item already exists in players inventory" << std::endl;
			addTextToScreen("Text-ItemAlreadyEquipped");
		}
	}
	else
	{
		this->currentlyEquipedItem = inventorySlot;
		std::string texturePNG = "Texture.png";
		std::string filePath = item + texturePNG;
		int width, height, nrOfChannels;

		// ----------========== Equipment FrameBuffer ==========----------
		unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, inventoryFBO[inventorySlot]);

		glBindTexture(GL_TEXTURE_2D, inventoryTexture[inventorySlot]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load Inventory Texture from path" << std::endl;
		}

		stbi_image_free(data);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inventoryTexture[inventorySlot], 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Inventory Framebuffer not complete!" << std::endl;

		this->imagesCurrentlyInInventory[inventorySlot] = item;
		inventoryCount++;
	}
}

bool Player::checkInventory(std::string item)
{
	bool check = false; // Item does not already exist in inventory

	for (int i = 0; i < maxAmountOfItems && check == false; i++)
	{
		if (item.c_str() == imagesCurrentlyInInventory[i])
			check = true; // Item already exists in players inventory
	}

	return check;
}

void Player::addTextToScreen(std::string item)
{
	if (item == "Text-ItemAlreadyEquipped")
		textureTimer = 3.0;
	else
		textureTimer = 10.0;

	std::string texturePNG = ".png";
	std::string filePath = item + texturePNG;
	int width, height, nrOfChannels;

	// ----------========== Equipment FrameBuffer ==========----------
	unsigned char * data = stbi_load(filePath.c_str(), &width, &height, &nrOfChannels, 0);

	if (this->textInitializer == 0)
		glGenFramebuffers(1, &textFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, textFBO);

	if (this->textInitializer == 0)
	{
		glGenTextures(1, &textTexture);
		this->textInitializer = 1;
	}
	glBindTexture(GL_TEXTURE_2D, textTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load Text Texture from path" << std::endl;
	}

	stbi_image_free(data);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Text Framebuffer not complete!" << std::endl;

	if (item == "EmptyImageTexture")
		this->textOnScreen = false;
	else
		this->textOnScreen = true;

	this->textTimer = 0.0;

}

void Player::swappingItem(float deltaTime)
{
	if (swapItem)
	{
		if (!pullDown)
		{
			if (pickUp >= 0)
				swapItem = false;
			else
				pickUp += deltaTime * 3;
		}
		else
		{
			if (pickUp <= -1)
			{
				pullDown = false;
				this->equipedID = equipItem;
			}
			else
				pickUp -= deltaTime * 4;
		}
	}
}

void Player::dropItem()
{
	pickUp = -1;
	inInventory[this->currentlyEquipedItem] = false;
}

void Player::useItem(GLFWwindow * window)
{
	// Use Axe
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && (this->currentlyEquipedItem == 0) && (pickUp >= 0) && !swing && !isPressed)
	{
		swing = true;
		axeSwing = 0;
		isPressed = true;
		if (!Swing.isPlaying())
		{
			Swing.playSound();
		}
	}
	// Use Can
	else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && this->currentlyEquipedItem == 3 && pickUp >= 0 && inInventory[3] == true && foodTimer > 1.0f && !isPressed)
	{
		if (!Eat.isPlaying())
			Eat.playSound();

		food += 50;
		if (food >= 100)
			food = 100;
		addImageToInventory("EmptyImage", 3);
		dropItem();
		equip("EmptyImage");
	}

	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && this->currentlyEquipedItem == 4 && pickUp >= 0)
	{
		if (bucketContent == 0)
		{
			waterTimer = 0.0f;
			bucketContent = 1;
			equipItemMesh();
			pickUpSnow = true;
			swapItem = true;
			pullDown = true;
			if (!pickUpSnowSound.isPlaying())
				pickUpSnowSound.playSound();
		}
		if (bucketContent == 2)
		{
			bucketContent = 0;
			water += 50;
			equipItemMesh();
			swapItem = true;
			pullDown = true;
			if (water >= 100)
				water = 100;

			if (!Drink.isPlaying())
				Drink.playSound();
		}

	}
}

void Player::recieveTerrainInformation(float currentHeight, float frontV, float backV, float leftV, float rightV, float distance, int nrof)
{
	this->currentY = currentHeight;
	this->frontVertexHeight = frontV;
	this->backVertexHeight = backV;
	this->leftVertexHeight = leftV;
	this->rightVertexHeight = rightV;
	this->distanceToNextVertex = distance;
	this->vertexLength = nrof;

}

void Player::setCurrentHeight(float height)
{
	if (isColliding)
		previousY = currentY;

	currentY = height + 7;
}

void Player::setIsWalkable(bool walkable)
{
	if (walkable)
		isColliding = false;
	else
		isColliding = true;
}

glm::vec2 Player::setXZ()
{
	float u = Transformable::transform.position.x;
	float v = Transformable::transform.position.z;

	return glm::vec2(u, v);
}

void Player::update(float deltaTime, float seconds)
{
	//update velocity
	//Transformable::transform.velocity = Transformable::transform.forward * deltaTime;
	//...
	textWarnings();

	if (isColliding && lastPos.y < currentY)
	{
		Transformable::transform.position = lastPos;
		currentY = lastPos.y;
	}
	if (time <= timeInAir && inAir == true)
	{
		glm::vec3 jumpdir = Transformable::transform.up;

		Transformable::transform.position += jumpSpeed * jumpdir * deltaTime;
		Transformable::transform.velocity = Transformable::transform.up * deltaTime * cameraSpeed;
	}
	else
		inAir = false;


	if (inAir == false && Transformable::transform.position.y <= currentY)
	{
		gravity = false;
		jumpReady = true;
	}
	else
		gravity = true;


	if (gravity == true && inAir == false)
		Transformable::transform.position -= fallSpeed * Transformable::transform.up  * deltaTime;


	if (Transformable::transform.position.y <= currentY - 0.0001)
		Transformable::transform.position.y = currentY;

	if (isWalking == true)
	{
		if (!SnowCrunch.isPlaying())
			SnowCrunch.playSound();
	}
	else
	{
		SnowCrunch.stopSound();
	}
	//=======================-------------Leons Test Sak------------==================

	if (Transformable::transform.position.y > 47.0f)
	{
		if (!HeavySnow.isPlaying())
			HeavySnow.playSound();

		float mixVar = 89.0f - 47.0f;
		float volumeVar = Transformable::transform.position.y - 47.0f;
		float volume = glm::mix(0.0f, 100.0f, volumeVar / mixVar);
		volume *= 0.7f;
		HeavySnow.setVolume(volume);


		this->cold = this->cold - (this->coldTick * 8.0f * deltaTime * this->coldResistance);
		
		if (!jacket)
		{
			//this->damage = this->coldMeter + this->waterMeter + this->foodMeter;
			this->hp = this->hp - (this->coldTick * 8.0f * deltaTime);

		}
	}
	else if (HeavySnow.isPlaying())
	{
		HeavySnow.stopSound();
	}


	swappingItem(deltaTime);

	if (swing)
		swingAxe(deltaTime);
	else
		rotateSwing = 0.0f;

	if (win)
	{
		if (flareTimer >= 4.0f && !HelicopterSound.isPlaying())
		{

			HelicopterSound.playSound();
		}

		if (flareTimer >= 4.0f && flareTimer <= 10.0f)
		{

			float tempTimer = flareTimer - 4.0f;
			float volume = glm::mix(0.0f, 100.0f, tempTimer / 4.0f);
			HelicopterSound.setVolume(volume);
		}

		if (flareTimer >= 10.0f)
			stateOfGame.state = Gamestate::ID::CLEAR_LEVEL;
	}
	else if (fade >= 2.0f)
	{
		stateOfGame.state = Gamestate::ID::CLEAR_LEVEL;
	}

	float tempSeconds = seconds / 1000.0f;
	time += tempSeconds;


	this->foodTimer += tempSeconds;
	this->waterTimer += tempSeconds;

	this->textTimer += tempSeconds;
	if (this->textTimer >= textureTimer && this->textOnScreen == true)
	{
		addTextToScreen("EmptyImageTexture");
	}

	// LOOSING HP
	if (this->cold < 20)
		this->coldMeter = 0.5;

	if (this->water < 20)
		this->waterMeter = 0.5;

	if (this->food < 20)
		this->foodMeter = 0.5;

	// REGENERATION
	if (this->cold > 80 && this->water > 80 && this->food > 80)
	{
		this->coldMeter = -0.5;
		this->foodMeter = -0.5;
		this->waterMeter = -0.5;
	}
	// NEUTRAL
	else if ((this->cold < 80 || this->water < 80 || this->food < 80) && this->cold > 20 && this->water > 20 && this->food > 20)
	{
		this->coldMeter = 0;
		this->foodMeter = 0;
		this->waterMeter = 0;
	}

	// DECREASE OVER TIME
	if (this->cold <= 100)
		this->cold = this->cold - (this->coldTick * deltaTime * this->coldResistance);

	if (this->water <= 100)
		this->water = this->water - (this->waterTick * deltaTime);

	if (this->food <= 100)
		this->food = this->food - (this->foodTick * deltaTime);

	this->damage = this->coldMeter + this->waterMeter + this->foodMeter;

	// HP DMG / REG
	if (this->hp <= 100 && this->hp > 0)
		this->hp = this->hp - (this->damage * deltaTime);
	else if (this->hp > 100)
		this->hp = 100;

	// SPAWN & GAME OVER FADE
	if (this->startGame && this->fade > 0)
	{
		this->fade -= 0.005f;
		if (this->fade <= 0)
			this->startGame = false;
	}

	if (this->hp <= 0 && this->fade < 2)
	{
		this->fade += deltaTime;
		addTextToScreen("YouDiedTexture");
	}

	//Winning

	if (this->win == true)
	{
		this->flareTimer += deltaTime;
		if (flareTimer >= 2.0f)
		{
			if (!FlareSound.isPlaying() && flareS == false)
			{
				FlareSound.playSound();
				flareS = true;
			}
			this->winFade += deltaTime / 8.0f;
		}
	}

	// Text Fade
	//else if (this->textOnScreen == false)
	//this->textFade = 1.0;

	if (cold <= 0.0f)
	{
		cold = 0.0f;
	}
	if (water <= 0.0f)
	{
		water = 0.0f;
	}
	if (food <= 0.0f)
	{
		food = 0.0f;
	}
}

void Player::processEvents(GLFWwindow * window, float deltaTime)
{

	isWalking = false;
	movingForward = false;
	movingBackwards = false;
	movingLeft = false;
	movingRight = false;

	lastPosTemp = Transformable::transform.position;

	useItem(window);

	//Equipment and Stats

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !isPressed && this->currentlyEquipedItem != 0)
	{
		if (inInventory[0] == true)
		{
			equip("AxeIcon");
			this->currentlyEquipedItem = 0;
			equipItemMesh();
			isPressed = true;
			swapItem = true;
			pullDown = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !isPressed && this->currentlyEquipedItem != 1)
	{
		if (inInventory[1] == true)
		{
			equip("LighterIcon");
			this->currentlyEquipedItem = 1;
			equipItemMesh();
			isPressed = true;
			swapItem = true;
			pullDown = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !isPressed && this->currentlyEquipedItem != 2)
	{
		if (inInventory[2] == true)
		{
			equip("WoodIcon");
			this->currentlyEquipedItem = 2;
			equipItemMesh();
			isPressed = true;
			swapItem = true;
			pullDown = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !isPressed && this->currentlyEquipedItem != 3)
	{
		if (inInventory[3] == true)
		{
			equip("FoodIcon");
			this->currentlyEquipedItem = 3;
			equipItemMesh();
			inInventory[3] = true;
			isPressed = true;
			swapItem = true;
			pullDown = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !isPressed && this->currentlyEquipedItem != 4)
	{
		if (inInventory[4] == true)
		{
			equip("BucketIcon");
			this->currentlyEquipedItem = 4;
			equipItemMesh();
			isPressed = true;
			swapItem = true;
			pullDown = true;
		}
	}
	else if (  glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		isPressed = false;
	}

	//... Mouse Movement
	glfwGetCursorPos(window, &xpos, &ypos);

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw = xoffset;
	pitch = yoffset;

	glm::mat4 matrix = glm::mat4(1);


	oldYaw = oldYaw - yaw;
	oldPitch = oldPitch + pitch;

	matrix = glm::rotate(matrix, oldYaw, Transformable::transform.up);
	glm::vec4 right = glm::vec4(matrix[0][0], matrix[1][0], matrix[2][0], 0);
	Transformable::transform.right = right;

	if (oldPitch < 1.48f && oldPitch > -1.48f)
	{
		matrix = glm::rotate(matrix, oldPitch, Transformable::transform.right);
		glm::vec4 forward = glm::vec4(matrix[0][2], matrix[1][2], matrix[2][2], 0);
		Transformable::transform.forward = forward;
	}
	else if (oldPitch > 1.48f)
	{
		oldPitch = 1.48f;
	}
	else if (oldPitch < -1.48f)
	{
		oldPitch = -1.48f;
	}


	if (firstMouse) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	xoffset = lastX - (float)xpos;
	yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;



	//... WASD Movement
	glm::vec3 direction = glm::vec3(0);
	bool shift = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		shift = true;
		SnowCrunch.setPitch(1.5f);
		setWater(-0.005f);
	}
	else
		SnowCrunch.setPitch(1.0f);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (jumpReady == true)
			isWalking = true;

		movingForward = true;


		direction += Transformable::transform.forward;
		if (shift == true)
			Transformable::transform.position += cameraSpeed * (Transformable::transform.forward * 1.8f) * deltaTime;
		else
			Transformable::transform.position += cameraSpeed * Transformable::transform.forward * deltaTime;
		//velocity
		Transformable::transform.velocity = Transformable::transform.forward * deltaTime * cameraSpeed;

	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (jumpReady == true)
			isWalking = true;

		movingBackwards = true;

		direction -= Transformable::transform.forward;
		Transformable::transform.position -= cameraSpeed * Transformable::transform.forward * deltaTime;
		//velocity
		Transformable::transform.velocity = Transformable::transform.forward * deltaTime * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (jumpReady == true)
			isWalking = true;

		movingLeft = true;

		direction += Transformable::transform.right;
		Transformable::transform.position += Transformable::transform.right * cameraSpeed * deltaTime;
		//velocity
		Transformable::transform.velocity = Transformable::transform.right * deltaTime * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (jumpReady == true)
			isWalking = true;

		movingRight = true;

		direction += Transformable::transform.right;
		Transformable::transform.position -= Transformable::transform.right * cameraSpeed * deltaTime;
		//velocity
		Transformable::transform.velocity = Transformable::transform.right * deltaTime * cameraSpeed;
	}

	//... Jump mechanic
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && inAir == false && jumpReady == true && !isColliding)
	{

		inAir = true;
		time = 0.0f;
		jumpReady = false;
	}


	if (movingForward || movingBackwards || movingLeft || movingRight)
		lastPos = lastPosTemp;
}
void  Player::swingTest()
{
}

void Player::eatFood()
{
}

int Player::interactionResponse(const ObjectType::ID id, bool & isAlive)
{
	// Check ObjectType ID
	// Set isAlive to false if you want to delete the interacted item from the world.


	if (id == ObjectType::ID::Axe)
	{
		if (inInventory[0] == false)
		{
			equip("Axe");
			this->currentlyEquipedItem = 0;
			equipItemMesh();
			addImageToInventory("InventoryAxeIcon", 0);
			addTextToScreen("TipTextureAxe");
			inInventory[0] = true;
			isPressed = true;
			swapItem = true;
			pullDown = true;
			isAlive = false;
		}
		else
		{
			addTextToScreen("Text-ItemAlreadyEquipped");
		}
			
	}
	else if (id == ObjectType::ID::MatchBox)
	{
		if (inInventory[1] == false)
		{
			equip("LighterIcon");
			this->currentlyEquipedItem = 1;
			equipItemMesh();
			addImageToInventory("InventoryLighterIcon", 1);
			addTextToScreen("TipTextureLighter");
			inInventory[1] = true;
			isPressed = true;
			swapItem = true;
			pullDown = true;
			isAlive = false;
		}
		else
			addTextToScreen("Text-ItemAlreadyEquipped");
	}
	else if (id == ObjectType::ID::Can)
	{
		if (inInventory[3] == false)
		{
			this->foodTimer = 0.0f;
			equip("FoodIcon");
			this->currentlyEquipedItem = 3;
			equipItemMesh();
			addImageToInventory("InventoryFoodIcon", 3);
			addTextToScreen("TipTextureCan");
			inInventory[3] = true;
			isPressed = true;
			swapItem = true;
			pullDown = true;
			isAlive = false;
		}
		else
			addTextToScreen("Text-ItemAlreadyEquipped");
	}
	else if (id == ObjectType::ID::Bucket_Empty)
	{
		if (inInventory[4] == false)
		{
			equip("BucketIcon");
			this->currentlyEquipedItem = 4;
			equipItemMesh();
			addImageToInventory("InventoryBucketIcon", 4);
			addTextToScreen("TipTextureBucket");
			inInventory[4] = true;
			isPressed = true;
			swapItem = true;
			pullDown = true;
			isAlive = false;
		}
		else
			addTextToScreen("Text-ItemAlreadyEquipped");
	}
	else if (id == ObjectType::ID::Jacket)
	{
		jacket = true;
		equipItemMesh();
		addTextToScreen("TipJacketTexture");
		swapItem = true;
		pullDown = true;
		isAlive = false;
		this->coldResistance = 0.3f;
		if (!JacketSound.isPlaying())
			JacketSound.playSound();
	}
	else if (id == ObjectType::ID::FlareGunBox)
	{
		addTextToScreen("MissingFlareGunTexture");
	}

	else if (id == ObjectType::ID::FlareGun)
	{
		this->win = true;
		return 42;
	}

	return -1;
}

int Player::actionResponse(const ObjectType::ID id, bool & isAlive, int & counter)
{
	if ((id == ObjectType::ID::BrokenTree || id == ObjectType::ID::BrokenTree_Snow || id == ObjectType::ID::DeadTree
		|| id == ObjectType::ID::DeadTreeSnow || id == ObjectType::ID::DeadTreeSnow_Small || id == ObjectType::ID::DeadTree_Small
		|| id == ObjectType::ID::Pine_Tree || id == ObjectType::ID::Pine_Tree_Snow || id == ObjectType::ID::Tree_Small || id == ObjectType::ID::Tree_Small_Snow)
		&& currentlyEquipedItem == 0)
	{
		if (!HitWAxe.isPlaying())
			HitWAxe.playSound();
		if (inInventory[2] == false)
		{
			if (counter >= 4)
			{
				equip("WoodIcon");
				this->currentlyEquipedItem = 2;
				addImageToInventory("InventoryWoodIcon", 2);
				addTextToScreen("TipTextureWood");
				inInventory[2] = true;
				this->currentlyEquipedItem = 2;
				equipItemMesh();
				swapItem = true;
				pullDown = true;
				isAlive = false;
			}
			else
				counter++;
		}
		else if (inInventory[2] == true)
		{
			if (counter >= 4)
				isAlive = false;
			else
				counter++;
		}
		//else
		//	addTextToScreen("Text-ItemAlreadyEquipped");
	}
	else if (id == ObjectType::ID::Campfire && currentlyEquipedItem == 1)
	{
		return 3;
	}

	return -1;
}

int Player::collisionResponse(const ObjectType::ID)
{
	if (id == ObjectType::ID::Campfire)
	{
		return 3;
	}

	return -1;
}

void Player::heatResponse(float deltaTime)
{
	cold += .1f;
	if (this->cold > 100)
		this->cold = 100;
	if (currentlyEquipedItem == 4 && bucketContent == 1)
	{
		meltTimer += deltaTime;
		if (meltTimer > 5.0)
		{
			bucketContent = 2;
			equipItemMesh();
			swapItem = true;
			pullDown = true;
			meltTimer = 0.0;
		}
	}
}

void Player::takeDamange(float damage, float deltaTime)
{
	hp -= damage * deltaTime;
}

int Player::getEquipedItem()
{
	if (inInventory[2] == true)
	{
		addImageToInventory("EmptyImage", 2);
		inInventory[2] = false;
		return 2;
	}

	return -1;
}

const int Player::getEquipedID()
{
	return this->equipedID;
}

void Player::wolfHowl(float nightTimer)
{
	float wolfTimer = nightTimer / 20;
	if (nightTimer > 10 && nightTimer < 11 &&  !wolf2.isPlaying() && !wolf3.isPlaying())
		wolf3.playSound();
	
	if (nightTimer > 28 && nightTimer < 29 && !wolf2.isPlaying() && !wolf3.isPlaying())
		wolf2.playSound();

	if (nightTimer > 80 && nightTimer < 81 && !wolf2.isPlaying() && !wolf3.isPlaying())
		wolf2.playSound();

	if (nightTimer > 78 && nightTimer < 79 && !wolf2.isPlaying() && !wolf3.isPlaying())
		wolf3.playSound();
}

void Player::equipItemMesh()
{
	switch (currentlyEquipedItem)
	{
		case 0:
		{
			if (jacket)
				this->equipItem = 50;
			else
				this->equipItem = 33;
			break;
		}
		case 1:
		{
			if (jacket)
				this->equipItem = 52;
			else
				this->equipItem = 44;
			break;
		}
		case 2:
		{
			if (jacket)
				this->equipItem = 53;
			else
				this->equipItem = 45;
			break;
		}
		case 3:
		{
			if (jacket)
				this->equipItem = 51;
			else
				this->equipItem = 46;
			break;
		}
		case 4:
		{
			if (jacket)
			{
				if (bucketContent == 0)
					this->equipItem = 47;
				else if (bucketContent == 1)
				{
					this->equipItem = 48;
					pickUpSnow = false;
				}
				else
					this->equipItem = 49;
			}
			else
			{
				if (bucketContent == 0)
					this->equipItem = 34;
				else if (bucketContent == 1)
				{
					this->equipItem = 35;
					pickUpSnow = false;
				}
				else
					this->equipItem = 36;
			}
		}
	}
}

void Player::textWarnings()
{
	if (transform.position.y > 42 && !jacket && !warning)
	{
		addTextToScreen("ColdWarning");
		warning = true;
	}
	else if (transform.position.y < 42 && warning)
	{
		warning = false;
	}
}

void Player::findY()
{
	//float frontTemp = glm::mix(currentY, frontVertexHeight);
	int gridX = (int)glm::floor(cameraPos.x / distanceToNextVertex);
	int gridZ = (int)glm::floor(cameraPos.z / distanceToNextVertex);
	float gridSquareSize(distanceToNextVertex / ((float)vertexLength - 1));
	float xpos = cameraPos.x;
	float xCoord = ((int)cameraPos.x % (int)distanceToNextVertex) / distanceToNextVertex;
	float zCoord = ((int)cameraPos.z % (int)distanceToNextVertex) / distanceToNextVertex;
}

void Player::swingAxe(float deltaTime)
{
	if (currentlyEquipedItem != 0)
	{
		swing = false;
		return;
	}
	else if (axeSwing == 0)
	{
		swing = true;
		rotateSwing += deltaTime * 2.0f;
		pickUp += deltaTime * 2.0f;

		if (pickUp >= 0.4f)
			axeSwing = 1;
	}
	else if (axeSwing == 1)
	{
		pickUp -= deltaTime * 7.0f;
		rotateSwing -= deltaTime * 1.0f;
		if (pickUp <= -0.3f)
			axeSwing = 3;
	}
	else if (axeSwing == 3)
	{
		pickUp += deltaTime * 3.0f;
		rotateSwing -= deltaTime * 3.0f;
		if (pickUp >= 0)
		{
			pickUp = 0;
			rotateSwing = 0;
			swing = false;
		}
	}
}