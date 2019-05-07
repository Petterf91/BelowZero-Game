#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

bool Vertex::operator==(const Vertex & other)
{
	bool isEqual = false;

	if (this->x == other.x && this->y == other.y && this->z == other.z && this->u == other.u && this->v == other.v && this->nx == other.nx && this->ny == other.ny && this->nz == other.z) {
		isEqual = true;
	}

	return isEqual;
}
