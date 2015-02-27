#ifndef __GL_BUFFER_OBJECT_H
#define __GL_BUFFER_OBJECT_H

#include <GL/glew.h>

#include "geometry.h"

typedef struct gl_buffer_object {
	GLuint id;
	GLenum type;
	GLenum mode;
	GLint size;
	GLsizei length;
} gl_buffer_object;

void create_array_buffer(gl_buffer_object* array_buffer);

void bind_buffer_object(gl_buffer_object* buffer_object);

void upload_buffer_data(gl_buffer_object* array_buffer, GLenum mode, GLint size, GLsizei length, GLfloat* data);

void set_vertex_attrib_pointer(gl_buffer_object* array_buffer, GLuint attrib_array_index);

void draw_array_buffer(gl_buffer_object* array_buffer);

void delete_buffer_object(gl_buffer_object* buffer_object);

#endif
