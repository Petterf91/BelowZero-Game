#pragma once
#include <vector>
using namespace std;

struct Counter
{
	unsigned int vertexCount;
	unsigned int boundingBoxCount;
	unsigned int jointCount;
	unsigned int animationCount;
	unsigned int blendShapeCount;
	unsigned int groupID;
	unsigned int customMayaAttributeCount;
	unsigned int levelObjectCount;
};

struct MeshInfo {
	float globalTranslation[3];
	float globalRotation[3];
	float globalScaling[3];
};

struct VertexInformation {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
	float weight[4];
	float weightID[4];
};

struct MaterialInformation {
	float ambient[3];
	float diffuse[3];
	float emissive[3];
	float opacity;
	char* textureFilePath[100];
};

struct BoundingBox
{
	float minVector[3];
	float maxVector[3];
	float center[3];
};

struct JointInformation {
	char jointName[100];
	char parentName[100];
	float localTransformMatrix[16];
	float bindPoseMatrix[16];
};

struct KeyFrameData {
	float position[3];
	float rotation[4];
	float scaling[3];
};

struct KeyFrame {
	float time;
	vector <KeyFrameData> keyFrameData;
};

struct AnimationInformation {
	char animationName[100];
	int keyFrameCount;
	vector <KeyFrame> keyFrames;
};

struct BlendShapeVertex {
	float position[3];
};

struct BlendShape {
	float time;
	int blendShapeVertexCount;
	vector <BlendShapeVertex> blendShapeVertices;
};

struct Group {
	char groupName[100];
	char parentName[100];
};

struct CustomMayaAttributes {
	unsigned int meshType;
};

struct LevelObject {
	float x, y, z;
	float rotationX, rotationY, rotationZ;
	int id;
};