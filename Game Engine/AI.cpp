#include "AI.h"



AI::AI(Transform& transform) : Transformable(transform)
{
	Component::id = ComponentType::ID::AI;

	this->startPosition = transform.position;
	this->collision = false;
	this->time = 0.f;
	this->bounce = 0.f;
	this->bounceTimer = 0.f;
	this->bounceCounter = 0;

	this->wait = false;
	this->jumping = false;

	lastTarget = glm::vec3(0, 0, 0);
	target = glm::vec3(0, 0, 0);
	direction = glm::vec3(0, 0, 0);
	forward = Transformable::transform.right;
}


AI::~AI()
{

}

void AI::update(float deltaTime, float seconds)
{
	time += deltaTime;
	setBounce(deltaTime);

	if (time > 10.f || collision == true)
	{
		setNewTarget();
	}
	if (time > 4.5f && time < 6.f)
	{

	}
	else
		move(deltaTime);

}

void AI::processEvents(GLFWwindow * window, float deltaTime)
{

}

glm::vec2 AI::getXY()
{
	float u = Transformable::transform.position.x;
	float v = Transformable::transform.position.z;

	return glm::vec2(u, v);
}

void AI::setCurrentHeight(float height)
{
	if (height + 0.001 < Transformable::transform.position.y + 0.002);
		this->Transformable::transform.position.y = height + 0.0;

	this->Transformable::transform.position.y += bounce;
}

void AI::setRotation()
{
	// Calculate angle
	float dot = glm::dot(forward, direction);
	float directionMag = glm::length(direction);
	float forwardMag = glm::length(forward);
	float angle = dot / (directionMag * forwardMag);
	angle = glm::degrees(glm::acos(angle));
	// Set new rotation.y angle
	if (direction.x > 0.f && direction.z > 0.f)				// + , +
		Transformable::transform.rotation.y = angle * -1.f;	// -1.f
	else if (direction.x < 0.f && direction.z > 0.f)		// - , +
		Transformable::transform.rotation.y = angle * -1.f;		
	else if (direction.x > 0.f && direction.z < 0.f)		// + , -
		Transformable::transform.rotation.y = angle;
	else if (direction.x < 0.f && direction.z < 0.f)		// - , -
		Transformable::transform.rotation.y = angle;

	//std::cout << "Rabbit Angle: " << angle << std::endl;
	//std::cout << "Rabbit Direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
}

void AI::setBounce(float deltaTime)
{
	if (wait == true)
	{
		bounce = 0.f;
		bounceTimer += deltaTime;
		if (bounceTimer >= 3.0f)
		{
			bounceCounter = rand() % 3;
			bounceTimer = 0.f;
			wait = false;
		}
	}
	else if (jumping == false && wait == false)
	{
		bounce += JUMP_SPEED * deltaTime;
		if (bounce > JUMP_HEIGHT)
		{
			jumping = true;
		}
	}
	else if (jumping == true && wait == false)
	{
		bounce -= JUMP_SPEED * deltaTime;
		if (bounce < 0.001f)
		{
			jumping = false;
			bounceCounter++;
			if (bounceCounter >= 4)
				wait = true;
		}
	}
}

void AI::move(float deltaTime)
{
	Transformable::transform.position +=  direction * (float)SPEED * deltaTime;
}

void AI::setNewTarget()
{
	glm::vec3 currentPosition = Transformable::transform.position;
	lastTarget = target;
	// Get new target coords
	float x = rand() % SPAWN_OFFSET - SPAWN_OFFSET * 0.5;
	float z = rand() % SPAWN_OFFSET - SPAWN_OFFSET * 0.5;
	// Update target
	target = glm::vec3(startPosition.x + x, startPosition.y, startPosition.z + z);
	// Direction
	direction = glm::normalize(target - currentPosition);
	// Reset timer
	time = 0.f;
	// Set rotation y
	setRotation();
}