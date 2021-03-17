build: engine.o
	c99 -l glfw -l OpenGL -l GLEW engine.o -o output

engine.o: engine.c
	c99 -c engine.c
