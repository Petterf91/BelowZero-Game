#include "RenderManager.h"

RenderManager::RenderManager()
{
	createBuffers();
}

RenderManager::RenderManager(GameScene * otherGameScene, GLFWwindow* otherWindow, ShaderProgramLib* shaderProgram)
{
	gameScene = otherGameScene;
	window = otherWindow;


	this->geometryShaderProgram = shaderProgram->getShader<GeometryShaders>()->geometryShaderProgram;
	this->lightpassShaderProgram = shaderProgram->getShader<LightpassShaders>()->lightpassShaderProgram;
	//this->animationShaderProgram = shaderProgram->getShader<AnimationShaders>()->animationShaderProgram;
	this->shadowMapShaderProgram = shaderProgram->getShader<ShadowMapShader>()->ShadowMapShaderProgram;
	this->UIShaderProgram = shaderProgram->getShader<UIShaders>()->UIShaderProgram;
	this->vfxFireShaderProgram = shaderProgram->getShader<VFXFireShaders>()->vfxFireShaderProgram;
	this->vfxSnowShaderProgram = shaderProgram->getShader<VFXSnowShaders>()->vfxSnowShaderProgram;
	this->vfxFlareShaderProgram = shaderProgram->getShader<VFXFlareShaders>()->vfxFlareShaderProgram;
	this->vfxLighterShaderProgram = shaderProgram->getShader<VFXLighterShaders>()->vfxLighterShaderProgram;
	this->terrainShaderProgram = shaderProgram->getShader<TerrainShaders>()->TerrainShaderProgram;
	this->mainMenuShaderProgram = shaderProgram->getShader<MainMenuShader>()->MainMenuShaderProgram;
	this->refractionShaderProgram = shaderProgram->getShader<RefractionShader>()->RefractionShaderProgram;

	vao = 0;
	daylight = 1;
	time = 0;
	dayOrNight = true;
	fireFlicker = true;
	oldPitch = 0;
	oldYaw = 0;



	//// CHECK AGAINST GAMESTATE TO NOT LOAD unnecessary DATA
	//createMainMenuBuffer();


	//cascadePlaneEnds[0] = 0.1f;
	//cascadePlaneEnds[1] = 51.0f;
	//cascadePlaneEnds[2] = 70.0f;
	//cascadePlaneEnds[3] = 100.0f;

	//TESTING TWO CASCADES
	cascadePlaneEnds[0] = 0.1f;
	cascadePlaneEnds[1] = 51.0f;
	cascadePlaneEnds[2] = 70.0f;



	shatteredIce.CreateTextureData("iceNormal2.jpg");
	damageTexture.CreateTextureData("damage1.png");
	UiMeterTexture.CreateTextureData("UItest1.jpg");
}

RenderManager::~RenderManager()
{
	deleteData();

	glDeleteBuffers(1, &UIFBO);
	glDeleteBuffers(1, &UITexture);
	glDeleteBuffers(1, &shadowFBO);
	glDeleteBuffers(1, &animationVAO);
	glDeleteBuffers(1, &animationVBO);
	glDeleteBuffers(1, &animationEBO);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &gbo);
	glDeleteBuffers(1, &rboDepth);
	glDeleteBuffers(1, &finalFBO);
	glDeleteBuffers(1, &finalColorBuffer);
	glDeleteBuffers(1, &finalDepthStensil);
	glDeleteBuffers(1, &fireTexture);
	glDeleteBuffers(1, &snowTexture);
	glDeleteBuffers(1, &flareTexture);
	glDeleteBuffers(1, &lighterTexture);
	glDeleteBuffers(1, &fireVAO);
	glDeleteBuffers(1, &snowVAO);
	glDeleteBuffers(1, &flareVAO);
	glDeleteBuffers(1, &lighterVAO);
	glDeleteBuffers(1, &fireVBO);
	glDeleteBuffers(1, &snowVBO);
	glDeleteBuffers(1, &flareVBO);
	glDeleteBuffers(1, &lighterVBO);
	glDeleteBuffers(1, &fireParticlePositionBuffer);
	glDeleteBuffers(1, &snowParticlePositionBuffer);
	glDeleteBuffers(1, &flareParticlePositionBuffer);
	glDeleteBuffers(1, &lighterParticlePositionBuffer);
	glDeleteBuffers(1, &fireParticleColorBuffer);
	glDeleteBuffers(1, &snowParticleColorBuffer);
	glDeleteBuffers(1, &flareParticleColorBuffer);
	glDeleteBuffers(1, &lighterParticleColorBuffer);
	glDeleteBuffers(1, &PPFBO);
	glDeleteBuffers(1, &finalPPFBO);

	glDeleteShader(shadowMapShaderProgram);
	glDeleteShader(geometryShaderProgram);
	glDeleteShader(lightpassShaderProgram);
	glDeleteShader(animationShaderProgram);
	glDeleteShader(UIShaderProgram);
	glDeleteShader(terrainShaderProgram);
	glDeleteShader(vfxFireShaderProgram);
	glDeleteShader(vfxSnowShaderProgram);
	glDeleteShader(vfxFlareShaderProgram);
	glDeleteShader(vfxLighterShaderProgram);
	glDeleteShader(mainMenuShaderProgram);
	glDeleteShader(refractionShaderProgram);
}

bool zoneTest(GameObject* player, GameObject* object)
{
	if (player->zone.zoneXY == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(0, 1)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(0, -1)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(1, 0)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(-1, 0)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(-1, 1)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(1, -1)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(1, 1)) == object->zone.zoneXY ||
		(player->zone.zoneXY + glm::ivec2(-1, -1)) == object->zone.zoneXY
		)
	{
		return true;
	}
	return false;
}

void RenderManager::FindObjectsToRender() {
	for (unsigned int i = 0; i < gameScene->inZone.size(); i++) {
		if (zoneTest(gameScene->gameObjects[0], gameScene->inZone[i]))
		{
			glm::vec3 vectorToObject = gameScene->gameObjects[0]->transform->position - gameScene->inZone[i]->transform->position;
			float distance = length(vectorToObject);

			if (gameScene->inZone[i]->getIsRenderable() == true && distance < 110) {
				gameObjectsToRender.push_back(gameScene->inZone[i]);
			}

			if (gameScene->inZone[i]->hasLight == true) {
				lightsToRender.push_back(gameScene->inZone[i]->lightComponent);
			}
			if (gameScene->inZone[i]->fireComponent != nullptr)
			{
				lightsToRender.push_back(gameScene->inZone[i]->fireComponent);
			}
		}
		if (gameScene->gameObjects[i]->lighterComponent != nullptr)
		{
			lightsToRender.push_back(gameScene->gameObjects[i]->lighterComponent);
		}
		if (gameScene->gameObjects[i]->flareComponent != nullptr)
		{
			lightsToRender.push_back(gameScene->gameObjects[i]->flareComponent);
		}
	}
}

void RenderManager::clearObjectsToRender()
{
	while (!gameObjectsToRender.empty())
	{
		gameObjectsToRender.pop_back();
	}
	gameObjectsToRender.clear();

	while (!lightsToRender.empty())
	{
		lightsToRender.pop_back();
	}
	lightsToRender.clear();
}

