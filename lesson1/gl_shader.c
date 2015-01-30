#include "gl_shader.h"

#include <stdio.h>
#include <stdlib.h>

/* TODO: review! */
static GLchar* read_source(const char* source_file) {
	long size;
	GLchar* source;
	FILE* handle = fopen(source_file, "rb");

	fseek(handle, 0, SEEK_END);
	size = ftell(handle);
	rewind(handle);
	source = (GLchar*) malloc(sizeof(GLchar) * (size + 1));
	fread(source, sizeof(GLchar), size, handle);
	source[size] = '\0';
	fclose(handle);
	return source;
}

static void print_info_log(gl_shader* shader) {
	GLchar* log_message;
	GLint log_length;

	glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &log_length);

	log_message = (GLchar*) malloc(sizeof(GLchar) * log_length);
	glGetShaderInfoLog(shader->id, log_length, NULL, log_message);

	fprintf(stderr, "%s\n", log_message);
	free(log_message);
}

static void check_compile_status(gl_shader* shader) {
	GLint status = GL_FALSE;

	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		print_info_log(shader);
	}
}

static void create_shader(gl_shader* shader, GLchar* source_file, GLenum type) {
	shader->id = glCreateShader(type);
	shader->source = read_source(source_file);

	glShaderSource(shader->id, 1, (const GLchar**) &shader->source, NULL);
}

/* interface */

void create_vertex_shader(gl_shader* shader, GLchar* source_file) {
	create_shader(shader, source_file, GL_VERTEX_SHADER);
}

void create_fragment_shader(gl_shader* shader, GLchar* source_file) {
	create_shader(shader, source_file, GL_FRAGMENT_SHADER);
}

void compile_shader(gl_shader* shader) {
	glCompileShader(shader->id);

	check_compile_status(shader);
	free(shader->source);
}

void delete_shader(gl_shader* shader) {
	glDeleteShader(shader->id);
}
