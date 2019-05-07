#pragma once

#include "Mesh.h"

class Entity
{
public:
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity();
	virtual ~Entity();

	//glm::vec3 getPosition()const;
	//void setPosition(const glm::vec3 position);

	//virtual void update(Time deltaTime) = 0;
	//virtual void processEvents(Window & window) = 0;

private:
	//virtual void draw() const override;
	
	Mesh _mesh;


};

