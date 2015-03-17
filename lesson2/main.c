/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <SDL.h>

#include "math3d.h"
#include "geometry.h"
#include "gl_utils.h"

#define WINDOW_TITLE "Lesson 2"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct SDLContext {
	int width;                /* viewport width */
	int height;               /* viewport height */
	SDL_Window *window;       /* SDL window handle */
	SDL_GLContext glContext;  /* OpenGL context handle */
} SDLContext;

typedef struct GLResources {
	/* meshes */
	Geometry triangle;
	Geometry quad;

	/* matrices */
	Mat4 modelViewMatrix;
	Mat4 projectionMatrix;
	Mat4 modelViewProjectionMatrix;

	/* shader program */
	GLProgram program;

	GLAttribLocation vertexAttribLocation;
	GLAttribLocation colorAttribLocation;

	GLUniformLocation modelViewProjectionMatrixLocation;

	/* vertex array objects */
	GLVertexArray triangleVAO;
	GLVertexArray quadVAO;

	/* vertex buffer objects */
	GLBuffer triangleVertexVBO;
	GLBuffer triangleColorVBO;

	GLBuffer quadVertexVBO;
	GLBuffer quadColorVBO;
} GLResources;


int initGLEW() {
	GLenum glewError;

	glewExperimental = GL_TRUE; /* fixes segfault issue when calling glGenVertexArrays (http://openglbook.com/blog/glgenvertexarrays-access-violationsegfault-with-glew/) */
	glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(glewError));
		return 0;
	}
	return 1;
}

int initSDL(SDLContext* context, int width, int height, const char* title) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { /* initialize the SDL video subsystem */
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return 0;
	}

	/* request an OpenGL 3.2 core context */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); /* enable double buffering */
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  /* set size of z-buffer (you may need to change this to 16 or 32, depending on your system) */

	/* enable anti-aliasing */
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetSwapInterval(1); /* enable VSync */

	context->window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (!context->window) {
		fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
		return 0;
	}

	context->width = width;
	context->height = height;

	return 1;
}

/* Explaination see: https://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_%28C%2B%2B/Win%29 */
void initVertexArrays(GLResources* resources) {
	glGenVertexArrays(1, &resources->triangleVAO.id);
	glGenVertexArrays(1, &resources->quadVAO.id);
}

void initShaders(GLResources* resources) {
	GLchar* vertexShaderSource;
	GLchar* fragmentShaderSource;
	GLShader vertexShader;
	GLShader fragmentShader;

	/* load and compile vertex shader */
	vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
	vertexShaderSource = getShaderSource("shaders/shader.vert");

	glShaderSource(vertexShader.id, 1, (const GLchar**) &vertexShaderSource, NULL);
	glCompileShader(vertexShader.id);
	checkShaderCompileStatus(&vertexShader);

	free(vertexShaderSource);

	/* load and compile fragment shader */
	fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaderSource = getShaderSource("shaders/shader.frag");

	glShaderSource(fragmentShader.id, 1, (const GLchar**) &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader.id);
	checkShaderCompileStatus(&fragmentShader);

	free(fragmentShaderSource);

	/* link shaders to a program */
	resources->program.id = glCreateProgram();

	glAttachShader(resources->program.id, vertexShader.id);
	glAttachShader(resources->program.id, fragmentShader.id);

	glLinkProgram(resources->program.id);
	checkProgramLinkStatus(&resources->program);

	/* delete shaders as they are not needed after the program was linked */
	glDeleteShader(vertexShader.id);
	glDeleteShader(fragmentShader.id);
}

void initBuffers(GLResources* resources) {
	createTriangle(&resources->triangle);
	createQuad(&resources->quad);

	/* upload triangle data */
	glBindVertexArray(resources->triangleVAO.id);

	glGenBuffers(1, &resources->triangleVertexVBO.id);
	glBindBuffer(GL_ARRAY_BUFFER, resources->triangleVertexVBO.id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * resources->triangle.itemSize * resources->triangle.numItems,
		resources->triangle.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
	   resources->vertexAttribLocation.id,
	   resources->triangle.itemSize,
	   GL_FLOAT,
	   GL_FALSE,
	   0,
	   (void*) 0
	);
	glEnableVertexAttribArray(resources->vertexAttribLocation.id);

	/* lesson 2 - upload color values */
	glGenBuffers(1, &resources->triangleColorVBO.id);
	glBindBuffer(GL_ARRAY_BUFFER, resources->triangleColorVBO.id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * resources->triangle.itemSize * resources->triangle.numItems,
		resources->triangle.colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
	   resources->colorAttribLocation.id,
	   resources->triangle.itemSize,
	   GL_FLOAT,
	   GL_FALSE,
	   0,
	   (void*) 0
	);
	glEnableVertexAttribArray(resources->colorAttribLocation.id);

	/* upload quad data */
	glBindVertexArray(resources->quadVAO.id);

	glGenBuffers(1, &resources->quadVertexVBO.id);
	glBindBuffer(GL_ARRAY_BUFFER, resources->quadVertexVBO.id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * resources->quad.itemSize * resources->quad.numItems,
		resources->quad.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
	   resources->vertexAttribLocation.id,
	   resources->quad.itemSize,
	   GL_FLOAT,
	   GL_FALSE,
	   0,
	   (void*) 0
	);
	glEnableVertexAttribArray(resources->vertexAttribLocation.id);

	/* lesson 2 - upload color values */
	glGenBuffers(1, &resources->quadColorVBO.id);
	glBindBuffer(GL_ARRAY_BUFFER, resources->quadColorVBO.id);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GLfloat) * resources->quad.itemSize * resources->quad.numItems,
		resources->quad.colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
	   resources->colorAttribLocation.id,
	   resources->quad.itemSize,
	   GL_FLOAT,
	   GL_FALSE,
	   0,
	   (void*) 0
	);
	glEnableVertexAttribArray(resources->colorAttribLocation.id);
}

