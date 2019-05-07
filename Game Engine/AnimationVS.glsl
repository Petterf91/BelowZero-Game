#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_normal;
layout(location = 2) in vec3 uv_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in vec4 weights;
layout(location = 6) in vec4 boneID;


out VS_OUT {
	vec3 vertex_normal;
	vec2 uv_coord;
	vec3 tangent;
	vec3 bitangent;
} vs_out;


out vec3 normal;
out vec3 fragPos;
out vec2 textureCoordinate;
out vec3 CamPos;

const int MAX_BONES = 50;
uniform mat4 Bones[MAX_BONES];

uniform mat4 FullTransformMatrix1;
uniform mat4 ModelMatrix1;
uniform mat4 CameraMatrix1;
uniform mat4 ProjectionMatrix1;
uniform vec3 camPos1;



void main() 
{
	float WSum = weights.x + weights.y + weights.z + weights.w;
	
	vec3 finalPos = vec3(0.0, 0.0, 0.0);
	We = Weights;
	Id = BoneID;

	mat4 BMatrix = Bones[int(boneID.x)] * weights.x;
	BMatrix += Bones[int(boneID.y)] * weights.y;
	BMatrix += Bones[int(boneID.z)] * weights.z;
	BMatrix += Bones[int(boneID.w)] * weights.w;

	CamPos = camPos1;
	fragPos = vec3(ModelMatrix1 * vec4(vertex_position, 1.0));

	gl_Position = gs_in[i].projection_matrix * gs_in[i].view_matrix * gs_in[i].world_matrix * BMatrix * vec4(vertex_position, 1.0);


}