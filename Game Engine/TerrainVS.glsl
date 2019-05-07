#version 440 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;


uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 normal;
out vec4 fragpos;


void main()
{
	fragpos = world_matrix * vec4(vertex_position, 1.0);
	
	normal = vec4(world_matrix * vec4(vertex_normal, 0.0)).xyz;
	
	gl_Position = projection_matrix * view_matrix * world_matrix * vec4(vertex_position, 1.0);

}