#ifndef __GL_VERTEX_ARRAY_H
#define __GL_VERTEX_ARRAY_H

#include <GL/glew.h>

/* TODO investigate: what does this even do?? */

typedef struct gl_vertex_array {
	GLuint id;
} gl_vertex_array;

void create_vertex_array(gl_vertex_array* vertex_array);

void bind_vertex_array(gl_vertex_array* vertex_array);

void delete_vertex_array(gl_vertex_array* vertex_array);

#endif
