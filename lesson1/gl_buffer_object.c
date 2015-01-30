#include "gl_buffer_object.h"

void create_array_buffer(gl_buffer_object* array_buffer) {
	glGenBuffers(1, &array_buffer->id);
	array_buffer->type = GL_ARRAY_BUFFER;
	array_buffer->geom = NULL;
}

void upload_buffer_data(gl_buffer_object* array_buffer, geometry* geom) {
	glBindBuffer(array_buffer->type, array_buffer->id);
	glBufferData(array_buffer->type, sizeof(GLfloat) * geom->size * geom->length, geom->vertices, GL_STATIC_DRAW);

	array_buffer->geom = geom;
}

void draw_array_buffer(gl_buffer_object* array_buffer, GLuint attrib_array_index) {
	glEnableVertexAttribArray(attrib_array_index); /* TODO what is happening here? */
	glBindBuffer(array_buffer->type, array_buffer->id);
	glVertexAttribPointer(
	   attrib_array_index,       /* attribute 0; must match the layout in the shader. */
	   array_buffer->geom->size, /* size */
	   GL_FLOAT,                 /* type */
	   GL_FALSE,			        /* normalized? */
	   0,                        /* stride */
	   (void*) 0                 /* array buffer offset */
	);

	glDrawArrays(array_buffer->geom->mode, 0, array_buffer->geom->length); /* Starting from vertex 0; 3 vertices total -> 1 triangle */
	glDisableVertexAttribArray(attrib_array_index);
}

void delete_buffer_object(gl_buffer_object* buffer_object) {
	glDeleteBuffers(1, &buffer_object->id);
}
