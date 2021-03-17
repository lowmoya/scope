#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct 
{
	float x, y, z;
} Position;


char *file_load(char *path)
{
	FILE *file;
	unsigned int length;
	char *contents;
	char temp;

	file = fopen(path, "r");
	if (!file)
	{
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	rewind(file);

	contents = malloc(length);
	for (int i = 0; i < length; i++)
	{
		contents[i] = fgetc(file);
	}

	fclose(file);

	return contents;
}

unsigned int program_create(char *vertex_path, char *fragment_path)
{
	unsigned int program_handle, vertex_handle, fragment_handle;
	char *vertex_source, *fragment_source;
	int status;

	program_handle = glCreateProgram();

	vertex_handle = glCreateShader(GL_VERTEX_SHADER);
	vertex_source = file_load(vertex_path);
	glShaderSource(vertex_handle, 1, (const char **)&vertex_source, NULL);
	glCompileShader(vertex_handle);
	glAttachShader(program_handle, vertex_handle);
	free(vertex_source);

	glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[500];
		glGetShaderInfoLog(vertex_handle, 500, NULL, buffer);
		printf("%s\n", buffer);
	}

	fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
	fragment_source = file_load(fragment_path);
	glShaderSource(fragment_handle, 1, (const char **)&fragment_source, NULL);
	glCompileShader(fragment_handle);
	glAttachShader(program_handle, fragment_handle);
	free(fragment_source);

	glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[500];
		glGetShaderInfoLog(fragment_handle, 500, NULL, buffer);
		printf("%s\n", buffer);
	}

	glLinkProgram(program_handle);
	glDeleteShader(vertex_handle);
	glDeleteShader(fragment_handle);

	glGetProgramiv(program_handle, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[500];
		glGetProgramInfoLog(program_handle, 500, NULL, buffer);
		printf("%s\n", buffer);

		glDeleteProgram(program_handle);
		return 0;
	}
	
	return program_handle;
}

int main()
{
	GLFWwindow *window;

	if (!glfwInit())
	{
		return 1;
	}

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "Pog \\dev", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return 1;
	}

	glClearColor(0.1, 0.2, 0.3, 1.0);

	Position vertices[] =
	{
		{ 30,  30,  -30},
		{-30, -30,  -30},
		{-30,  30,  -30},
		{ 30,  30,  -30},
		{ 30, -30,  -30},
		{-30, -30,  -30},

		{ 30, -30,   30},
		{-30, -30,  -30},
		{-30, -30,   30},
		{ 30, -30,   30},
		{ 30, -30,  -30},
		{-30, -30,  -30},
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Position), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	unsigned int program_handle = program_create("resources/basic.vert", "resources/basic.frag");
	glUseProgram(program_handle);

#define ASPECT 4/3
#define TF 1
#define FAR 100
#define NEAR -1

	float perspective[] = {
		1/(ASPECT * TF), 0, 0, 0,
		0, 1/(TF), 0, 0,
		0, 0, -(FAR + NEAR)/(FAR - NEAR), -(2 * FAR * NEAR)/(FAR - NEAR),
		0,   0, -1, 0
	};

	float rotation[] = {
		

	int location = glGetUniformLocation(program_handle, "u_m_perspective");
	glUniformMatrix4fv(location, 1, GL_FALSE, perspective);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 12);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program_handle);
	glfwTerminate();
	return 0;
}
