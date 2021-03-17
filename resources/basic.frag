#version 400 core

in vec4 pass_position;

out vec4 color;

void main()
{
	color = pass_position;
}
