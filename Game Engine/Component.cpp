#include "Component.h"

Component::Component()
{
	this->id = ComponentType::ID::COMPONENT;
}

Component::~Component()
{

}

bool Component::operator==(const Component & rhs)
{
	bool value = false;

	if (this->assetName == rhs.assetName) {
		value = true;
	}
	return value;
}

void Component::setInformation(float time)
{

}


