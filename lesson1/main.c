/** This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <SDL.h>

#define WINDOW_TITLE "Lesson 1"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct SDLContext {
	int width;                /* viewport width */
	int height;               /* viewport height */
	SDL_Window *window;       /* SDL window handle */
	SDL_GLContext glContext;  /* OpenGL context handle */
} SDLContext;

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

void setViewport(SDLContext* context) {
	glViewport(0, 0, context->width, context->height);
}

int initGL(SDLContext* context) {
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); /* accept fragment if it closer to the camera than the former one */

	setViewport(context);

	return 1;
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear buffer */

	/* draw calls */
}

void startRenderLoop(SDLContext* context) {
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

				setViewport(context);
			}
		}

		drawScene();

		SDL_GL_SwapWindow(context->window); /* update display */
	}
}

void cleanUp(SDLContext* context) {
	if (context->window != NULL) {
		if (context->glContext != NULL) {
			SDL_GL_DeleteContext(context->glContext);
		}

		SDL_DestroyWindow(context->window);
	}

	SDL_Quit();
}

int main() {
	SDLContext context;

	/* check whether OpenGL 3.2 is supported */
	if (GL_VERSION_3_2) {
		context.window = NULL;
		context.glContext = NULL;

		if (initSDL(&context, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) && initGL(&context)) {
			startRenderLoop(&context);
		}
		cleanUp(&context);
	} else {
		printf("OpenGL 3.2 is not supported!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
