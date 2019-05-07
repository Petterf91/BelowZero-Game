#pragma once
#include "MeshStructs.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class LeapMesh
{
private:
	void loader(const char* fileName);

public:
	LeapMesh();
	LeapMesh(const char* fileName);
	~LeapMesh();

	std::string fileName;

	int getVertexCount();

	VertexInformation* vertices;
	MeshInfo* transform;
	MaterialInformation* material;
	vector <BoundingBox*> boundingBoxes;
	JointInformation* joints;
	AnimationInformation* animation;
	KeyFrame* keyFrame;
	KeyFrameData* keyFrameData;
	BlendShape* blendShape;
	BlendShapeVertex* blendShapeVertices;
	Group* group;
	CustomMayaAttributes* customMayaAttribute;
	Counter counterReader;
};