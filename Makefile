build: engine.o
	c99 -l glfw -l OpenGL -l GLEW -lm engine.o -o output

run: build
	./output

engine.o: engine.c
	c99 -c engine.c
