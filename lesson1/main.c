#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <SDL.h>

#include "math3d.h"
#include "geometry.h"
#include "gl_shader.h"
#include "gl_program.h"
#include "gl_buffer_object.h"
#include "gl_vertex_array.h"

#define WINDOW_TITLE "Lesson 1"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct sdl_render_context {
	GLsizei width;
	GLsizei height;
	SDL_Window *window; /* SDL window handle */
	SDL_GLContext gl_context; /* OpenGL context handle */
} sdl_render_context;

typedef struct gl_data {
	gl_program program;

	mat4 model_view_matrix;
	mat4 projection_matrix;

	geometry triangle;
	geometry quad;

	gl_vertex_array vertex_array;

	gl_buffer_object triangle_vertex_buffer;
	gl_buffer_object quad_vertex_buffer;
} gl_data;

int initSDL(sdl_render_context* context, int width, int height, const char* title) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { /* Initialize SDL's Video subsystem */
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		SDL_ClearError(); // necessary?
		return 0;
	}

	/* request an OpenGL 3.2 core context */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); /* enable double buffering */
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); /* set size of z-buffer (you may need to change this to 16 or 32 for your system) */

	// enable anti-aliasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetSwapInterval(1); /* enable VSync */

	context->window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!context->window) {
		fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
		SDL_ClearError(); // TODO necessary?
		return 0;
	}

	context->width = width;
	context->height = height;

	return 1;
}

int initGL(sdl_render_context* context, gl_data* data) {
	GLenum glewError;
	gl_shader vert_shader;
	gl_shader frag_shader;

	/* create OpenGL context within our window */
	context->gl_context = SDL_GL_CreateContext(context->window);
	if (context->gl_context == NULL) {
		fprintf(stderr, "Failed to initialize OpenGL context: %s\n", SDL_GetError());
		SDL_ClearError(); // necessary?
		return 0;
	}

	glewExperimental = GL_TRUE; // fixing segfault issue when calling glGenVertexArrays (http://openglbook.com/blog/glgenvertexarrays-access-violationsegfault-with-glew/)
	glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(glewError));
		return 0;
	}

	/* OpenGL settings */
	glViewport(0, 0, context->width, context->height); // optional, default: SDL viewport size
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_DEPTH_TEST);

	// correlation betweeen vertex arrays and vertex buffers???
	create_vertex_array(&data->vertex_array);
	bind_vertex_array(&data->vertex_array);

	create_vertex_shader(&vert_shader, "shaders/shader.vert");
	create_fragment_shader(&frag_shader, "shaders/shader.frag");
	compile_shader(&vert_shader);
	compile_shader(&frag_shader);

	create_program(&data->program);
	attach_shader(&data->program, &vert_shader);
	attach_shader(&data->program, &frag_shader);
	link_program(&data->program);

	// delete shaders as they are not needed after linking the program
	delete_shader(&vert_shader);
	delete_shader(&frag_shader);

	mat4_identity(&data->model_view_matrix);
	mat4_translate(&data->model_view_matrix, -1.5f, 0.0f, -6.0f);
	mat4_perspective(
		&data->projection_matrix,
		45.0, ((double)context->width) / ((double)context->height),
		0.1, 100.0);

	create_triangle(&data->triangle);
	create_quad(&data->quad);

	mat4_print(&data->model_view_matrix);
	printf("\n");
	mat4_print(&data->projection_matrix);
	printf("\n");
	print_geometry(&data->triangle);

	create_array_buffer(&data->triangle_vertex_buffer);
	upload_buffer_data(&data->triangle_vertex_buffer, &data->triangle);

	create_array_buffer(&data->quad_vertex_buffer);
	upload_buffer_data(&data->quad_vertex_buffer, &data->quad);

	request_attrib_location(&data->program, "vertex"); // unused

	use_program(&data->program);

	// TODO put into gl_program.h
	//glBindFragDataLocation(context->program.id, 0, "colorOut");

	// upload matrix data
	glUniformMatrix4fv(glGetUniformLocation(data->program.id, "modelViewMatrix"), 1, GL_FALSE, data->model_view_matrix.data);
	glUniformMatrix4fv(glGetUniformLocation(data->program.id, "projectionMatrix"), 1, GL_FALSE, data->projection_matrix.data);

	return 1;
}

void render(gl_data* data) {
	GLint uniformLocation = glGetUniformLocation(data->program.id, "modelViewMatrix"); // TODO call glGetUniformLocation() only once in the whole program! (costly operation)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear buffer */

	mat4_identity(&data->model_view_matrix);

	/* do rendering stuff */
	mat4_translate(&data->model_view_matrix, -1.5f, 0.0f, -6.0f);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, data->model_view_matrix.data);
	draw_array_buffer(&data->triangle_vertex_buffer, 0); // context->program.attrib_pointer0

	mat4_translate(&data->model_view_matrix, 3.0f, 0.0f, 0.0f);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, data->model_view_matrix.data);
	draw_array_buffer(&data->quad_vertex_buffer, 0);
}

void initRenderLoop(sdl_render_context* context, gl_data* data) {
	int run = 1;
	SDL_Event event;

	while(run) { //Handle events on queue
		// TODO consider integrating this: http://stackoverflow.com/questions/18860243/sdl-pollevent-vs-sdl-waitevent#18863281
		while(SDL_PollEvent(&event) != 0) { //User requests quit
			if(event.type == SDL_QUIT) {
				run = 0;
			}
		}

		render(data);

		/* update screen */
		SDL_GL_SwapWindow(context->window);
	}
}

void cleanup(sdl_render_context* context, gl_data* data) {
	if (context->window != NULL) {
		if (context->gl_context != NULL) {
			SDL_GL_DeleteContext(context->gl_context);

			delete_program(&data->program);

			delete_geometry(&data->triangle);
			delete_geometry(&data->quad);

			delete_buffer_object(&data->triangle_vertex_buffer);
			delete_buffer_object(&data->quad_vertex_buffer);

			delete_vertex_array(&data->vertex_array);
		}

		SDL_DestroyWindow(context->window);
	}

	SDL_Quit();
}

int main() {
	sdl_render_context context;
	gl_data data;

	memset(&context, 0, sizeof(sdl_render_context));
	if (GL_VERSION_1_3) { /* checks if OpenGL 1.3 is supported */
		if (initSDL(&context, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) && initGL(&context, &data)) {
			initRenderLoop(&context, &data);
		}

		cleanup(&context, &data);
	} else {
		printf("OpenGL 3 is not supported!\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
