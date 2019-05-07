#version 440 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;


out vec2 TexCoords;
out vec3 cornerPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	cornerPos = gl_Position.xyz;
	TexCoords = aTexCoords;
}