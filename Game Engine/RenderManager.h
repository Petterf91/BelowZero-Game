#pragma once
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/glm.hpp"
#include <GL/gl3w.h>   
#include "glfw\include\GLFW\glfw3.h"
#include <vector>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <random>
#include "stb_image.h"
#include "GameScene.h"
#include "ShaderProgramLib.h"
#include "TextureLib.h"
#include "Debug.h"

#include <math.h>
#define STB_IMAGE_IMPLEMENTATION

// Cube Map defines for its positions
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X  0x8515 
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X  0x8516 
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y  0x8517 
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y  0x8518 
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z  0x8519 
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  0x851A

#define VERY_LOW_SHADOW 512
#define LOW_SHADOW 1024
#define MEDIUM_SHADOW 2048
#define HIGH_SHADOW 4096
#define CASCADESPLITS 2
#define MAX_PARTICLES 1000					//Maximum: 1000, Intel NUC can't handle more stack than this
#define LIGHTER_PARTICLES 100

struct QuadVertex
{
	float x, y, u, v;
};

class RenderManager
{
public:
	RenderManager();
	RenderManager(GameScene *otherGameScene, GLFWwindow* otherWindow, ShaderProgramLib* shaderProgram);
	~RenderManager();

	GameScene *gameScene;
	Texture shatteredIce;
	Texture damageTexture;
	Texture UiMeterTexture;
	
	void FindObjectsToRender();
	void clearObjectsToRender();

	void Render();
	void renderMainMenu();

	void createBuffers();
	void createMainMenuBuffer();
	void createButtonQuads();
	void deleteData();
	void renderQuad();
	void renderPPQuad();
	void Update();
	void setDeltaTime(float deltaTime);
	void setSeconds(float seconds);
	void setupMeshY();
	void renderFireParticles();
	void renderSnowParticles();
	void renderFlareParticles();
	void renderLighterParticles();
	void dayNightCycle();

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a;
		float size, angle, weight;
		float life;
		float cameraDistance;
	};

	Particle fireParticleContainer[MAX_PARTICLES];
	Particle snowParticleContainer[MAX_PARTICLES];
	Particle flareParticleContainer[MAX_PARTICLES];
	Particle lighterParticleContainer[MAX_PARTICLES];

	int FindUnusedParticle(Particle* container, int lastUsedParticle);
	void ParticleLinearSort(Particle* arr, int size);

	//Shadows
	void bindForWriting(int cascadeIndex);
	void calculateOrthoProjectionMatrices(unsigned int shaderToUse);
	void setOrthoProjectionMatrix(int index);
	void renderDepthQuadsForVisualization();

