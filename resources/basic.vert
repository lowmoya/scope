#version 400 core

layout (location = 0) in vec3 position;

out vec4 pass_position;

uniform mat4 u_m_perspective;

void main()
{
	pass_position = vec4(position, 1.0);
	gl_Position = pass_position * u_m_perspective;
}
