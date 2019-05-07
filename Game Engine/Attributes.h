#pragma once
#include <GL/gl3w.h>  
#include"Component.h"

class Attributes :public Component
{
public:
	Attributes();
	Attributes(bool interactive, bool equipment);
	~Attributes();

	bool interactive;
	bool equipment;

	bool isInteractive();
	bool isEquipment();
private:

};

