#include <math.h>
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
	length = ftell(file) + 1;
	rewind(file);

	contents = malloc(length);
	for (int i = 0; i < length; i++)
	{
		contents[i] = fgetc(file);
	}
	contents[length - 1] = '\0';

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
		{ 7,  7,  20},
		{-7, -7,  10},
		{-7,  7,  20},
		{ 7,  7,  20},
		{ 7, -7,  10},
		{-7, -7,  10},

		/*{ 30, -30,  60},
		{-30, -30,  0},
		{-30, -30,  60},
		{ 30, -30,  60},
		{ 30, -30,  0},
		{-30, -30,  0},*/
	};

	/* work on ibo */

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Position), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	unsigned int program_handle = program_create("resources/basic.vert", "resources/basic.frag");
	glUseProgram(program_handle);

#define FAR 100.0
#define NEAR 0.0
#define ASPECT (4.0/3.0)
#define THF 1
#define PI 3.1415

	float perspective[16] = { 0.0 };
	perspective[0 + (0 * 4)] = 1.0 / (ASPECT * THF);
	perspective[1 + (1 * 4)] = 1.0 / THF;
	perspective[2 + (2 * 4)] = 2.0 / (FAR-NEAR);
	perspective[2 + (3 * 4)] = -(FAR + NEAR)/(FAR - NEAR);
	perspective[3 + (2 * 4)] = 1.0;
	perspective[3 + (3 * 4)] = -NEAR + 1.0;

	float view[16] = { 0.0 };
	view[0 + (0 * 4)] = 1.0;
	view[1 + (1 * 4)] = 1.0;
	view[2 + (2 * 4)] = 1.0;
	view[3 + (3 * 4)] = 1.0;

	/* Rotate on Z
	view[0 + (0 * 4)] = cos(PI/4);
	view[0 + (1 * 4)] = -sin(PI/4);
	view[1 + (0 * 4)] = sin(PI/4);
	view[1 + (1 * 4)] = cos(PI/4);
	*/

	int perspective_location = glGetUniformLocation(program_handle, "projection");
	int view_location = glGetUniformLocation(program_handle, "view");
	glUniformMatrix4fv(perspective_location, 1, GL_FALSE, perspective);
	glUniformMatrix4fv(view_location, 1, GL_FALSE, view);

	float rot_var = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

	view[0 + (0 * 4)] = cos(PI * rot_var);
	view[0 + (1 * 4)] = -sin(PI * rot_var);
	view[1 + (0 * 4)] = sin(PI* rot_var);
	view[1 + (1 * 4)] = cos(PI* rot_var);
	/*	
	view[1 + (1 * 4)] = cos(PI * rot_var);
		view[1 + (2 * 4)] = -sin(PI * rot_var);
		view[2 + (1 * 4)] = sin(PI * rot_var);
		view[2 + (2 * 4)] = cos(PI * rot_var);
		*/
		glUniformMatrix4fv(view_location, 1, GL_FALSE, view);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);

		glfwPollEvents();
		rot_var += ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				- (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) * 1.0/32.0;

	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program_handle);
	glfwTerminate();
	return 0;
}
