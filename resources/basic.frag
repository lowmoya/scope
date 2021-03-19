#version 400 core

smooth in float depth;

out vec4 color;

void main()
{
	color = vec4(depth, depth, depth, 1);
}
