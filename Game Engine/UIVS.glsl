#version 440 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 texCoords;
//uniform int depthMapTransformation;


void main()
{
	/*
	if (depthMapTransformation == 1)
		gl_Position = vec4(aPos.x - 0.7, aPos.y + 0.7, 0.0, 1.0);
	else if (depthMapTransformation == 2)
		gl_Position = vec4(aPos.x - 0.1, aPos.y + 0.7, 0.0, 1.0);
	else if (depthMapTransformation == 3)
		gl_Position = vec4(aPos.x + 0.5, aPos.y + 0.7, 0.0, 1.0);*/
	//else if (depthMapTransformation == 0)
	//{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	//}
	texCoords = aTexCoords;
}