void setViewport(SDLContext* context, GLResources* resources) {
	glViewport(0, 0, context->width, context->height);

	/* recalculate the projection matrix accordingly */
	mat4Perspective(
		&resources->projectionMatrix,
		45.0, ((double)context->width) / ((double)context->height),
		0.1, 100.0);
}

int initGL(SDLContext* context, GLResources* resources) {
	/* create the OpenGL context within our window */
	context->glContext = SDL_GL_CreateContext(context->window);
	if (context->glContext == NULL) {
		fprintf(stderr, "Failed to initialize OpenGL context: %s\n", SDL_GetError());
		return 0;
	}
	if (!initGLEW()) {
		return 0;
	}

	/* basic OpenGL setup */
	glClearColor(0.0, 0.0, 0.0, 1.0); /* tell OpenGL which color to use for clearing the pixel buffer (black) */
	glEnable(GL_MULTISAMPLE); /* enables anti-aliasing */
	/* lesson4 */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); /* accept fragment if it closer to the camera than the former one */

	setViewport(context, resources);

	initVertexArrays(resources);
	initShaders(resources);

	/* enable the shader program */
	glUseProgram(resources->program.id);
	if (glGetError() != GL_NO_ERROR) {
		printProgramInfoLog(&resources->program);
	}

	/* get attribute and uniform locations */
	resources->vertexAttribLocation.id = glGetAttribLocation(resources->program.id, "vertexAttrib");
	resources->colorAttribLocation.id  = glGetAttribLocation(resources->program.id, "colorAttrib");

	resources->modelViewProjectionMatrixLocation.id = glGetUniformLocation(resources->program.id, "modelViewProjectionMatrix");

	initBuffers(resources);

	return 1;
}

void setMatrixUniforms(GLResources* resources) {
	glUniformMatrix4fv(resources->modelViewProjectionMatrixLocation.id, 1, GL_FALSE, resources->modelViewProjectionMatrix.data);
}

void drawScene(GLResources* resources) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear buffer */

	mat4Identity(&resources->modelViewMatrix);

	/* render triangle */
	mat4Translate(&resources->modelViewMatrix, -1.5f, 0.0f, -6.0f);
	mat4Multiply(&resources->modelViewProjectionMatrix, &resources->projectionMatrix, &resources->modelViewMatrix);
	setMatrixUniforms(resources);

	glBindVertexArray(resources->triangleVAO.id);
	glDrawArrays(GL_TRIANGLES, 0, resources->triangle.numItems);

	/* render quad */
	mat4Translate(&resources->modelViewMatrix, 3.0f, 0.0f, 0.0f);
	mat4Multiply(&resources->modelViewProjectionMatrix, &resources->projectionMatrix, &resources->modelViewMatrix);
	setMatrixUniforms(resources);

	glBindVertexArray(resources->quadVAO.id);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, resources->quad.numItems);
}

void startRenderLoop(SDLContext* context, GLResources* resources) {
	SDL_Event event;
	int run = 1, fullscreen = 0;

	while (run) { /* main loop */
		while (SDL_PollEvent(&event)) { /* check for new events */
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
				run = 0;
			} else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F11) {
				SDL_SetWindowFullscreen(context->window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
				fullscreen = !fullscreen;
			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				context->width = event.window.data1;
				context->height = event.window.data2;

				setViewport(context, resources);
			}
		}

		drawScene(resources);

		SDL_GL_SwapWindow(context->window); /* update display */
	}
}

void cleanUp(SDLContext* context, GLResources* resources) {
	if (context->window != NULL) {
		if (context->glContext != NULL) {
			SDL_GL_DeleteContext(context->glContext);

			glDeleteProgram(resources->program.id);

			glDeleteBuffers(1, &resources->triangleVertexVBO.id);
			glDeleteBuffers(1, &resources->triangleColorVBO.id);
			glDeleteBuffers(1, &resources->quadVertexVBO.id);
			glDeleteBuffers(1, &resources->quadColorVBO.id);

			glDeleteVertexArrays(1, &resources->triangleVAO.id);
			glDeleteVertexArrays(1, &resources->quadVAO.id);

			deleteGeometry(&resources->triangle);
			deleteGeometry(&resources->quad);
		}

		SDL_DestroyWindow(context->window);
	}

	SDL_Quit();
}

int main() {
	SDLContext context;
	GLResources resources;

	/* check whether OpenGL 3.2 is supported */
	if (GL_VERSION_3_2) {
		context.window = NULL;
		context.glContext = NULL;

		if (initSDL(&context, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) && initGL(&context, &resources)) {
			startRenderLoop(&context, &resources);
		}
		cleanUp(&context, &resources);
	} else {
		printf("OpenGL 3.2 is not supported!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
