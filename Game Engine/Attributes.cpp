#include "Attributes.h"

Attributes::Attributes()
{
}

Attributes::Attributes(bool interactive, bool equipment)
{
	this->interactive;
	this->equipment;
}

Attributes::~Attributes()
{
}

bool Attributes::isInteractive()
{
	return this->interactive;
}

bool Attributes::isEquipment()
{
	return this->equipment;
}