void RenderManager::createBuffers()
{
	//screen size
	glfwGetFramebufferSize(window, &display_w, &display_h);

	// ----------========== CASCADED SHADOW MAPS ==========----------
	glGenFramebuffers(1, &shadowFBO);
	glGenTextures(2, shadowMaps);
	for (int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
		if (i == 0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, HIGH_SHADOW, HIGH_SHADOW, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		if (i == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, VERY_LOW_SHADOW, VERY_LOW_SHADOW, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		//if (i == 2)
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, LOW_SHADOW, LOW_SHADOW, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, MEDIUM_SHADOW, MEDIUM_SHADOW, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps[0], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Cascaded Shadow Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//VFX
	fireParticlePositionData = new GLfloat[MAX_PARTICLES * 4];
	snowParticlePositionData = new GLfloat[MAX_PARTICLES * 4];
	flareParticlePositionData = new GLfloat[MAX_PARTICLES * 4];
	lighterParticlePositionData = new GLfloat[MAX_PARTICLES * 4];
	fireParticleColorData = new GLubyte[MAX_PARTICLES * 4];
	snowParticleColorData = new GLubyte[MAX_PARTICLES * 4];
	flareParticleColorData = new GLubyte[MAX_PARTICLES * 4];
	lighterParticleColorData = new GLubyte[MAX_PARTICLES * 4];
	
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f
	};

	//Fire Texture
	glGenVertexArrays(1, &fireVAO);
	glBindVertexArray(fireVAO);
	glGenBuffers(1, &fireVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &fireParticlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, fireParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &fireParticleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, fireParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	width = 0;
	height = 0;
	nrOfChannels = 0;
	data = stbi_load("ParticleQuad.png", &width, &height, &nrOfChannels, 0);
	glGenTextures(1, &fireTexture);
	glBindTexture(GL_TEXTURE_2D, fireTexture);
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
	{
		std::cout << "Failed to load Billboard Texture from path" << std::endl;
	}
	stbi_image_free(data);

	//Lighter Texture
	glGenVertexArrays(1, &lighterVAO);
	glBindVertexArray(lighterVAO);
	glGenBuffers(1, &lighterVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lighterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &lighterParticlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lighterParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &lighterParticleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lighterParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	width = 0;
	height = 0;
	nrOfChannels = 0;
	data = stbi_load("ParticleQuad.png", &width, &height, &nrOfChannels, 0);
	glGenTextures(1, &lighterTexture);
	glBindTexture(GL_TEXTURE_2D, lighterTexture);
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
	{
		std::cout << "Failed to load Billboard Texture from path" << std::endl;
	}
	stbi_image_free(data);
	
	//Snow Texture
	glGenVertexArrays(1, &snowVAO);
	glBindVertexArray(snowVAO);
	glGenBuffers(1, &snowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &snowParticlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, snowParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &snowParticleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, snowParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	width = 0;
	height = 0;
	nrOfChannels = 0;
	data = stbi_load("ParticleSnow.png", &width, &height, &nrOfChannels, 0);
	glGenTextures(1, &snowTexture);
	glBindTexture(GL_TEXTURE_2D, snowTexture);
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
	{
		std::cout << "Failed to load Billboard Texture from path" << std::endl;
	}
	stbi_image_free(data);

	//Flare Texture
	//First reset the flareAlive bool
	flareAlive = false;
	glGenVertexArrays(1, &flareVAO);
	glBindVertexArray(flareVAO);
	glGenBuffers(1, &flareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, flareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &flareParticlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, flareParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &flareParticleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, flareParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	width = 0;
	height = 0;
	nrOfChannels = 0;
	data = stbi_load("Particle.png", &width, &height, &nrOfChannels, 0);
	glGenTextures(1, &flareTexture);
	glBindTexture(GL_TEXTURE_2D, flareTexture);
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
	{
		std::cout << "Failed to load Billboard Texture from path" << std::endl;
	}
	stbi_image_free(data);

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		fireParticleContainer[i].life = -1.0f;
		snowParticleContainer[i].life = -1.0f;
		flareParticleContainer[i].life = -1.0f;
		lighterParticleContainer[i].life = -1.0f;
		fireParticleContainer[i].cameraDistance = -1.0f;
		snowParticleContainer[i].cameraDistance = -1.0f;
		flareParticleContainer[i].cameraDistance = -1.0f;
		lighterParticleContainer[i].cameraDistance = -1.0f;
	}

	//... Create G-buffers
	//framebufferobject
	glGenFramebuffers(1, &gbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbo);
	//g-buffer position
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//g-buffer albedo
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gAlbedo, 0);

	//g-buffer normal
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attach texture to current framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gNormal, 0);

	glDrawBuffers(3, attachments);

	//... Create and attach depth buffer
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, display_w, display_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	//... Create Colorbuffer
	glGenFramebuffers(1, &finalFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
	glGenTextures(1, &finalColorBuffer);
	glBindTexture(GL_TEXTURE_2D, finalColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;

	//... Create and attach depth and stencil buffer
	glGenRenderbuffers(1, &finalDepthStensil);
	glBindRenderbuffer(GL_RENDERBUFFER, finalDepthStensil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, display_w, display_h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, finalDepthStensil);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, finalDepthStensil);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	//... Create PostProccessingbuffer
	glGenFramebuffers(1, &PPFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, PPFBO);
	glGenTextures(1, &finalPPFBO);
	glBindTexture(GL_TEXTURE_2D, finalPPFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, display_w, display_h, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalPPFBO, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;

	//.. Create UI Frame Buffer with UI Texture
	width = 0;
	height = 0;
	nrOfChannels = 0;
	data = stbi_load("uiTexture3.png", &width, &height, &nrOfChannels, 0);

	glGenFramebuffers(1, &UIFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);

	glGenTextures(1, &UITexture);
	glBindTexture(GL_TEXTURE_2D, UITexture);
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
	{
		std::cout << "Failed to load UI Texture from path" << std::endl;
	}

	stbi_image_free(data);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, UITexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "UI Framebuffer not complete!" << std::endl;
}

void RenderManager::deleteData()
{
	if (fireParticlePositionData != nullptr)
	{
		delete fireParticlePositionData;
	}
	if (fireParticleColorData != nullptr)
	{
		delete fireParticleColorData;
	}
	if (snowParticlePositionData != nullptr)
	{
		delete snowParticlePositionData;
	}
	if (snowParticleColorData != nullptr)
	{
		delete snowParticleColorData;
	}
	if (flareParticlePositionData != nullptr)
	{
		delete flareParticlePositionData;
	}
	if (flareParticleColorData != nullptr)
	{
		delete flareParticleColorData;
	}
	if (lighterParticlePositionData != nullptr)
	{
		delete lighterParticlePositionData;
	}
	if (lighterParticleColorData != nullptr)
	{
		delete lighterParticleColorData;
	}
}

void RenderManager::createMainMenuBuffer()
{
	glGenFramebuffers(1, &finalMainMenuFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, finalMainMenuFBO);

	glGenTextures(1, &finalMainMenuFBOTexture);
	glBindTexture(GL_TEXTURE_2D, finalMainMenuFBOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display_w, display_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalMainMenuFBOTexture, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, finalMainMenuFBOTexture);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Main Menu Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] =
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVertexArrayObject);
	glGenBuffers(1, &quadVertexBufferObject);
	glBindVertexArray(quadVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void RenderManager::createButtonQuads()
{
	float buttonQuadVertices1[] =
	{
		-0.3f,  0.1f, 0.0f, 1.0f, //same
		-0.3f, -0.1f, 0.0f, 0.0f,
		0.3f, -0.1f,  1.0f, 0.0f, //same

		-0.3f,  0.1f, 0.0f, 1.0f, //same
		0.3f, -0.1f,  1.0f, 0.0f, //same
		0.3f,  0.1f,  1.0f, 1.0f

	};
	float buttonQuadVertices2[] =
	{
		-0.3f,  0.1f,  0.0f, 1.0f,
		-0.3f, -0.1f,  0.0f, 0.0f,
		0.3f, -0.1f,  1.0f, 0.0f,

		-0.3f,  0.1f,  0.0f, 1.0f,
		0.3f, -0.1f,  1.0f, 0.0f,
		0.3f,  0.1f,  1.0f, 1.0f

	};
	float buttonQuadVertices3[] =
	{
		-0.3f,  0.1f, 0.0f, 1.0f,
		-0.3f, -0.1f,  0.0f, 0.0f,
		0.3f, -0.1f,  1.0f, 0.0f,

		-0.3f,  0.1f,  0.0f, 1.0f,
		0.3f, -0.1f,  1.0f, 0.0f,
		0.3f,  0.1f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &buttonVertexArrayObject[0]);
	glGenBuffers(1, &buttonBufferObject[0]);
	glBindVertexArray(buttonVertexArrayObject[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buttonBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttonQuadVertices1), &buttonQuadVertices1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenVertexArrays(1, &buttonVertexArrayObject[1]);
	glGenBuffers(1, &buttonBufferObject[1]);
	glBindVertexArray(buttonVertexArrayObject[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buttonBufferObject[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttonQuadVertices2), &buttonQuadVertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenVertexArrays(1, &buttonVertexArrayObject[2]);
	glGenBuffers(1, &buttonBufferObject[2]);
	glBindVertexArray(buttonVertexArrayObject[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buttonBufferObject[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttonQuadVertices3), &buttonQuadVertices3, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void RenderManager::Render() {
	FindObjectsToRender();
	dayNightCycle();

	for (int i = 0; i < gameScene->gameObjects.size(); i++)
	{
		if (gameScene->gameObjects[i]->getPlayer() != nullptr)
		{
			glm::vec2 temp = gameScene->gameObjects[i]->getPlayer()->setXZ();
			for (int j = 0; j < gameScene->gameObjects.size(); j++)
			{
				if (gameScene->gameObjects[j]->getTerrain() != nullptr)
				{
					gameScene->gameObjects[i]->getPlayer()->setCurrentHeight(gameScene->gameObjects[j]->getTerrain()->calculateY(temp.x, temp.y));
					break;
				}
			}
		}
		if (gameScene->gameObjects[i]->getAI() != nullptr)
		{
			glm::vec2 temp = gameScene->gameObjects[i]->getAI()->getXY();
			for (int j = 0; j < gameScene->gameObjects.size(); j++)
			{
				if (gameScene->gameObjects[j]->getTerrain() != nullptr)
				{
					gameScene->gameObjects[i]->getAI()->setCurrentHeight(gameScene->gameObjects[j]->getTerrain()->calculateY(temp.x, temp.y));
					break;
				}
			}
		}
	}

	//... Set view and projection matrix
	for (int i = 0; i < gameScene->gameObjects.size(); i++)
	{
		if (gameScene->gameObjects[i]->getPlayer() != nullptr)
		{
			view_matrix = gameScene->gameObjects[i]->getViewMatrix();
			break;
		}
	}
	projection_matrix = glm::perspective(glm::radians(60.0f), float(display_w) / float(display_h), 0.1f, 150.0f);

	glm::mat4 world_matrix = glm::mat4(1);
	//world_matrix = glm::translate(world_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
	//world_matrix = glm::rotate(world_matrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//... Clear Back Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.749 * daylight, 0.843 * daylight, 0.823 * daylight, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//... Clear finalFBO
	glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//... Clear PPFBO
	glBindFramebuffer(GL_FRAMEBUFFER, PPFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	
	// CASCADED SHADOWMAP PASS-----------------------------------------------------------------
	if (gameScene->gameObjects[0]->transform->position.y < 6.0)
	{
		glUseProgram(shadowMapShaderProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		calculateOrthoProjectionMatrices(shadowMapShaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(shadowMapShaderProgram, "lightView"), 1, GL_FALSE, glm::value_ptr(lightView));


		for (int i = 0; i < CASCADESPLITS; i++)
		{
			if (i == 0)
				glViewport(0, 0, HIGH_SHADOW, HIGH_SHADOW);
			if (i == 1)
				glViewport(0, 0, VERY_LOW_SHADOW, VERY_LOW_SHADOW);

			bindForWriting(i);
			glClear(GL_DEPTH_BUFFER_BIT);

			setOrthoProjectionMatrix(i);
			glUniformMatrix4fv(glGetUniformLocation(shadowMapShaderProgram, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

			//if (gameObjectsToRender.size() > 0)
				//gameObjectsToRender[0]->materialComponent->bindTextures();

			for (unsigned int i = 0; i < gameObjectsToRender.size(); i++)
			{
				gameObjectsToRender[i]->meshFilterComponent->bindVertexArray();

				//... Rotation equipment with player
				if (gameObjectsToRender[i]->meshFilterComponent->meshType == 2)
				{
					tempMatrix = gameObjectsToRender[i]->getModelMatrix();
					glm::vec3 forward = glm::vec3(gameObjectsToRender[i]->transform->forward);
					glm::vec3 right = glm::vec3(gameObjectsToRender[i]->transform->right);

					tempMatrix = glm::rotate(tempMatrix, gameObjectsToRender[0]->getPlayer()->oldYaw, glm::vec3(0, 1, 0));
					tempMatrix = glm::rotate(tempMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
					tempMatrix = glm::rotate(tempMatrix, gameObjectsToRender[0]->getPlayer()->oldPitch + gameObjectsToRender[0]->getPlayer()->pickUp, glm::vec3(0, 0, 1));
				}
				else
				{
					tempMatrix = glm::translate(glm::mat4(1), gameObjectsToRender[i]->transform->position);
					tempMatrix = glm::rotate(tempMatrix, glm::radians(gameObjectsToRender[i]->transform->rotation.y), gameObjectsToRender[i]->transform->up);
				}

				//...
				glUniformMatrix4fv(glGetUniformLocation(shadowMapShaderProgram, "world_matrix"), 1, GL_FALSE, glm::value_ptr(tempMatrix));
				glDrawArrays(GL_TRIANGLES, 0, gameObjectsToRender[i]->meshFilterComponent->vertexCount);
			}
		}
	}
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//... Terrain PASS-----------------------------------------------------------------------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, gbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, display_w, display_h);
	glUseProgram(terrainShaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(terrainShaderProgram, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(glGetUniformLocation(terrainShaderProgram, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(glGetUniformLocation(terrainShaderProgram, "world_matrix"), 1, GL_FALSE, glm::value_ptr(world_matrix));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // enable writing to the stencil buffer
	for (int i = 0; i < gameScene->gameObjects.size(); i++)
	{
		if (gameScene->gameObjects[i]->getTerrain() != nullptr)
		{
			gameScene->gameObjects[i]->getTerrain()->bindVertexArray();

			glDrawElements(GL_TRIANGLE_STRIP, gameScene->gameObjects[i]->getTerrain()->indices.size(), GL_UNSIGNED_INT, 0);
			
			break;
		}
	}

	//... GEOMETRY PASS----------------------------------------------------------------------------------------------------------------------------------------

	glUseProgram(geometryShaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(geometryShaderProgram, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(glGetUniformLocation(geometryShaderProgram, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
	//glUniformMatrix4fv(glGetUniformLocation(geometryShaderProgram, "world_matrix"), 1, GL_FALSE, glm::value_ptr(world_matrix));

	if (gameObjectsToRender.size() > 0)
		gameObjectsToRender[0]->materialComponent->bindTextures();

	for (unsigned int i = 0; i < gameObjectsToRender.size(); i++)
	{
		gameObjectsToRender[i]->meshFilterComponent->bindVertexArray();

		//... Rotation equipment with player
		if (gameObjectsToRender[i]->meshFilterComponent->meshType == 2)
		{
			tempMatrix = gameObjectsToRender[i]->getModelMatrix();
			tempMatrix = glm::rotate(tempMatrix, -gameObjectsToRender[i]->getPlayer()->oldYaw, glm::vec3(0, 1, 0));
			tempMatrix = glm::rotate(tempMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			tempMatrix = glm::rotate(tempMatrix, gameObjectsToRender[0]->getPlayer()->rotateSwing, glm::vec3(1, 0, 0));
			tempMatrix = glm::rotate(tempMatrix, gameObjectsToRender[0]->getPlayer()->oldPitch + gameObjectsToRender[0]->getPlayer()->pickUp, glm::vec3(0, 0, 1));
		}
		else
		{
			//... Position
			tempMatrix = glm::translate(glm::mat4(1), gameObjectsToRender[i]->transform->position);
			//... Rotation
			tempMatrix = glm::rotate(tempMatrix, glm::radians(gameObjectsToRender[i]->transform->rotation.y), gameObjectsToRender[i]->transform->up);
		}

		//...
		glUniformMatrix4fv(glGetUniformLocation(geometryShaderProgram, "world_matrix"), 1, GL_FALSE, glm::value_ptr(tempMatrix));
		glDrawArrays(GL_TRIANGLES, 0, gameObjectsToRender[i]->meshFilterComponent->vertexCount);
	}


	//... VFX--------------------------------------------------------------------------------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, gbo);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF); // enable writing to the stencil buffer
	glStencilFunc(GL_ALWAYS, 2, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glm::vec3 cameraPosition(glm::inverse(view_matrix)[3]);

	//... FIRE
	glUseProgram(vfxFireShaderProgram);
	for (GameObject* gameObject_ptr : gameObjectsToRender)
	{
		if (gameObject_ptr->getIsBurning())
		{
			if (gameObject_ptr->hasSoundAttatched == false && gameObject_ptr->objectID == ObjectType::ID::Campfire)
			{
				gameObject_ptr->hasSoundAttatched = true;
				gameObject_ptr->burning.addSound("fireplace2.0.ogg");
				gameObject_ptr->burning.setVolume(40.0f);
				if (!gameObject_ptr->burning.isPlaying())
				{
					gameObject_ptr->burning.playSound();
				}
			}
			else if (gameObject_ptr->hasSoundAttatched == false && gameObject_ptr->objectID == ObjectType::ID::Player)
			{
				gameObject_ptr->hasSoundAttatched = true;
				gameObject_ptr->burning.addSound("fireplace2.0.ogg");
				gameObject_ptr->burning.setVolume(40.0f);
				if (!gameObject_ptr->burning.isPlaying())
				{
					gameObject_ptr->burning.playSound();
				}
			}

			if (gameObject_ptr->hasSoundAttatched == true && gameObject_ptr->objectID == ObjectType::ID::Campfire)
			{
				float mixVar = glm::length(gameScene->gameObjects[0]->getPlayer()->transform.position - gameObject_ptr->transform->position);
				if (mixVar >= 50.0f)
					mixVar = 50.0f;

				float volume = glm::mix(60.0f, 0.0f, mixVar / 50.0f);
				gameObject_ptr->burning.setVolume(volume);
			}


			if (gameObject_ptr->hasSoundAttatched == true && gameObject_ptr->objectID == ObjectType::ID::Campfire && !gameObject_ptr->burning.isPlaying()
				|| gameObject_ptr->hasSoundAttatched == true && gameObject_ptr->objectID == ObjectType::ID::Player && !gameObject_ptr->burning.isPlaying())
			{
				gameObject_ptr->burning.loop(true);
				gameObject_ptr->burning.playSound();

			}
			offset = 40.0f;

			float flickerSpeed = (rand() % 1000) / 100000.0f;

			if (gameObject_ptr->fireComponent->intensity < 1.0 && fireFlicker)
			{
				gameObject_ptr->fireComponent->intensity += flickerSpeed;
				if (gameObject_ptr->fireComponent->intensity >= 1.0)
				{
					fireFlicker = false;
				}
			}
			else if (gameObject_ptr->fireComponent->intensity > 0.8 && !fireFlicker)
			{
				gameObject_ptr->fireComponent->intensity -= flickerSpeed;
				if (gameObject_ptr->fireComponent->intensity <= 0.8)
				{
					fireFlicker = true;
				}
			}

			//Randomizer for the spawn location
			randomX = gameObject_ptr->transform->position.x + (rand() % 5000 - 2500.0f) / 1000.0f;
			randomZ = gameObject_ptr->transform->position.z + (rand() % 5000 - 2500.0f) / 1000.0f;

			//Create the direction vector from a start and end point
			//and check how far away the particles are.
			//targetPoint = glm::vec3(defaultX, defaultY + offset, defaultZ);
			targetPoint = gameObject_ptr->transform->position;
			targetPoint.y += offset;
			//startPoint = glm::vec3(randomX, defaultY, randomZ);
			startPoint = glm::vec3(randomX, gameObject_ptr->transform->position.y, randomZ);
			//particlePivot = glm::vec3(defaultX, defaultY, defaultZ);
			particlePivot = gameObject_ptr->transform->position;
			directionVec = targetPoint - startPoint;

			//Get a random target point direction
			randomDirectionX = directionVec.x + (rand() % 2000 - 1000) / 3000.0f;
			randomDirectionZ = directionVec.z + (rand() % 2000 - 1000) / 3000.0f;

			directionVec.x = randomDirectionX;
			directionVec.y = directionVec.y / 5.0f;
			directionVec.z = randomDirectionZ;

			//Check if the particles are far away from the player,
			//if too far away --> Don't render
			// if player
			tempDistance = particlePivot - gameScene->gameObjects[0]->transform->position;
			distanceToParticles = abs((int)tempDistance.x + (int)tempDistance.z);

			if (distanceToParticles <= 100)
			{
				//Create a randomizer so it doesn't spawn all the particles on every frame, if needed
				randomizer = 1;

				if (randomizer == 1)
				{
					if (particleCount <= MAX_PARTICLES)
					{
						for (int i = 0; i < fireParticles; i++)
						{
							lastUsedParticle = FindUnusedParticle(fireParticleContainer, lastUsedParticle);
							int particleIndex = lastUsedParticle;

							fireParticleContainer[particleIndex].life = 1.0f;
							fireParticleContainer[particleIndex].pos = startPoint;
							glm::vec3 mainDir = glm::vec3(0.0f, 0.1f, 0.0f);

							//Set the new direction for the particle
							fireParticleContainer[particleIndex].speed = mainDir + directionVec / 3.0f;

							//Set colors
							fireParticleContainer[particleIndex].r = 1.0f;
							fireParticleContainer[particleIndex].g = 1.0f;
							fireParticleContainer[particleIndex].b = 0;
							fireParticleContainer[particleIndex].a = (rand() % 256) / 3;

							fireParticleContainer[particleIndex].size = ((rand() % 1000) / 1500.0f) / 1.5f;
						}
					}
				}

				particleCount = 0;
				//Movement of the new particles
				for (int i = 0; i < MAX_PARTICLES; i++)
				{
					fireParticleContainer[i].life -= deltaTime / 1.5f;
					if (fireParticleContainer[i].life > 0.0f)
					{
						//Control the movement with the wind
						if (fireParticleContainer[i].life > 0.6f)
						{
							fireParticleContainer[i].speed += (glm::vec3(0.0f, -0.05f, 0.0f) * 0.5f) * deltaTime;
						}
						else if (fireParticleContainer[i].life > 0.4f)
						{
							fireParticleContainer[i].speed += (glm::vec3(2.5f, -0.05f, 1.75f) * 0.5f) * deltaTime;
						}
						else
						{
							fireParticleContainer[i].speed += (glm::vec3(5.0f, -0.05f, 2.5f) * 0.5f) * deltaTime;
						}
						fireParticleContainer[i].pos += fireParticleContainer[i].speed / 90.0f;
						fireParticleContainer[i].cameraDistance = glm::length(fireParticleContainer[i].pos - cameraPosition);

						//Set Positions
						fireParticlePositionData[4 * particleCount + 0] = fireParticleContainer[i].pos.x;
						fireParticlePositionData[4 * particleCount + 1] = fireParticleContainer[i].pos.y;
						fireParticlePositionData[4 * particleCount + 2] = fireParticleContainer[i].pos.z;
						fireParticlePositionData[4 * particleCount + 3] = fireParticleContainer[i].size;

						//Set Colors

						if (fireParticleContainer[i].life > 0.7f)
						{
							fireParticleColorData[4 * particleCount + 1] = ((fireParticleContainer[i].life * fireParticleContainer[i].g) / 3.0f) * 255.0f;
						}
						else
						{
							fireParticleColorData[4 * particleCount + 1] = ((fireParticleContainer[i].life * fireParticleContainer[i].g) / 4.0f) * 255.0f;
						}

						if (fireParticleContainer[i].life <= 0.3f)
						{
							fireParticleColorData[4 * particleCount + 0] = (fireParticleContainer[i].r * (fireParticleContainer[i].life * 3.0f)) * 255.0f;
						}
						else
						{
							fireParticleColorData[4 * particleCount + 0] = fireParticleContainer[i].r * 255.0f;

						}

						fireParticleColorData[4 * particleCount + 2] = fireParticleContainer[i].life * fireParticleContainer[i].b;
						fireParticleColorData[4 * particleCount + 3] = (fireParticleContainer[i].a * fireParticleContainer[i].life) * 3.0f;
					}
					else
					{
						fireParticleContainer[i].cameraDistance = -1.0f;
						fireParticlePositionData[4 * particleCount + 3] = 0;	//If dead -> Size = 0
					}
					particleCount++;
				}

				//Update particle information
				glBindBuffer(GL_ARRAY_BUFFER, fireParticlePositionBuffer);
				glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLfloat), fireParticlePositionData);

				glBindBuffer(GL_ARRAY_BUFFER, fireParticleColorBuffer);
				glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLubyte), fireParticleColorData);

				//Apply Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, fireTexture);
				glUniform1i(glGetUniformLocation(vfxFireShaderProgram, "particleTexture"), 0);

				//Get and set matrices
				viewProjectionMatrix = projection_matrix * view_matrix;
				cameraRight_vector = glm::vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
				cameraUp_vector = glm::vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);
				glUniform3fv(glGetUniformLocation(vfxFireShaderProgram, "cameraRight_worldspace"), 1, glm::value_ptr(cameraRight_vector));
				glUniform3fv(glGetUniformLocation(vfxFireShaderProgram, "cameraUp_worldspace"), 1, glm::value_ptr(cameraUp_vector));
				glUniformMatrix4fv(glGetUniformLocation(vfxFireShaderProgram, "vp"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
				glUniform3fv(glGetUniformLocation(vfxFireShaderProgram, "view_position"), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));
				glUniform3fv(glGetUniformLocation(vfxFireShaderProgram, "particlePivot"), 1, glm::value_ptr(startPoint));

				//Draw Particles
				renderFireParticles();
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
			}
		}
	}

	//!...LIGHTER
	GLCall(glUseProgram(vfxLighterShaderProgram));
	for (GameObject* gameObject_ptr : gameObjectsToRender)
	{
		if (gameObject_ptr->lighterEquipped)
		{
			//Particle system location, can be changed dynamically if e.g. a torch is wanted

			//Offset from local: X: 0.53 Y: 0.35 Z: -0.29

			startPoint = gameObject_ptr->transform->getLighterPosition();

			//Create the direction vector from a start and end point
			//and check how far away the particles are.
			targetPoint = glm::vec3(startPoint.x, startPoint.y, startPoint.z);
			targetPoint.y += 5.0f;

			particlePivot = gameObject_ptr->transform->position;
			directionVec = targetPoint - startPoint;

			lighterPosition = startPoint;
			
			//Create a randomizer so it doesn't spawn all the particles on every frame, if needed
			randomizer = rand() % 5;

			if (randomizer == 1)
			{
				if (particleCount <= MAX_PARTICLES)
				{
					for (int i = 0; i < lighterParticles; i++)
					{
						lastUsedParticle = FindUnusedParticle(lighterParticleContainer, lastUsedParticle);
						int particleIndex = lastUsedParticle;

						lighterParticleContainer[particleIndex].life = 1.0f;
						lighterParticleContainer[particleIndex].pos = startPoint;

						//Fix the rest constants that's needed for a "living" looking fire.
						//First, create a spread with values from 0.00 -> 1.00
						float spread = (rand() % 100) / 100.0f;
						glm::vec3 mainDir = glm::vec3(0.0f, 0.01f, 0.0f);

						//Set the new direction for the particle
						lighterParticleContainer[particleIndex].speed = mainDir + directionVec / 10.0f;

						//Set colors
						lighterParticleContainer[particleIndex].r = 1.0f;
						lighterParticleContainer[particleIndex].g = 1.0f;
						lighterParticleContainer[particleIndex].b = 0;
						lighterParticleContainer[particleIndex].a = (rand() % 256) / 3;

						lighterParticleContainer[particleIndex].size = ((rand() % 1000) / 1500.0f) / 10.0f;
					}
				}
			}

			particleCount = 0;
			//Movement of the new particles
			for (int i = 0; i < MAX_PARTICLES; i++)
			{
				lighterParticleContainer[i].life -= 0.016f;
				if (lighterParticleContainer[i].life > 0.0f)
				{
					//Control the movement with the wind
					
					lighterParticleContainer[i].speed += glm::vec3(0.0f, -0.1f, 0.0f) * 0.5f * deltaTime;
					
					if (lighterParticleContainer[i].life >= 0.6f)
					{
						lighterParticleContainer[i].pos += lighterParticleContainer[i].speed / 80.0f;
					}
					else if (lighterParticleContainer[i].life < 0.6f && lighterParticleContainer[i].life > 0.4f)
					{
						lighterParticleContainer[i].pos += lighterParticleContainer[i].speed / 100.0f;
					}
					else
					{
						lighterParticleContainer[i].pos += lighterParticleContainer[i].speed / 140.0f;
					}

					lighterParticleContainer[i].cameraDistance = glm::length(lighterParticleContainer[i].pos - cameraPosition);

					//Set Positions
					lighterParticlePositionData[4 * particleCount + 0] = lighterParticleContainer[i].pos.x;
					lighterParticlePositionData[4 * particleCount + 1] = lighterParticleContainer[i].pos.y;
					lighterParticlePositionData[4 * particleCount + 2] = lighterParticleContainer[i].pos.z;
					lighterParticlePositionData[4 * particleCount + 3] = lighterParticleContainer[i].size;

					//Set Colors

					if (lighterParticleContainer[i].life > 0.7f)
					{
						lighterParticleColorData[4 * particleCount + 1] = ((lighterParticleContainer[i].life * lighterParticleContainer[i].g) / 3.0f) * 255.0f;
					}
					else
					{
						lighterParticleColorData[4 * particleCount + 1] = ((lighterParticleContainer[i].life * lighterParticleContainer[i].g) / 4.0f) * 255.0f;
					}

					if (lighterParticleContainer[i].life <= 0.3f)
					{
						lighterParticleColorData[4 * particleCount + 0] = (lighterParticleContainer[i].r * (lighterParticleContainer[i].life * 3.0f)) * 255.0f;
					}
					else
					{
						lighterParticleColorData[4 * particleCount + 0] = lighterParticleContainer[i].r * 255.0f;

					}

					lighterParticleColorData[4 * particleCount + 2] = lighterParticleContainer[i].life * lighterParticleContainer[i].b;
					lighterParticleColorData[4 * particleCount + 3] = (lighterParticleContainer[i].a * lighterParticleContainer[i].life) * 3.0f;
				}
				else
				{
					lighterParticleContainer[i].cameraDistance = -1.0f;
					lighterParticlePositionData[4 * particleCount + 3] = 0;	//If dead -> Size = 0
				}
				particleCount++;
			}

			//Update particle information
			glBindBuffer(GL_ARRAY_BUFFER, lighterParticlePositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLfloat), lighterParticlePositionData);

			glBindBuffer(GL_ARRAY_BUFFER, lighterParticleColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLubyte), lighterParticleColorData);

			//Apply Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, lighterTexture);
			glUniform1i(glGetUniformLocation(vfxLighterShaderProgram, "particleTexture"), 0);

			//Get and set matrices
			viewProjectionMatrix = projection_matrix * view_matrix;
			cameraRight_vector = glm::vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
			cameraUp_vector = glm::vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);
			glUniform3fv(glGetUniformLocation(vfxLighterShaderProgram, "cameraRight_worldspace"), 1, glm::value_ptr(cameraRight_vector));
			glUniform3fv(glGetUniformLocation(vfxLighterShaderProgram, "cameraUp_worldspace"), 1, glm::value_ptr(cameraUp_vector));
			glUniformMatrix4fv(glGetUniformLocation(vfxLighterShaderProgram, "vp"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
			glUniform3fv(glGetUniformLocation(vfxLighterShaderProgram, "view_position"), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));
			GLCall(glUniform3fv(glGetUniformLocation(vfxLighterShaderProgram, "particlePivot"), 1, glm::value_ptr(startPoint)));

			//Draw Particles
			renderLighterParticles();
			GLCall(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount));
			break;
		}
	}

	//!...SNOW
	glUseProgram(vfxSnowShaderProgram);
	//Particle system location, can be changed dynamically if e.g. a torch is wanted
	defaultX = gameScene->gameObjects[0]->transform->position.x;
	defaultY = gameScene->gameObjects[0]->transform->position.y;
	defaultZ = gameScene->gameObjects[0]->transform->position.z;
	offset = 15.0f;

	//Create the direction vector from a start and end point
	//and check how far away the particles are.
	targetPoint = glm::vec3(defaultX, defaultY + offset, defaultZ);
	startPoint = glm::vec3(defaultX, defaultY, defaultZ);
	particlePivot = glm::vec3(defaultX, defaultY, defaultZ);
	directionVec = targetPoint - startPoint;

	//Get a random target point direction
	randomDirectionX = directionVec.x + (rand() % 2000 - 1000) / 3000.0f;
	randomDirectionZ = directionVec.z + (rand() % 2000 - 1000) / 3000.0f;

	directionVec.x = randomDirectionX;
	directionVec.y = directionVec.y / 5.0f;
	directionVec.z = randomDirectionZ;

	//Check if the particles are far away from the player,
	//if too far away --> Don't render
	tempDistance = particlePivot - gameScene->gameObjects[0]->transform->position;
	distanceToParticles = abs((int)tempDistance.x + (int)tempDistance.z);

	//Create a randomizer so it doesn't spawn all the particles on every frame, if needed
	randomizer = 1;

	if (randomizer == 1)
	{
		if (particleCount <= MAX_PARTICLES)
		{
			for (int i = 0; i < snowParticles; i++)
			{
				//Randomizer for the spawn location
				randomX = defaultX + (rand() % 6000 - 3000.0f) / 100.0f;
				randomY = defaultY + (rand() % 6000 - 3000.0f) / 100.0f;
				randomZ = defaultZ + (rand() % 6000 - 3000.0f) / 100.0f;

				lastUsedParticle = FindUnusedParticle(snowParticleContainer, lastUsedParticle);
				int particleIndex = lastUsedParticle;

				snowParticleContainer[particleIndex].life = rand() % 3 + 1;
				snowParticleContainer[particleIndex].pos = glm::vec3(randomX, randomY + 5.0f, randomZ);

				//First, create a spread with values from 0.00 -> 1.00
				float spread = (rand() % 100) / 100.0f;

				//Start direction
				glm::vec3 mainDir = glm::vec3(10.0f, -4.5f, 5.0f);					//Change to (0.0f, -0.1f, 0.0f) for straight falling snow 

																					//Complete random in X- and Z-axis
				glm::vec3 randomDir = glm::vec3(
					(sin(rand() % 10 - 10.0f) / 5.0f),
					0,
					(sin(rand() % 10 - 10.0f) / 5.0f)
				);

				//Set the new direction for the particle
				snowParticleContainer[particleIndex].speed = mainDir + randomDir * spread;

				//Set colors, if you want color from texture, don't change the color
				/*lighterParticleContainer[particleIndex].r = 150.0f;
				lighterParticleContainer[particleIndex].g = 150.0f;
				lighterParticleContainer[particleIndex].b = 150.0f;*/
				snowParticleContainer[particleIndex].a = (rand() % 256) / 3;

				snowParticleContainer[particleIndex].size = ((rand() % 750) / 2000.0f);
			}
		}
	}

	particleCount = 0;
	//Movement of the new particles
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		snowParticleContainer[i].life -= deltaTime / 2.0f;
		if (snowParticleContainer[i].life > 0.0f)
		{
			snowParticleContainer[i].speed += (glm::vec3(20.0f, -6.0f, 10.0f) * 0.5f) * deltaTime;
			snowParticleContainer[i].pos += snowParticleContainer[i].speed / 70.0f;
			snowParticleContainer[i].cameraDistance = glm::length(snowParticleContainer[i].pos - cameraPosition);

			//Set Positions
			snowParticlePositionData[4 * particleCount + 0] = snowParticleContainer[i].pos.x;
			snowParticlePositionData[4 * particleCount + 1] = snowParticleContainer[i].pos.y;
			snowParticlePositionData[4 * particleCount + 2] = snowParticleContainer[i].pos.z;

			if (snowParticleContainer[i].life > 1.0f)
			{
				snowParticlePositionData[4 * particleCount + 3] = snowParticleContainer[i].size;
			}
			else
			{
				snowParticlePositionData[4 * particleCount + 3] = snowParticleContainer[i].size * snowParticleContainer[i].life;
			}

			//Set Colors
			snowParticleColorData[4 * particleCount + 0] = snowParticleContainer[i].r;
			snowParticleColorData[4 * particleCount + 1] = snowParticleContainer[i].g;
			snowParticleColorData[4 * particleCount + 2] = snowParticleContainer[i].b;
			snowParticleColorData[4 * particleCount + 3] = snowParticleContainer[i].a;
		}
		else
		{
			snowParticleContainer[i].cameraDistance = -1.0f;
			snowParticlePositionData[4 * particleCount + 3] = 0;	//If dead -> Size = 0
		}
		particleCount++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, snowParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLfloat), snowParticlePositionData);

	glBindBuffer(GL_ARRAY_BUFFER, snowParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLubyte), snowParticleColorData);

	//Apply Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, snowTexture);
	glUniform1i(glGetUniformLocation(vfxSnowShaderProgram, "particleTexture"), 0);

	//Get and set matrices
	viewProjectionMatrix = projection_matrix * view_matrix;
	cameraRight_vector = glm::vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
	cameraUp_vector = glm::vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);
	glUniform3fv(glGetUniformLocation(vfxSnowShaderProgram, "cameraRight_worldspace"), 1, glm::value_ptr(cameraRight_vector));
	glUniform3fv(glGetUniformLocation(vfxSnowShaderProgram, "cameraUp_worldspace"), 1, glm::value_ptr(cameraUp_vector));
	glUniformMatrix4fv(glGetUniformLocation(vfxSnowShaderProgram, "vp"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
	glUniform3fv(glGetUniformLocation(vfxSnowShaderProgram, "view_position"), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));
	glUniform3fv(glGetUniformLocation(vfxSnowShaderProgram, "particlePivot"), 1, glm::value_ptr(startPoint));

	//Draw Particles
	renderSnowParticles();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);


	//... FLARE
	glUseProgram(vfxFlareShaderProgram);
	for (GameObject* gameObject_ptr : gameObjectsToRender)
	{
		if (gameObject_ptr->gameEnd || pickedUpFlare == true)
		{
			pickedUpFlare = true;

			if (gameObject_ptr->objectID == ObjectType::ID::FlareGun)
			{
				gameObject_ptr->isActive = false;
			}
			//Particle system location, can be changed dynamically if e.g. a torch is wanted
			flareShotTimer += deltaTime;

			if (flareShotTimer >= 2.0f)
			{
				gameScene->gameObjects[0]->delayFlare = true;
				defaultX = gameScene->gameObjects[0]->transform->position.x;
				defaultY = gameScene->gameObjects[0]->transform->position.y;
				defaultZ = gameScene->gameObjects[0]->transform->position.z;
				offset = 100.0f;

				//Create the direction vector from a start and end point
				//and check how far away the particles are.
				targetPoint = gameObject_ptr->transform->position;
				targetPoint.y += offset;

				startPoint = glm::vec3(defaultX, defaultY, defaultZ);// +(gameScene->gameObjects[0]->transform->forward * 1.2f);
				particlePivot = gameObject_ptr->transform->position;
				directionVec = targetPoint - startPoint;

				if (particleCount <= MAX_PARTICLES && flareAlive == false)
				{
					flareAlive = true;
					for (int i = 0; i < flareParticles; i++)
					{
						lastUsedParticle = FindUnusedParticle(flareParticleContainer, lastUsedParticle);
						int particleIndex = lastUsedParticle;

						flareParticleContainer[particleIndex].life = 4.0f;
						flareParticleContainer[particleIndex].pos = startPoint;

						//Fix the rest constants that's needed for a "living" looking fire.
						//First, create a spread with values from 0.00 -> 1.00
						float spread = (rand() % 100) / 100.0f;
						glm::vec3 mainDir = glm::vec3(0.0f, 0.1f, 0.0f);

						//Set the new direction for the particle
						flareParticleContainer[particleIndex].speed = directionVec / 5.0f;

						//Set colors
						flareParticleContainer[particleIndex].r = 1.0f;
						flareParticleContainer[particleIndex].g = 0;
						flareParticleContainer[particleIndex].b = 0;
						flareParticleContainer[particleIndex].a = 255.0f;

						flareParticleContainer[particleIndex].size = 2.0f;
					}
				}

				particleCount = 0;
				//Movement of the new particles
				for (int i = 0; i < MAX_PARTICLES; i++)
				{
					flareParticleContainer[i].life -= 0.016f / 1.8f;
					if (flareParticleContainer[i].life > 0.0f)
					{
						//Control the movement with the wind
						if (flareParticleContainer[i].pos.y <= 80.0f)
						{
							flareParticleContainer[i].speed += glm::vec3(1.5f, -5.0f, 0.75f) * deltaTime;
						}
						else
						{
							flareParticleContainer[i].speed += glm::vec3(0.5f, -3.5f, 0.25f) * deltaTime;
						}

						flareParticleContainer[i].pos += flareParticleContainer[i].speed / 20.0f;
						flareParticleContainer[i].cameraDistance = glm::length(flareParticleContainer[i].pos - cameraPosition);

						//Set Positions
						flareParticlePositionData[4 * particleCount + 0] = flareParticleContainer[i].pos.x;
						flareParticlePositionData[4 * particleCount + 1] = flareParticleContainer[i].pos.y;
						flareParticlePositionData[4 * particleCount + 2] = flareParticleContainer[i].pos.z;
						flareParticlePositionData[4 * particleCount + 3] = flareParticleContainer[i].size;

						//Set Colors
						flareParticleColorData[4 * particleCount + 0] = flareParticleContainer[i].r * 255.0f;
						flareParticleColorData[4 * particleCount + 1] = flareParticleContainer[i].g;
						flareParticleColorData[4 * particleCount + 2] = flareParticleContainer[i].b;
						flareParticleColorData[4 * particleCount + 3] = flareParticleContainer[i].a;
						if (flareParticleContainer[i].life <= 1.0f)
						{
							flareParticleColorData[4 * particleCount + 3] = flareParticleContainer[i].a * flareParticleContainer[i].life;
						}
					}
					else
					{
						flareParticleContainer[i].cameraDistance = -1.0f;
						flareParticlePositionData[4 * particleCount + 3] = 0;	//If dead -> Size = 0
					}
					particleCount++;
				}

				//Update particle information
				glBindBuffer(GL_ARRAY_BUFFER, flareParticlePositionBuffer);
				glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLfloat), flareParticlePositionData);

				glBindBuffer(GL_ARRAY_BUFFER, flareParticleColorBuffer);
				glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * 4 * sizeof(GLubyte), flareParticleColorData);

				//Apply Texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, flareTexture);
				glUniform1i(glGetUniformLocation(vfxFlareShaderProgram, "particleTexture"), 0);

				//Get and set matrices
				viewProjectionMatrix = projection_matrix * view_matrix;
				cameraRight_vector = glm::vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
				cameraUp_vector = glm::vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);
				glUniform3fv(glGetUniformLocation(vfxFlareShaderProgram, "cameraRight_worldspace"), 1, glm::value_ptr(cameraRight_vector));
				glUniform3fv(glGetUniformLocation(vfxFlareShaderProgram, "cameraUp_worldspace"), 1, glm::value_ptr(cameraUp_vector));
				glUniformMatrix4fv(glGetUniformLocation(vfxFlareShaderProgram, "vp"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
				glUniform3fv(glGetUniformLocation(vfxFlareShaderProgram, "view_position"), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));
				glUniform3fv(glGetUniformLocation(vfxFlareShaderProgram, "particlePivot"), 1, glm::value_ptr(startPoint));

				//Draw Particles
				renderFlareParticles();
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
			}
			break;
		}
	}

	//... Copy Stencil Buffer from gbo to finalFBO
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, finalFBO);
	glBlitFramebuffer(0, 0, display_w, display_h, 0, 0, display_w, display_h, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, display_w, display_h, 0, 0, display_w, display_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//... LIGHTING PASS----------------------------------------------------------------------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);
	glUseProgram(lightpassShaderProgram);

	//CAM pos
	glUniform3fv(glGetUniformLocation(lightpassShaderProgram, "view_position"), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));

	//Nr Of Lights
	glUniform1i(glGetUniformLocation(lightpassShaderProgram, "nrOfLights"), lightsToRender.size());

	for (unsigned int i = 0; i < lightsToRender.size(); i++)
	{
		std::string lightUniform;
		//position
		if (lightsToRender.at(i)->isLighter == true)
		{
			lightUniform = "lights[" + std::to_string(i) + "].Position";
			glUniform3fv(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), 1, glm::value_ptr(lightsToRender.at(i)->transform.getLighterPosition() + glm::vec3(0, 2, 0)));
		}
		else if (lightsToRender.at(i)->isFlare == true)
		{
			lightUniform = "lights[" + std::to_string(i) + "].Position";
			glUniform3fv(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));
		}
		else
		{
			lightUniform = "lights[" + std::to_string(i) + "].Position";
			glUniform3fv(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), 1, glm::value_ptr(glm::vec3(lightsToRender.at(i)->transform.position.x, lightsToRender.at(i)->transform.position.y + lightsToRender.at(i)->offset, lightsToRender.at(i)->transform.position.z)));
		}

		//Color
		lightUniform = "lights[" + std::to_string(i) + "].Color";
		glUniform3fv(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), 1, glm::value_ptr(lightsToRender.at(i)->color));

		//Attenuation lightType
		lightUniform = "lights[" + std::to_string(i) + "].lightType";
		glUniform1i(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), lightsToRender.at(i)->lightType);

		//Attenuation intensity
		lightUniform = "lights[" + std::to_string(i) + "].intensity";
		glUniform1f(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), lightsToRender.at(i)->intensity);

		//Attenuation linear
		lightUniform = "lights[" + std::to_string(i) + "].Linear";
		glUniform1f(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), lightsToRender.at(i)->Linear);

		//Attenuation quadratic
		lightUniform = "lights[" + std::to_string(i) + "].Quadratic";
		glUniform1f(glGetUniformLocation(lightpassShaderProgram, lightUniform.c_str()), lightsToRender.at(i)->Quadratic);

	}

	glUniform1f(glGetUniformLocation(lightpassShaderProgram, "daylight"), daylight);

	glUniform1i(glGetUniformLocation(lightpassShaderProgram, "gPosition"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glUniform1i(glGetUniformLocation(lightpassShaderProgram, "gAlbedo"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);

	glUniform1i(glGetUniformLocation(lightpassShaderProgram, "gNormal"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gNormal);

	glUniform1i(glGetUniformLocation(lightpassShaderProgram, "shadowMap0"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowMaps[0]);

	glUniform1i(glGetUniformLocation(lightpassShaderProgram, "shadowMap1"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, shadowMaps[1]);
	
	glUniform3fv(glGetUniformLocation(lightpassShaderProgram, "shadowMapLightPosition"), 1, glm::value_ptr(shadowMapLightPosition));

	for (int i = 0; i < CASCADESPLITS; i++)
	{
		std::string cT = "cascadeEndClipSpace[" + std::to_string(i) + "]";
		glUniform1f(glGetUniformLocation(lightpassShaderProgram, cT.c_str()), cascadesInClipSpace[i]);
	}
	for (int i = 0; i < CASCADESPLITS; i++)
	{
		std::string lT = "lightSpaceMatrix[" + std::to_string(i) + "]";
		glUniformMatrix4fv(glGetUniformLocation(lightpassShaderProgram, lT.c_str()), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrices[i]));
	}	

	glUniformMatrix4fv(glGetUniformLocation(lightpassShaderProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(glGetUniformLocation(lightpassShaderProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));


	glUniform1f(glGetUniformLocation(lightpassShaderProgram, "water"), gameScene->gameObjects[0]->getPlayer()->water);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00); // disable writing to the stencil buffer
	renderQuad();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_STENCIL_TEST);

	//-----------=====POST PROCESSING====----------------------
	glBindFramebuffer(GL_READ_FRAMEBUFFER, finalFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, PPFBO);
	glBlitFramebuffer(0, 0, display_w, display_h, 0, 0, display_w, display_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	if (this->seconds > 2.0)
		this->seconds = 0.0f;

	glBindFramebuffer(GL_FRAMEBUFFER, PPFBO);
	glUseProgram(refractionShaderProgram);

	glUniform3fv(glGetUniformLocation(refractionShaderProgram, "view_position"), 1, glm::value_ptr(gameScene->gameObjects[0]->transform->position));

	glUniform1i(glGetUniformLocation(refractionShaderProgram, "gPosition"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);

	glUniform1i(glGetUniformLocation(refractionShaderProgram, "gNormal"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);

	glUniform1i(glGetUniformLocation(refractionShaderProgram, "iceNormal"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shatteredIce.gTexture);

	glUniform1i(glGetUniformLocation(refractionShaderProgram, "SceneTexture"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, finalColorBuffer);

	glUniform1i(glGetUniformLocation(refractionShaderProgram, "DamageTexture"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, damageTexture.gTexture);

	glUniform1f(glGetUniformLocation(refractionShaderProgram, "healthSeconds"), this->seconds);
	glUniform1f(glGetUniformLocation(refractionShaderProgram, "hp"), gameScene->gameObjects[0]->getPlayer()->hp);
	glUniform1f(glGetUniformLocation(refractionShaderProgram, "cold"), gameScene->gameObjects[0]->getPlayer()->cold);
	glUniform1f(glGetUniformLocation(refractionShaderProgram, "water"), gameScene->gameObjects[0]->getPlayer()->water);
	glUniform1f(glGetUniformLocation(refractionShaderProgram, "food"), gameScene->gameObjects[0]->getPlayer()->food);
	glUniform1f(glGetUniformLocation(refractionShaderProgram, "fade"), gameScene->gameObjects[0]->getPlayer()->fade);

	glUniform1i(glGetUniformLocation(refractionShaderProgram, "ScreenX"), SCREEN_WIDTH);
	glUniform1i(glGetUniformLocation(refractionShaderProgram, "ScreenY"), SCREEN_HEIGHT);

	//glBindFramebuffer(GL_READ_FRAMEBUFFER, finalFBO);
	//glBindFramebuffer(GL_TEXTURE_2D, PPFBO);
	//glBlitFramebuffer(0, 0, display_w, display_h, 0, 0, display_w, display_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, PPFBO);
	//glBindTexture(GL_TEXTURE_2D, PPFBO);

	renderQuad();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//... UI -----------------------------------------------------------------------------------------------------------------------------------

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(UIShaderProgram);

	//depthMapTransformation = 0;
	//glUniform1i(glGetUniformLocation(UIShaderProgram, "depthMapTransformation"), depthMapTransformation);

	glUniform1i(glGetUniformLocation(UIShaderProgram, "theTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, UITexture);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "equipedTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->equipedTexture);

	glUniform1i(glGetUniformLocation(UIShaderProgram, "inventoryTexture1"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->inventoryTexture[0]);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "inventoryTexture2"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->inventoryTexture[1]);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "inventoryTexture3"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->inventoryTexture[2]);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "inventoryTexture4"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->inventoryTexture[3]);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "inventoryTexture5"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->inventoryTexture[4]);

	glUniform1i(glGetUniformLocation(UIShaderProgram, "textTexture"), 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getPlayer()->textTexture);

	glUniform1i(glGetUniformLocation(UIShaderProgram, "SceneTexture"), 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, finalPPFBO);

	glUniform1i(glGetUniformLocation(UIShaderProgram, "emilFusk"), 9);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, UiMeterTexture.gTexture);

	/*depthMapTransformation = 1;
	glUniform1i(glGetUniformLocation(UIShaderProgram, "depthMapTransformation"), depthMapTransformation);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "shadowMap1"), 10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, shadowMaps[0]);
	glBindVertexArray(depthMapVertexArrayObject[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	depthMapTransformation = 2;
	glUniform1i(glGetUniformLocation(UIShaderProgram, "depthMapTransformation"), depthMapTransformation);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "shadowMap2"), 11);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, shadowMaps[1]);
	glBindVertexArray(depthMapVertexArrayObject[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	depthMapTransformation = 3;
	glUniform1i(glGetUniformLocation(UIShaderProgram, "depthMapTransformation"), depthMapTransformation);
	glUniform1i(glGetUniformLocation(UIShaderProgram, "shadowMap3"), 12);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, shadowMaps[2]);
	glBindVertexArray(depthMapVertexArrayObject[2]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	depthMapTransformation = 0;
	glUniform1i(glGetUniformLocation(UIShaderProgram, "depthMapTransformation"), depthMapTransformation);*/

	glUniform1f(glGetUniformLocation(UIShaderProgram, "hp"), gameScene->gameObjects[0]->getPlayer()->hp);
	glUniform1f(glGetUniformLocation(UIShaderProgram, "cold"), gameScene->gameObjects[0]->getPlayer()->cold);
	glUniform1f(glGetUniformLocation(UIShaderProgram, "water"), gameScene->gameObjects[0]->getPlayer()->water);
	glUniform1f(glGetUniformLocation(UIShaderProgram, "food"), gameScene->gameObjects[0]->getPlayer()->food);
	glUniform1f(glGetUniformLocation(UIShaderProgram, "fade"), gameScene->gameObjects[0]->getPlayer()->fade);
	glUniform1f(glGetUniformLocation(UIShaderProgram, "winFade"), gameScene->gameObjects[0]->getPlayer()->winFade);
	glUniform1f(glGetUniformLocation(UIShaderProgram, "flareTimer"), gameScene->gameObjects[0]->getPlayer()->flareTimer);

	//glBindTexture(GL_TEXTURE_2D, finalPPFBO);
	renderQuad();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



	clearObjectsToRender();
	Update();
}

void RenderManager::renderMainMenu()
{
	FindObjectsToRender();

	if (!gameScene->gameObjects[0]->addedMenuMusic)
	{
		gameScene->gameObjects[0]->menuMusic.addSound("SubtextCorrect.ogg");
		gameScene->gameObjects[0]->addedMenuMusic = true;
	}
	if(!gameScene->gameObjects[0]->menuMusic.isPlaying())
	{
		gameScene->gameObjects[0]->menuMusic.playSound();
		gameScene->gameObjects[0]->menuMusic.setVolume(80.0f);
	}

	view_matrix = gameScene->gameObjects[0]->getViewMatrix();
	//projection_matrix = glm::perspective(glm::radians(60.0f), float(display_w) / float(display_h), 0.1f, 100.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.749f, 0.843f, 0.823f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(mainMenuShaderProgram);

	gameScene->gameObjects[0]->getMenuScene()->buttonTransformations = 1;
	glBindVertexArray(buttonVertexArrayObject[0]);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "textureToUse"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->startButtonTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "loadingTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->loadingTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "buttonTransformation"), gameScene->gameObjects[0]->getMenuScene()->buttonTransformations);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling1"), gameScene->gameObjects[0]->getMenuScene()->scaling1);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling2"), gameScene->gameObjects[0]->getMenuScene()->scaling2);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling3"), gameScene->gameObjects[0]->getMenuScene()->scaling3);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	gameScene->gameObjects[0]->getMenuScene()->buttonTransformations = 2;
	glBindVertexArray(buttonVertexArrayObject[1]);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "textureToUse"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->settingsButtonTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "loadingTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->loadingTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "buttonTransformation"), gameScene->gameObjects[0]->getMenuScene()->buttonTransformations);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling1"), gameScene->gameObjects[0]->getMenuScene()->scaling1);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling2"), gameScene->gameObjects[0]->getMenuScene()->scaling2);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling3"), gameScene->gameObjects[0]->getMenuScene()->scaling3);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "fade"), gameScene->gameObjects[0]->getMenuScene()->fade);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	gameScene->gameObjects[0]->getMenuScene()->buttonTransformations = 3;
	glBindVertexArray(buttonVertexArrayObject[2]);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "textureToUse"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->exitButtonTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "loadingTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->loadingTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "buttonTransformation"), gameScene->gameObjects[0]->getMenuScene()->buttonTransformations);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling1"), gameScene->gameObjects[0]->getMenuScene()->scaling1);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling2"), gameScene->gameObjects[0]->getMenuScene()->scaling2);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling3"), gameScene->gameObjects[0]->getMenuScene()->scaling3);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	gameScene->gameObjects[0]->getMenuScene()->buttonTransformations = 0;
	glBindVertexArray(quadVertexArrayObject);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "textureToUse"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->backgroundTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "loadingTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gameScene->gameObjects[0]->getMenuScene()->loadingTexture);
	glUniform1i(glGetUniformLocation(mainMenuShaderProgram, "buttonTransformation"), gameScene->gameObjects[0]->getMenuScene()->buttonTransformations);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling1"), gameScene->gameObjects[0]->getMenuScene()->scaling1);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling2"), gameScene->gameObjects[0]->getMenuScene()->scaling2);
	glUniform1f(glGetUniformLocation(mainMenuShaderProgram, "scaling3"), gameScene->gameObjects[0]->getMenuScene()->scaling3);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//gameScene->gameObjects[0].getMenuScene()->deleteObjects();
	clearObjectsToRender();
	Update();
	glDisable(GL_BLEND);
}

