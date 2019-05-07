#version 440 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gAlbedo;
layout(location = 2) out vec3 gNormal;

in vec3 normal;
in vec4 fragpos;

uniform sampler2D gAlbedoIn;

uniform int foundAlbedo;

void main () 
{

	gPosition = fragpos.xyz;

	//gAlbedo = vec3(0.86, 0.94, 0.92);
	gAlbedo = vec3(0.82, 0.92, 0.953);

	gNormal = -normalize(normal);

}