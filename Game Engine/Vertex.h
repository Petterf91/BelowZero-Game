#pragma once

// this is how we will structure the input data for the vertex shader
// every six floats, is one vertex.

class Vertex
{
public:
	Vertex();
	~Vertex();

	float x, y, z;
	float nx, ny, nz;
	float u, v;
	float tx, ty, tz;
	float bx, by, bz;

	bool Vertex::operator==(const Vertex &other);

private:

};
