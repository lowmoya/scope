#version 400 core

layout (location = 0) in vec3 position;

smooth out float depth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = view * projection * vec4(position, 1);
	depth = (gl_Position.z + 1) / 2;
}
