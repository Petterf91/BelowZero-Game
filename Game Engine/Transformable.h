#pragma once
#include "Component.h"

#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/glm.hpp"

/*
This class gives the sub-Class the ability to manipulate the transform struct
in the GameObject class. This is needed for objects that need to move around in the scene. 

Transform& Transform can be called upon in the subclass by using Transformable::transform;
*/

struct Transform
{
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::vec3(1, 0, 0);

	glm::vec3 velocity = glm::vec3(0, 0, 0); //used in collision detection

	glm::vec3 getLighterPosition()
	{
		glm::vec3 lighterPosition;
		lighterPosition.x = position.x;
		lighterPosition.y = position.y;
		lighterPosition.z = position.z;

		lighterPosition.x += forward.x * 1.7f;
		lighterPosition.y += forward.y * 1.7f;
		lighterPosition.z += forward.z * 1.7f;

		lighterPosition.x -= right.x * 0.36f;
		lighterPosition.y -= right.y * 0.36f;
		lighterPosition.z -= right.z * 0.36f;

		lighterPosition.x -= up.x * 0.36f;
		lighterPosition.y -= up.y * 0.36f;
		lighterPosition.z -= up.z * 0.36f;

		return lighterPosition;
	}
};

class Transformable : public Component
{
public:
	Transformable(Transform& transform);
	virtual ~Transformable();

	Transform& transform;
};

