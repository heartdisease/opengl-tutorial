#include "gl_buffer_object.h"

void create_array_buffer(gl_buffer_object* array_buffer) {
	glGenBuffers(1, &array_buffer->id);
	array_buffer->type = GL_ARRAY_BUFFER;
}

void bind_buffer_object(gl_buffer_object* buffer_object) {
	glBindBuffer(buffer_object->type, buffer_object->id);
}

void upload_buffer_data(gl_buffer_object* array_buffer, GLenum mode, GLint size, GLsizei length, GLfloat* data) {
	glBufferData(array_buffer->type, sizeof(GLfloat) * size * length, data, GL_STATIC_DRAW);

	array_buffer->mode   = mode;
	array_buffer->size   = size;
	array_buffer->length = length;
}

void set_vertex_attrib_pointer(gl_buffer_object* array_buffer, GLuint attrib_array_index) {
	glVertexAttribPointer(
	   attrib_array_index, /* must match the layout in the shader. */
	   array_buffer->size, /* size */
	   GL_FLOAT,           /* type */
	   GL_FALSE,			  /* TODO: normalized (?) */
	   0,                  /* stride */
	   (void*) 0           /* array buffer offset */
	);
}

void draw_array_buffer(gl_buffer_object* array_buffer) {
	glDrawArrays(array_buffer->mode, 0, array_buffer->length);
}

void delete_buffer_object(gl_buffer_object* buffer_object) {
	glDeleteBuffers(1, &buffer_object->id);
}
