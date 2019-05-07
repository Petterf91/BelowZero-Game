#pragma once
#include "glm/glm/gtc/matrix_transform.hpp"
#undef max
#undef min
struct RayData
{
	glm::vec3 rayDirection;
	glm::vec3 rayOrigin;
};

class Ray
{
public:
	Ray() = delete;
	Ray(const Ray&) = delete;
	Ray& operator=(const Ray&) = delete;

	//
	static RayData getWorldRay(const float x, const float y, glm::mat4 viewMatrix, float sceenWidth, float screenHeight, glm::vec3 position)
	{
		/*Change data to glm::vec2, glm::mat4, glm::vec2 ?*/
		/* overload and use glm::vec2 glm::mat4 is we use fps? */
		float xPos = x;
		float yPos = y;

		//? Transform our x and y position into normalized device coordinates
		xPos = (2.f * xPos) / sceenWidth - 1.f;
		yPos = 1.f - (2.f * yPos) / screenHeight; // Reverse the direction of Y
		glm::vec2 normalizedCoords(xPos, yPos);

		//? Make our ray's z to point forward
		glm::vec4 clipCoords(normalizedCoords.x, normalizedCoords.y, -1.f, 1.f);

		//? Transform to eye space
		//? Multiply with the inverse of the matrix
		glm::mat4 projectionMatrix(1);
		glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
		eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);

		//? Transform to world space
		glm::vec4 rayWorld = (glm::inverse(viewMatrix) * eyeCoords);
		rayWorld = glm::normalize(rayWorld);

		RayData rayData;
		rayData.rayDirection = rayWorld;
		rayData.rayOrigin = position;

		/*
		//Calculate rayOrigin from viewMatrix
		glm::mat3 rotMat(viewMatrix);
		glm::vec3 d(viewMatrix[3]);
		rayData.rayOrigin = -d * rotMat;
		
		glm::mat4 world_matrix = glm::mat4(1);
		world_matrix = glm::translate(world_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
		world_matrix = glm::rotate(world_matrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 inverseWorld_matrix = glm::inverse(world_matrix);
		glm::mat4 model_matrix = glm::mat4(1);

		rayData.rayDirection = glm::vec3((inverseWorld_matrix * model_matrix) * glm::vec4(rayData.rayDirection, 0));
		rayData.rayOrigin = glm::vec3((inverseWorld_matrix * model_matrix) * glm::vec4(position, 0));

		rayData.rayDirection = rayData.rayOrigin + (rayData.rayDirection - rayData.rayOrigin);
		*/
		return rayData;
	}
};