#version 440 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gAlbedo;
layout(location = 2) out vec3 gNormal;

in GS_OUT{
	vec4 vertex_normal;
	vec4 FragPos;
	vec2 TexCoords;
} fs_in;

uniform sampler2D gAlbedoIn;

void main()
{
	//Normal
	gPosition = fs_in.FragPos.xyz;

	//gAlbedo = vec3(0.5, 0.3, 0.2);
	gAlbedo = texture(gAlbedoIn, fs_in.TexCoords).xyz;

	gNormal = fs_in.vertex_normal.xyz;
}