void RenderManager::renderQuad()
{
	if (vao == 0)
	{
		vertexPos = glGetAttribLocation(lightpassShaderProgram, "aPos");
		uvPos = glGetAttribLocation(lightpassShaderProgram, "aTexCoords");

		//create vertices
		QuadVertex vertices[] = {
			// pos and normal and uv for each vertex
			{ 1,  1, 1.0f, 1.0f },
		{ 1, -1, 1.0f, 0.0f },
		{ -1, -1, 0.0f, 0.0f },
		{ -1,  1, 0.0f, 1.0f },
		};

		unsigned int indices[] = {
			0,1,3,
			1,2,3,
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
			sizeof(QuadVertex),
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
			sizeof(QuadVertex),
			BUFFER_OFFSET(sizeof(float) * 2)
		);

		//ebo
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void RenderManager::renderPPQuad()
{
	if (vao == 0)
	{
		vertexPos = glGetAttribLocation(refractionShaderProgram, "aPos");
		uvPos = glGetAttribLocation(refractionShaderProgram, "aTexCoords");

		//create vertices
		QuadVertex vertices[] = {
			// pos and normal and uv for each vertex
			{ 1,  1, 1.0f, 1.0f },
		{ 1, -1, 1.0f, 0.0f },
		{ -1, -1, 0.0f, 0.0f },
		{ -1,  1, 0.0f, 1.0f },
		};

		unsigned int indices[] = {
			0,1,3,
			1,2,3,
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
			sizeof(QuadVertex),
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
			sizeof(QuadVertex),
			BUFFER_OFFSET(sizeof(float) * 2)
		);

		//ebo
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}


void RenderManager::calculateOrthoProjectionMatrices(unsigned int shaderToUse)
{
	// ----------========== CASCADED SHADOWS ==========----------

	glUseProgram(shaderToUse);

	tempView = glm::lookAt(gameScene->gameObjects[0]->transform->position, glm::vec3(gameScene->gameObjects[0]->transform->position.x - gameScene->gameObjects[0]->transform->forward.x, gameScene->gameObjects[0]->transform->position.y, gameScene->gameObjects[0]->transform->position.z - gameScene->gameObjects[0]->transform->forward.z), gameScene->gameObjects[0]->transform->up);
	inverseViewMatrix = glm::inverse(tempView);
	
	shadowMapLightPosition = glm::vec3(gameScene->gameObjects[0]->transform->position.x, 6.5, gameScene->gameObjects[0]->transform->position.z);
	shadowMapDirection = glm::vec3(gameScene->gameObjects[0]->transform->position.x, gameScene->gameObjects[0]->transform->position.y, gameScene->gameObjects[0]->transform->position.z);
	
	lightView = glm::lookAt(
		glm::vec3(shadowMapLightPosition), // Position, 8 units above PlayerPos
		glm::vec3(shadowMapDirection), // Direction, 1 unit in front of PlayerPos
		glm::vec3(1.0, 0.0, 0.0)); // Up

	
	float fieldOfView = 90.0f;
	float aspectRatio = (float)display_h / (float)display_w;
	float tanHalfHeightFOV = tanf(glm::radians(fieldOfView / 2.0f));
	float tanHalfWidthFOV = tanf(glm::radians((fieldOfView * aspectRatio) / 2.0f));

	for (int i = 0; i < CASCADESPLITS; i++)
	{
		float xn = cascadePlaneEnds[i] * tanHalfHeightFOV;
		float xf = cascadePlaneEnds[i + 1] * tanHalfHeightFOV;
		float yn = cascadePlaneEnds[i] * tanHalfWidthFOV;
		float yf = cascadePlaneEnds[i + 1] * tanHalfWidthFOV;

		glm::vec4 frustrumCorners[8]

		{
			// Near Plane
			glm::vec4(xn, yn, cascadePlaneEnds[i], 1.0f),
			glm::vec4(-xn, yn, cascadePlaneEnds[i], 1.0f),
			glm::vec4(xn, -yn, cascadePlaneEnds[i], 1.0f),
			glm::vec4(-xn, -yn, cascadePlaneEnds[i], 1.0f),

			// Far Plane
			glm::vec4(xf, yf, cascadePlaneEnds[i + 1], 1.0f),
			glm::vec4(-xf, yf, cascadePlaneEnds[i + 1], 1.0f),
			glm::vec4(xf, -yf, cascadePlaneEnds[i + 1], 1.0f),
			glm::vec4(-xf, -yf, cascadePlaneEnds[i + 1], 1.0f)
		};

		float minX = 200000.0;
		float maxX = -200000.0;
		float minY = 200000.0;
		float maxY = -200000.0;
		float minZ = 200000.0;
		float maxZ = -200000.0;

		for (int j = 0; j < 8; j++)
		{
			glm::vec4 vW = inverseViewMatrix * frustrumCorners[j];
			worldSpaceFrustrumCorners[j] = lightView * vW;

			minX = min(minX, worldSpaceFrustrumCorners[j].x);
			maxX = max(maxX, worldSpaceFrustrumCorners[j].x);
			minY = min(minY, worldSpaceFrustrumCorners[j].y);
			maxY = max(maxY, worldSpaceFrustrumCorners[j].y);
			minZ = min(minZ, worldSpaceFrustrumCorners[j].z);
			maxZ = max(maxZ, worldSpaceFrustrumCorners[j].z);
		}

		shadowOrthoProjInfo[i][0] = maxX;
		shadowOrthoProjInfo[i][1] = minX;
		shadowOrthoProjInfo[i][2] = maxY;
		shadowOrthoProjInfo[i][3] = minY;
		shadowOrthoProjInfo[i][4] = maxZ;
		shadowOrthoProjInfo[i][5] = minZ;
		
		vView = glm::vec4(0.0f, 0.0f, cascadePlaneEnds[i + 1], 1.0f);
		vClip = projection_matrix * vView;
		cascadesInClipSpace[i] = -vClip.z;
	}
}

void RenderManager::setOrthoProjectionMatrix(int index)
{
	shadowWorldMatrix = glm::mat4(1.0);
	lightProjection = glm::ortho(shadowOrthoProjInfo[index][1], shadowOrthoProjInfo[index][0], shadowOrthoProjInfo[index][3], shadowOrthoProjInfo[index][2], shadowOrthoProjInfo[index][5], shadowOrthoProjInfo[index][4]);

	lightSpaceMatrices[index] = lightProjection * lightView * shadowWorldMatrix;
}

void RenderManager::renderDepthQuadsForVisualization()
{
	float depthMapQuadVertices1[] =
	{
		-0.3f,  0.3f, 0.0f, 1.0f, //same
		-0.3f, -0.3f, 0.0f, 0.0f,
		0.3f, -0.3f,  1.0f, 0.0f, //same
	
		-0.3f,  0.3f, 0.0f, 1.0f, //same
		0.3f, -0.3f,  1.0f, 0.0f, //same
		0.3f,  0.3f,  1.0f, 1.0f
	
	};
	float depthMapQuadVertices2[] =
	{
		-0.3f,  0.3f, 0.0f, 1.0f, //same
		-0.3f, -0.3f, 0.0f, 0.0f,
		0.3f, -0.3f,  1.0f, 0.0f, //same

		-0.3f,  0.3f, 0.0f, 1.0f, //same
		0.3f, -0.3f,  1.0f, 0.0f, //same
		0.3f,  0.3f,  1.0f, 1.0f

	};
	float depthMapQuadVertices3[] =
	{
		-0.3f,  0.3f, 0.0f, 1.0f, //same
		-0.3f, -0.3f, 0.0f, 0.0f,
		0.3f, -0.3f,  1.0f, 0.0f, //same

		-0.3f,  0.3f, 0.0f, 1.0f, //same
		0.3f, -0.3f,  1.0f, 0.0f, //same
		0.3f,  0.3f,  1.0f, 1.0f

	};
	glGenVertexArrays(1, &depthMapVertexArrayObject[0]);
	glGenBuffers(1, &depthMapBufferObject[0]);
	glBindVertexArray(depthMapVertexArrayObject[0]);
	glBindBuffer(GL_ARRAY_BUFFER, depthMapBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(depthMapQuadVertices1), &depthMapQuadVertices1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenVertexArrays(1, &depthMapVertexArrayObject[1]);
	glGenBuffers(1, &depthMapBufferObject[1]);
	glBindVertexArray(depthMapVertexArrayObject[1]);
	glBindBuffer(GL_ARRAY_BUFFER, depthMapBufferObject[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(depthMapQuadVertices2), &depthMapQuadVertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenVertexArrays(1, &depthMapVertexArrayObject[3]);
	glGenBuffers(1, &depthMapBufferObject[3]);
	glBindVertexArray(depthMapVertexArrayObject[3]);
	glBindBuffer(GL_ARRAY_BUFFER, depthMapBufferObject[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(depthMapQuadVertices3), &depthMapQuadVertices3, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void RenderManager::renderFireParticles()
{
	glBindVertexArray(fireVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//Positions : center
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, fireParticlePositionBuffer);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//Colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, fireParticleColorBuffer);
	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

void RenderManager::renderLighterParticles()
{
	glBindVertexArray(lighterVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, lighterVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//Positions : center
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, lighterParticlePositionBuffer);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//Colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, lighterParticleColorBuffer);
	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

void RenderManager::renderSnowParticles()
{
	glBindVertexArray(snowVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, snowVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//Positions : center
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, snowParticlePositionBuffer);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//Colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, snowParticleColorBuffer);
	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

void RenderManager::renderFlareParticles()
{
	glBindVertexArray(flareVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, flareVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//Positions : center
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, flareParticlePositionBuffer);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	//Colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, flareParticleColorBuffer);
	glVertexAttribPointer(
		2,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

void RenderManager::dayNightCycle()
{
	if (time > 180 && dayOrNight)
	{

		daylight -= deltaTime * 0.02f;
		if (daylight < 0.1f)
		{
			daylight = 0.1f;
			dayOrNight = false;
			time = 0;
		}
	}
	else if (time > 90 && !dayOrNight)
	{
		daylight += deltaTime * 0.02f;
		if (daylight > 1)
		{
			daylight = 1;
			dayOrNight = true;
			time = 0;
		}
	}
	else
	{
		time += deltaTime;
		if(!dayOrNight)
			gameScene->gameObjects[0]->getPlayer()->wolfHowl(time);
	}
}

void RenderManager::ParticleLinearSort(Particle* arr, int size)
{
	int a, b;
	float key;
	for (a = 0; a < size; a++)
	{
		key = arr[a].life;
		b = a - 1;

		while (b >= 0 && arr[b].life > key)
		{
			arr[b + 1] = arr[b];
			b = b - 1;
		}
		arr[b + 1].life = key;
	}
}

void RenderManager::bindForWriting(int cascadeIndex)
{
	assert(cascadeIndex < 3);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMaps[cascadeIndex], 0);
}

int RenderManager::FindUnusedParticle(Particle* container, int lastUsedParticle)
{
	for (int i = lastUsedParticle; i < MAX_PARTICLES; i++)
	{
		if (container[i].life < 0)
		{
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++)
	{
		if (container[i].life < 0)
		{
			return i;
		}
	}
	return 0;
}

void RenderManager::Update()
{

}

void RenderManager::setDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}

void RenderManager::setSeconds(float seconds)
{
	float tempSec = seconds / 1000;
	this->seconds += tempSec;
}