private:

	std::vector<GameObject*> gameObjectsToRender;
	std::vector<Light*> lightsToRender;



	glm::mat4x4 view_matrix;
	glm::mat4x4 fpsView_matrix;
	glm::mat4x4 projection_matrix;
	glm::mat4x4 currentCubeMapView;
	glm::mat4 tempMatrix;
	glm::mat4 oldrotation;
	float oldPitch;
	float oldYaw;
	GLuint finalMainMenuFBOTexture;

	//Time constants
	float deltaTime;
	float seconds = 0.0f;
	int count;
	float daylight;
	float time;
	bool dayOrNight;
	bool fireFlicker;
	//Buffer Objects
	unsigned int finalMainMenuFBO;
	unsigned int quadVertexArrayObject;
	unsigned int quadVertexBufferObject;
	unsigned int buttonVertexArrayObject[3];
	unsigned int buttonBufferObject[3];

	//Texture data
	int width, height, nrOfChannels;
	unsigned char* data;

	//Buffers
	unsigned int UIFBO;
	unsigned int UITexture;
	unsigned int shadowFBO;
	unsigned int animationVAO;
	unsigned int animationVBO;
	unsigned int animationEBO;
	unsigned int ebo;
	unsigned int vbo;
	unsigned int vao;
	unsigned int gbo;
	unsigned int rboDepth;
	unsigned int finalFBO;
	unsigned int finalColorBuffer;
	unsigned int finalDepthStensil;
	unsigned int fireTexture;
	unsigned int snowTexture;
	unsigned int flareTexture;
	unsigned int lighterTexture;
	unsigned int fireVAO;
	unsigned int snowVAO;
	unsigned int flareVAO;
	unsigned int lighterVAO;
	unsigned int fireVBO;
	unsigned int snowVBO;
	unsigned int flareVBO;
	unsigned int lighterVBO;
	unsigned int fireParticlePositionBuffer;
	unsigned int snowParticlePositionBuffer;
	unsigned int flareParticlePositionBuffer;
	unsigned int lighterParticlePositionBuffer;
	unsigned int fireParticleColorBuffer;
	unsigned int snowParticleColorBuffer;
	unsigned int flareParticleColorBuffer;
	unsigned int lighterParticleColorBuffer;
	unsigned int PPFBO;
	unsigned int finalPPFBO;

	//VFX
	unsigned int lastUsedParticle = 0;
	unsigned int particleCount = 0;
	unsigned int fireParticles = 1;
	unsigned int snowParticles = 10;
	unsigned int flareParticles = 1;
	unsigned int lighterParticles = 1;
	unsigned int randomizer = 0;
	float randomX;
	float randomY;
	float randomZ;
	float randomDirectionX;
	float randomDirectionZ;
	float defaultX;
	float defaultY;
	float defaultZ;
	float offset;
	float flareShotTimer = 0;
	int distanceToParticles = 0;
	bool flareAlive = false;
	bool pickedUpFlare = false;
	glm::mat4 viewProjectionMatrix;
	glm::vec3 startPoint;
	glm::vec3 lighterPosition;
	glm::vec3 randomStartPoint;
	glm::vec3 targetPoint;
	glm::vec3 particlePivot;
	glm::vec3 directionVec;
	glm::vec3 tempDistance;
	glm::vec3 cameraRight_vector;
	glm::vec3 cameraUp_vector;

	GLfloat* fireParticlePositionData = 0;
	GLfloat* snowParticlePositionData = 0;
	GLfloat* flareParticlePositionData = 0;
	GLfloat* lighterParticlePositionData = 0;
	GLubyte* fireParticleColorData = 0;
	GLubyte* snowParticleColorData = 0;
	GLubyte* flareParticleColorData = 0;
	GLubyte* lighterParticleColorData = 0;

	unsigned int equipedFBO;
	unsigned int equipedTexture;

	unsigned int gAlbedo;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int attachments[3] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
	};

	QuadVertex vertices;
	GLFWwindow* window;

	//Shaders
	GLuint shadowMapShaderProgram;
	GLuint geometryShaderProgram;
	GLuint lightpassShaderProgram;
	GLuint animationShaderProgram;
	GLuint UIShaderProgram;
	GLuint terrainShaderProgram;
	GLuint vfxFireShaderProgram;
	GLuint vfxSnowShaderProgram;
	GLuint vfxFlareShaderProgram;
	GLuint vfxLighterShaderProgram;
	GLuint mainMenuShaderProgram;
	GLuint refractionShaderProgram;

	int display_w, display_h;
	unsigned int vertexPos;
	unsigned int uvPos;

	//Shadow
	glm::vec4 vView;
	glm::vec4 vClip;
	glm::vec4 frustrumCorners[8];
	glm::vec4 worldSpaceFrustrumCorners[8];
	glm::vec3 shadowMapLightPosition;
	glm::vec3 shadowMapDirection;

	glm::mat4 tempView;
	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrices[2];
	glm::mat4 inverseViewMatrix;
	glm::mat4 shadowWorldMatrix;

	float cascadePlaneEnds[3];
	float cascadesInClipSpace[2];
	float shadowOrthoProjInfo[2][6];
	int depthMapTransformation;
	unsigned int shadowMaps[2];

	unsigned int depthMapVertexArrayObject[2];
	unsigned int depthMapBufferObject[2];
};