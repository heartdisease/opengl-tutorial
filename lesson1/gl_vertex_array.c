#include "gl_vertex_array.h"

void create_vertex_array(gl_vertex_array* vertex_array) {
	glGenVertexArrays(1, &vertex_array->id);
}

void bind_vertex_array(gl_vertex_array* vertex_array) {
	glBindVertexArray(vertex_array->id);
}

void delete_vertex_array(gl_vertex_array* vertex_array) {
	glDeleteVertexArrays(1, &vertex_array->id);
}
