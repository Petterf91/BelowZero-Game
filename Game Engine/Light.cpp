#include "Light.h"

Light::Light(Transform& transform): Transformable(transform)
{
	assetName = "Light";

	color = glm::vec4(0.85f, 0.85f, 1, 1);
	Linear=0.05f;
	Quadratic=0.15f;
	lightType = -1;
	offset = 0;
	intensity = 1;
	isLighter = false;
	isFlare = false;
}

Light::~Light()
{
}

void Light::update(float deltaTime, float seconds)
{
}

void Light::processEvents(GLFWwindow * window, float deltaTime)
{
}